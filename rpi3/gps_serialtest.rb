require 'serialport'
require 'rubygems'
require 'io/console'
require 'net/http'
require 'uri'
require 'csv'

sp = SerialPort.new('/dev/serial0', 9600, 8, 1, 0)
sp.puts "serial0 open"

# echo loop
begin
    CSV.open('data_all.csv','w') do |row|
    end

    loop do
        line = sp.gets
        newLine = line.chomp
        strArr = newLine.split(",")
        if strArr[0] == '$GPGGA' then
            newStr = strArr[0] + ',' + strArr[2] + ',' + strArr[4]
            puts newStr
            #sp.puts line
        end

        CSV.open('data_all.csv','a') do |csv|
            csv << strArr
        end
    end
rescue Interrupt
    sp.close
    exit 0
end
