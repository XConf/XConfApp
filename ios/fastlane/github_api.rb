require 'net/http'
require 'net/https'
require 'json'

def send_github_request(url, access_token)
  uri = URI(url.start_with?("http") ? url : "https://api.github.com#{url}")

  # Create client
  http = Net::HTTP.new(uri.host, uri.port)
  http.use_ssl = true
  http.verify_mode = OpenSSL::SSL::VERIFY_PEER

  # Create Request
  req =  Net::HTTP::Get.new(uri)
  # Add headers
  req.add_field "Accept", "application/vnd.github.symmetra-preview+json"
  # Add headers
  req.add_field "Authorization", "token #{access_token}"

  # Fetch Request
  res = http.request(req)
  JSON.parse res.body
rescue StandardError => e
  puts "HTTP Request failed (#{e.message})"
  false
end

def get_changelog_from_pr(repo, pull_request_number)
  github_access_token = ENV['GITHUB_ACCESS_TOKEN']
  if !github_access_token
    puts 'Set the GITHUB_ACCESS_TOKEN environment variable to get more information in the changelog!'
    return "Build of pull request ##{pull_request_number}."
  end

  changelog = "[PR##{pull_request_number}]"

  pull_request_data = send_github_request("/repos/#{repo}/pulls/#{pull_request_number}", github_access_token)

  if pull_request_data
    changelog += " #{pull_request_data['title']}\n"
    changelog += "by @#{pull_request_data['user']['login']}"
    changelog += ", labels: #{pull_request_data['labels'].map { |label| label['name'] }.join(', ')}\n" if pull_request_data['labels'] && !pull_request_data['labels'].length.zero?
    changelog += "\n"
    changelog += "\n#{pull_request_data['body']}\n" if pull_request_data['body'] && !pull_request_data['body'].length.zero?
    commits_data = send_github_request(pull_request_data['commits_url'], github_access_token)
    if commits_data
      changelog += "\n---\n\nCommits:\n\n"
      changelog += commits_data.map { |c| " - #{c['commit']['message'].lines.first.chomp}" }.join("\n")
      changelog += "\n"
    end
  end

  changelog
end
