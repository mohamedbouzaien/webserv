require 'socket'

starttime = Process.clock_gettime(Process::CLOCK_MONOTONIC)

s = TCPSocket.new 'localhost', 80

s.write("testfiles/#{ARGV[0]}.cpp");

s.each_line do |line|
  #puts line
end

s.close

endtime = Process.clock_gettime(Process::CLOCK_MONOTONIC)
elapsed = endtime - starttime

puts "Elapsed: #{elapsed}"
