#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <SoftwareSerial.h>

#define pinGpsRx 10 //GPS Rx (Data Out) のピン番号
#define pinGpsTx 11 //GPS Tx (Data In) のピン番号
 
#define SerialBaudrate 115200 //シリアル通信のボーレート
#define GpsBaudrate 9600 //GPSのボーレート

SoftwareSerial sGps(pinGpsRx, pinGpsTx);
Process process;

void setup() {
  /* for console output */
  Serial.begin(SerialBaudrate);
  Serial.println("Software Serial Test Start!");

  /* LED 13 */
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  /* for http post */
  Bridge.begin();

  /* for gps*/
  sGps.begin(GpsBaudrate);
}

void loop() {
  /* gps */
  char buf[256];
  char c;
  int count = 0;
//  char *gpsTime, *gpsLat, *gpsLong, *gpsTemp; // 個別データ用の文字列用ポインタ
  String gpsTime, gpsLat, gpsLong, gpsTemp; // 個別データ用の文字列用ポインタ
  bool p_en = false;

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

    p_en = true;

    /* ★TEST★ */
//    gpsLat = "3606.7170";
//    gpsLong = "14005.9455";
  }

  /*post*/
  if(p_en && !(gpsLat=="0000.0000") ){
    digitalWrite(13, LOW);
    Serial.print(": SENDING :");
    Serial.print(gpsLat);
    String s1 = "curl -X POST -H \"Content-Type: application/json\" -H \"Accept: application/json\" -d  '{\"serial\":\"raspi\", \"latitude\":";
    String s2 = ", \"longitude\":";
    String s3 = "}' http://www12450ue.sakura.ne.jp/positions";
    String s0 =  String( s1 + gpsLat + s2 + gpsLong + s3);
    process.runShellCommand(s0);
    delay(1000);
    Serial.println(" NEXT");
  }
}
