# GPSサドル ハードウェア
Team. 加藤恵

## 🎉 リリースノート
- [08/02, 18:00] ❤️ RPi3でi2c通信を用いて9軸センサの取得

## 💾 プログラム (TEST 1)
センサ類の動作テスト用に作成したArduinoプログラム

### Arduinoプログラム
arduino/
- GPSロガー（GT723F）と9軸センサ(MPU9250)の動作テストプログラム

#### GPSロガー
arduino/gt723f_raw/gt723f_raw.ino
- GPSロガーをUARTで接続．
- 受け取った値をそのままコンソールに出力

arduino/gt723f_test/gt723f_test.ino
- GPSロガーをUARTで接続．
- 時間，緯度および軽度を出力

#### 9軸センサ
arduino/mpu9250_arduino/mpu9250_arduino.ino
- 9軸センサをi2c通信で接続
- 9軸の値を出力

### Raspberry Pi3プログラム (TEST 2)

#### GPSロガー
- $ sudo apt-get install ruby ruby-dev
- $ sudo gem install serialport
- $ cd [PROJECT]/rpi3/gps_serial.rb
- $ ruby gps_serial.rb
