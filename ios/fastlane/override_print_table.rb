# Override FastlaneCore::PrintTable to prevent actions from printing sensitive data
class FastlaneCore::PrintTable
  class << self
    alias print_values_original print_values
    alias transform_output_original transform_output

    def print_values(params)
      hide_keys = [:git_url]

      if params[:hide_keys].is_a? Array
        params[:hide_keys].concat(hide_keys)
      else
        params[:hide_keys] = hide_keys
      end

      print_values_original(params)
    end

    def transform_output(rows, transform: :newline)
      rows.each { |row| row[1] = '********' if row[0] == :git_url }
      transform_output_original(rows, transform: transform)
    end
  end
end
