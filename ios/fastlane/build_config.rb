require 'json'

def build_config
  return @build_config if @build_config
  build_config_file_path = File.join(File.dirname(__FILE__), '..', '..', 'build-config.json')
  build_config_file = File.read(build_config_file_path)
  @build_config = JSON.parse(build_config_file)
rescue JSON::ParserError
  error_message = "An error occured while parsing the build config. Hint: if you're setting it's value by a environment variable, you should put single quotes around it. E.g. `BUILD_CONFIG='{\"key\":\"value\"}'`"
  UI.error error_message if defined? UI.error
  raise error_message
end
