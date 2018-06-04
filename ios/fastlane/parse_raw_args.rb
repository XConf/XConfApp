def parse_raw_args(raw_args)
  command_args = raw_args.split('%%,%%')
  command_args.reduce(last: '', hash: {}) do |acc, arg|
    unless arg.length.zero?
      if acc[:last].start_with?('-') && !arg.start_with?('-')
        key = acc[:last]
        if !acc[:hash][key] || acc[:hash][key].is_a?(TrueClass)
          acc[:hash][key] = arg
        elsif acc[:hash][key].is_a?(Array)
          acc[:hash][key].push(arg)
        else
          acc[:hash][key] = [acc[:hash][key], arg]
        end
      else
        acc[:hash][arg] = true unless acc[:hash][arg]
      end
      acc[:last] = arg
      acc
    end
    acc
  end[:hash]
end
