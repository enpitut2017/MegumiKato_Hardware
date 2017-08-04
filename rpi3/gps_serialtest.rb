require 'serialport'
require 'rubygems'
require 'io/console'
require 'net/http'
require 'uri'
require 'csv'

sp = SerialPort.new('/dev/serial0', 9600, 8, 1, 0)
sp.puts "serial0 open"

# echo loop
loop do
  line = sp.gets

  newLine = line.chomp
  strArr = newLine.split(",")
  if strArr[0] == '$GPGGA' then
    newStr = strArr[0] + ',' + strArr[2] + ',' + strArr[4]
    puts newStr
    #sp.puts line
  end
end
sp.close
