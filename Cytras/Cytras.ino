// FaBo 202 9Axis MPU9250 - Version: Latest 
#include <FaBo9Axis_MPU9250.h>
#include <Wire.h>

#include<SakuraIO.h>

//library for gps module
#include <TinyGPS++.h>
#include <SoftwareSerial.h>


#define pinGpsRx 10 //GPS Rx (Data Out) のピン番号
#define pinGpsTx 11 //GPS Tx (Data In) のピン番号

#define Brate 9600 //シリアル通信のボーレート

TinyGPSPlus gps;
SoftwareSerial mySerial(pinGpsRx, pinGpsTx);
//TinyGPSCustom magneticVariation(gps, "GPRMC", 10);

FaBo9Axis fabo_9axis;

//SakuraIO_SPI sakuraio(10);
SakuraIO_I2C sakuraio;

void setup() {
  Serial.begin(Brate);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for native USB port only
  }
 
  //Serial.println("Begin");
  
  /* LED 13 */
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  //mySerial.println("Software Serial inited.");
  
  // mpu9250
  if (fabo_9axis.begin()) {
    Serial.println("configured FaBo 9Axis I2C Brick");
  } else {
    Serial.println("mpu9250 error");
    while(1);
  }
}
 
void loop() { // run over and over

  /* accelerometer */
  float ax,ay,az;
  float gx,gy,gz;
  float mx,my,mz;
  float temp;
  
  fabo_9axis.readAccelXYZ(&ax,&ay,&az);
  fabo_9axis.readGyroXYZ(&gx,&gy,&gz);
  fabo_9axis.readMagnetXYZ(&mx,&my,&mz);
  //fabo_9axis.readTemperature(&temp);


  /* pressure and bend */
  int ain0 = analogRead(0);
  int ain1 = analogRead(1);
  int ain2 = analogRead(2);
  int ain3 = analogRead(3);
  //int ain4 = analogRead(4);
  //int ain5 = analogRead(5);

   String lat = "36.11195";
   String lng = "140.099092";
  
   //GPSのシリアルデータの読み込み
   while (mySerial.available() > 0){
     char c_gps = mySerial.read();
     Serial.print(c_gps);
     gps.encode(c_gps);
    
     lat = String(gps.location.lat());
     lng = String(gps.location.lng());
  
     if (gps.location.isValid()){
       Serial.print("LAT="); Serial.println(gps.location.lat(), 6);
       Serial.print("LONG="); Serial.println(gps.location.lng(), 6);
       Serial.print("ALT="); Serial.println(gps.altitude.meters());
       break;
     }
   }
  

  Serial.print("P:");
  Serial.print(ain0); Serial.print(",");
  Serial.print(ain1); Serial.print(",");
  Serial.print(ain2); Serial.print(",");
  Serial.println(ain3); 
  
  Serial.print("xyz: ");
  Serial.print(ax);
  Serial.print(",");
  Serial.print(ay);
  Serial.print(",");
  Serial.println(az);

  
  //sendSakuraIo((float)ain0, (float)ain1);
 
    
  delay(1000);
}


void sendSakuraIo(float a,float b){
 //キューにデータを設定
 sakuraio.enqueueTx((uint8_t)0,(float)a);
 sakuraio.enqueueTx((uint8_t)1,(float)b);
 //データを送信
 sakuraio.send();
}
  
