//
// 105-03 $GPGGA 個別データ選別実験スケッチ
//
 
#include <string.h> // strncmp に必要
 
#define pinGpsRx 10
#define pinGpsTx 11
 
#define SerialBaudrate 9600
#define GpsBaudrate 9600
 
#include <SoftwareSerial.h>
SoftwareSerial sGps(pinGpsRx, pinGpsTx);
 
void setup() {
  Serial.begin(SerialBaudrate);
  Serial.println("Software Serial Test Start!");
 
  sGps.begin(GpsBaudrate);
}
 
void loop() {
  char buf[256];
  char c;
  int count = 0;
  char *gpsTime, *gpsLat, *gpsLong, *gpsTemp; // 個別データ用の文字列用ポインタ
 
  do {
    if (sGps.available()) {
      buf[count] = sGps.read();
      count++;
    }
    if (count > 250) break;
  } while(buf[count - 1] != 0x0A);
  buf[count] = '\0';
 
  if (0 == strncmp("$GPGGA", buf, 6)) {
    strtok(buf, ",");
    gpsTime = strtok(NULL, ","); // UTC時刻の抽出
    gpsLat = strtok(NULL, ",");  // 緯度の抽出
    strtok(NULL, ",");
    gpsLong = strtok(NULL, ","); // 経度の抽出
 
    Serial.print("Time = ");
    Serial.print(gpsTime);
    Serial.print(" : ");
    Serial.print("Latitude = ");
    Serial.print(gpsLat);
    Serial.print(" : ");
    Serial.print("Longitude = ");
    Serial.println(gpsLong);
  }
}
