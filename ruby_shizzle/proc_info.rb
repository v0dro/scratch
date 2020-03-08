pid = ARGV[0]

f = File.open "/proc/#{pid.to_i}/status"

f.each_line do |l|
  m = l.match(/VmSize:\s+(\d+)\s+kB/)
  if m
    puts m[1].to_i
  end
end
