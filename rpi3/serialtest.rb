require 'serialport'

# 9600bps
# データビット:8ビット
# ストップビット:1ビット
# パリティなし
sp = SerialPort.new('/dev/serial0', 9600, 8, 1, 0)
sp.puts "serial0 open"

# echo loop
loop do
  line = sp.gets
  # 確認のため受信文字列を標準出力
  puts line
  sp.puts line
end

sp.close
