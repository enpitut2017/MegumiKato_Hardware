#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>
#include <SoftwareSerial.h>
#include <FileIO.h>
#include <FaBo9Axis_MPU9250.h>
#include <Wire.h>

#define pinGpsRx 10 //GPS Rx (Data Out) のピン番号
#define pinGpsTx 11 //GPS Tx (Data In) のピン番号
 
#define SerialBaudrate 115200 //シリアル通信のボーレート
#define GpsBaudrate 9600 //GPSのボーレート

SoftwareSerial sGps(pinGpsRx, pinGpsTx);
Process process;

FaBo9Axis fabo_9axis;

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
  
      // mpu9250
  if (fabo_9axis.begin()) {
    Serial.println("configured FaBo 9Axis I2C Brick");
  } else {
    Serial.println("mpu9250 error");
    while(1);
  }
}

void loop() {
  
      /* accelerometer */
  float ax,ay,az;
  float gx,gy,gz;
  float mx,my,mz;
  float temp;
  
  fabo_9axis.readAccelXYZ(&ax,&ay,&az);
  fabo_9axis.readGyroXYZ(&gx,&gy,&gz);
  fabo_9axis.readMagnetXYZ(&mx,&my,&mz);
  
  /* pressure and bend */
  int ain0 = analogRead(0);
  int ain1 = analogRead(1);
  int ain2 = analogRead(2);
  int ain3 = analogRead(3);
  int ain4 = analogRead(4);
  int ain5 = analogRead(5);
  
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
 
    // Serial.print(ain0);
    // Serial.print(", ");
    // Serial.print(ain1);
    // Serial.print(", ");
    // Serial.print(ain2);
    // Serial.print(", ");
    // Serial.print(ain3);
    // Serial.print(", ");
    // Serial.print(ain4);
    // Serial.print(", ");
    // Serial.print(ain5);
    // Serial.print(", ");
    // Serial.print("Time = ");
    // Serial.print(gpsTime);
    // Serial.print(" : ");
    // Serial.print("Latitude = ");
    // Serial.print(gpsLat);
    // Serial.print(" : ");
    // Serial.print("Longitude = ");
    // Serial.println(gpsLong);

    p_en = true;

    /* ★TEST★ */
    //gpsLat = "3606.7170";
    //gpsLong = "14005.9455";
  }
  
  /*post*/
  if(p_en && !(gpsLat=="0000.0000") ){
    //digitalWrite(13, LOW);
    Serial.print("[CHECK] ");
    String s1 = "curl -X POST -H \"Content-Type: application/json\" -H \"Accept: application/json\" -d  '{\"serial\":\"raspi\", \"latitude\":";
    String s2 = ", \"longitude\":";
    String p0 = ", \"press_zero\":";
    String p1 = ", \"press_one\":";
    String p2 = ", \"press_two\":";
    String p3 = ", \"press_three\":";
    String b0 = ", \"accel_x\":";
    String b1 = ", \"accel_y\":";
    String s3 = "}' http://www12450ue.sakura.ne.jp/positions";
    // String s0 =  String( s1 + gpsLat + s2 + gpsLong + p0 + String(ain0) + p1 + String(ain1) + p2 + String(ain4) + p3 + String(ain5) + b0 + String(ain2) + b1 + String(ain3) + s3);
    // String s0 =  String( s1 + gpsLat + s2 + gpsLong + s3);
    Serial.print(s1);
    Serial.print(gpsLat);
    Serial.print(s2);
    Serial.print(gpsLong);
    Serial.print(p0);
    Serial.print(String(ain0));
    Serial.print(p1);
    Serial.print(String(ain1));
    Serial.print(p2);
    Serial.print(String(ain2));
    Serial.print(p3);
    Serial.print(String(ain3));
    Serial.print(b0);
    Serial.print(String(ax));
    Serial.print(b1);
    Serial.print(String(ay));
    Serial.print(s3);
    
    
    /*おくるとこ*/
    Serial.println("[SEND]");
    // process.runShellCommand(s0);
    
    File script = FileSystem.open("/tmp/myscript.sh", FILE_WRITE);
    script.print("#!/bin/sh\n");
    script.print(s1);
    script.print(gpsLat);
    script.print(s2);
    script.print(gpsLong);
    script.print(p0);
    script.print(String(ain0));
    script.print(p1);
    script.print(String(ain1));
    script.print(p2);
    script.print(String(ain2));
    script.print(p3);
    script.print(String(ain3));
    script.print(b0);
    script.print(String(ax));
    script.print(b1);
    script.print(String(ain3));
    script.print(s3);
    script.close();  // close the file
    
    // Make the script executable
    Process chmod;
    chmod.begin("chmod");      // chmod: change mode
    chmod.addParameter("+x");  // x stays for executable
    chmod.addParameter("/tmp/myscript.sh");  // path to the file to make it executable
    chmod.run();
    
    Process myscript;
    myscript.begin("/tmp/myscript.sh");
    myscript.run();
    
    String output = "";
    
    // read the output of the script
    while (myscript.available()) {
      output += (char)myscript.read();
    }
    // remove the blank spaces at the beginning and the ending of the string
    output.trim();
    Console.println(output);
    Console.flush();
    
    delay(1000);
  }
}
