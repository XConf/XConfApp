require 'net/http'
require 'net/https'
require 'json'
require 'redcarpet'
require 'redcarpet/render_strip'

def send_github_request(url, access_token, data = nil)
  uri = URI(url.start_with?("http") ? url : "https://api.github.com#{url}")

  # Create client
  http = Net::HTTP.new(uri.host, uri.port)
  http.use_ssl = true
  http.verify_mode = OpenSSL::SSL::VERIFY_PEER

  if data
    # Create Request
    req = Net::HTTP::Post.new(uri)
    # Add headers
    req.add_field "Accept", "application/vnd.github.symmetra-preview+json"
    # Add headers
    req.add_field "Authorization", "token #{access_token}"
    # Add headers
    req.add_field "Content-Type", "application/json"
    # Set body
    req.body = data.to_json
  else
    # Create Request
    req =  Net::HTTP::Get.new(uri)
    # Add headers
    req.add_field "Accept", "application/vnd.github.symmetra-preview+json"
    # Add headers
    req.add_field "Authorization", "token #{access_token}"
  end

  # Fetch Request
  res = http.request(req)
  JSON.parse res.body
rescue StandardError => e
  puts "HTTP Request failed (#{e.message})"
  false
end

def get_changelog_from_pr(repo, pull_request_number)
  github_access_token = ENV['GH_TOKEN']
  if !github_access_token
    puts 'Set the GH_TOKEN environment variable to get more information in the changelog!'
    return "Build of pull request ##{pull_request_number}."
  end

  changelog = "[PR##{pull_request_number}]"

  pull_request_data = send_github_request("/repos/#{repo}/pulls/#{pull_request_number}", github_access_token)

  if pull_request_data
    changelog += " #{pull_request_data['title']}\n"
    changelog += "by @#{pull_request_data['user']['login']}"
    changelog += ", labels: #{pull_request_data['labels'].map { |label| label['name'] }.join(', ')}\n" if pull_request_data['labels'] && !pull_request_data['labels'].length.zero?
    changelog += "\n"
    if pull_request_data['body'] && !pull_request_data['body'].strip.length.zero?
      md_renderer = Redcarpet::Markdown.new(Redcarpet::Render::StripDown)
      pull_request_data_body = md_renderer.render(pull_request_data['body'].strip.gsub(/<\/?[^>]*>/, '').gsub(/:[a-zA-Z\-_]+:[\t ]*/, ''))
      pull_request_data_body = "#{pull_request_data_body[0...2000]}... (truncated)" if pull_request_data_body.length > 2000
      changelog += "\n#{pull_request_data_body}\n"
    end
    commits_data = send_github_request(pull_request_data['commits_url'], github_access_token)
    if commits_data
      changelog += "\n---\n\nCommits:\n\n"
      changelog += commits_data.map { |c| " - #{c['commit']['message'].lines.first.chomp}" }.join("\n")
      changelog += "\n"
    end
  end

  changelog = changelog.gsub('→', '->')
  changelog = changelog.gsub(/[\u{1F300}-\u{1F5FF}|\u{1F1E6}-\u{1F1FF}|\u{2700}-\u{27BF}|\u{1F900}-\u{1F9FF}|\u{1F600}-\u{1F64F}|\u{1F680}-\u{1F6FF}|\u{2600}-\u{26FF}][^A-Za-z0-9 \-_]*[\t ]*/, '')

  changelog
end

def post_comment_on_issue(repo, issue_number, body)
  github_access_token = ENV['GH_TOKEN']
  if !github_access_token
    puts 'Set the GH_TOKEN environment variable to have comments post!'
    return
  end

  send_github_request("/repos/#{repo}/issues/#{issue_number}/comments", github_access_token, { body: body })
end

class GitHubDeployment
  def initialize(url)
    @url = url
  end

  def create_status(state, target_url: '')
    github_access_token = ENV['GH_TOKEN']
    if !github_access_token
      puts 'Set the GH_TOKEN environment variable to update deployment status!'
      return
    end

    send_github_request("#{@url}/statuses", github_access_token, { state: state, target_url: target_url })
  end
end

def create_github_deployment(repo, ref, environment)
  github_access_token = ENV['GH_TOKEN']
  if !github_access_token
    puts 'Set the GH_TOKEN environment variable to update deployment status!'
    return
  end

  response = send_github_request("/repos/#{repo}/deployments", github_access_token, { ref: ref, environment: environment, required_contexts: [] })

  GitHubDeployment.new(response['url'])
end

module Redcarpet
  module Render
    class StripDown
      def list_item(content, list_type)
        case list_type
        when :ordered
          "- #{content}\n"
        when :unordered
          "- #{content}\n"
        end
      end
    end
  end
end
