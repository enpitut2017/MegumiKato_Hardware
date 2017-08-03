//
// 105-01 GPS実験スケッチ
//
 
#define pinGpsRx 10 //GPS Rx (Data Out) のピン番号
#define pinGpsTx 11 //GPS Tx (Data In) のピン番号
 
#define SerialBaudrate 9600 //シリアル通信のボーレート
#define GpsBaudrate 9600 //GPSのボーレート
 
#include <SoftwareSerial.h>
SoftwareSerial sGps(pinGpsRx, pinGpsTx);  // ソフトウェアシリアルの設定
 
void setup() {
  Serial.begin(SerialBaudrate);  // シリアル通信の初期化
  Serial.println("Software Serial Test Start!");  // 開始メッセージの出力
 
  sGps.begin(GpsBaudrate);  // ソフトウェアシリアルの初期化
}
 
void loop() {
  if (sGps.available()) {  
    Serial.write(sGps.read());
  }
}
