#include <SoftwareSerial.h>
#include <SakuraIO.h>
#include <FaBo9Axis_MPU9250.h>

SoftwareSerial mySerial(11, 10);
float latitude_f = 36.11195;
float longitude_f = 140.099092;

FaBo9Axis fabo_9axis;
int mpu_flag = 0;

SakuraIO_I2C sakuraio;

// NMEAの緯度経度を「度分秒」(DMS)の文字列に変換する
//String NMEA2DMS(float val) {
//  int d = val / 100;
//  int m = ((val / 100.0) - d) * 100.0;
//  float s = ((((val / 100.0) - d) * 100.0) - m) * 60;
//  return String(d) + "度" + String(m) + "分" + String(s, 1) + "秒";
//}

// (未使用)NMEAの緯度経度を「度分」(DM)の文字列に変換する
//String NMEA2DM(float val) {
//  int d = val / 100;
//  float m = ((val / 100.0) - d) * 100.0;
//  return String(d) + "度" + String(m, 4) + "分";
//}

// NMEAの緯度経度を「度」(DD)の文字列に変換する
String NMEA2DD(float val) {
  int d = val / 100;
  int m = (((val / 100.0) - d) * 100.0) / 60;
  float s = (((((val / 100.0) - d) * 100.0) - m) * 60) / (60 * 60);
  return String(d + m + s, 6);
}

// UTC時刻から日本の標準時刻に変換する(GMT+9:00)
//String UTC2GMT900(String str) {
//  int hh = (str.substring(0,2).toInt()) + 9;
//  if(hh > 24) hh = hh - 24;
//
//  return String(hh,DEC) + ":" + str.substring(2,4) + ":" + str.substring(4,6);
//}

void setup() {
  mySerial.begin(9600);
  Serial.begin(115200);

  //  /* MPU9250--------------------------*/
  if (fabo_9axis.begin()) {
    Serial.println("configured FaBo 9Axis I2C Brick");
    mpu_flag = 1;
  } else {
    Serial.println("mpu9250 error");
    //while (1);
  }
  /* ---------------------------------*/

}
void loop() {
  //   1つのセンテンスを読み込む
  String line = mySerial.readStringUntil('\n');

  Serial.println(line);

  if (line != "") {
    int i, index = 0, len = line.length();
    String str = "";

    //     StringListの生成(簡易)
    String list[50];
    for (i = 0; i < 50; i++) {
      list[i] = "";
    }

    //     「,」を区切り文字として文字列を配列にする
    for (i = 0; i < len; i++) {
      if (line[i] == ',') {
        list[index++] = str;
        str = "";
        continue;
      }
      str += line[i];
    }

    //     $GPGGAセンテンスのみ読み込む
    for (i = 0; i < index; i++) {
      //      Serial.print("list[");
      //      Serial.print(i);
      //      Serial.print("] = ");
      //      Serial.println(list[i]);
      //if (list[i] == "$GPGGA" || list[i] == "$GP$GPGGA") {
      if (list[i].indexOf("$GPGGA") != -1) {
        //         ステータス
        if (list[i + 6] != "0" && list[i + 5] == "E") {
          //           現在時刻
          //          Serial.print(UTC2GMT900(list[1]));

          //           緯度
          //          Serial.print(" 緯度:");
          //          Serial.print(NMEA2DMS(list[2].toFloat()));
          //          Serial.print("(");
          Serial.print("latitide = ");
          Serial.println(NMEA2DD(list[i + 2].toFloat()));
          latitude_f = NMEA2DD(list[i + 2].toFloat()).toFloat();
          //          Serial.print(")");

          //           経度
          //         Serial.print(" 経度:");
          //         Serial.print(NMEA2DMS(list[4].toFloat()));
          //         Serial.print("(");
          Serial.print("longitude = ");
          Serial.println(NMEA2DD(list[i + 4].toFloat()));
          longitude_f = NMEA2DD(list[i + 4].toFloat()).toFloat();
          //         Serial.print(")");

          //          海抜
          //          Serial.print(" 海抜:");
          //          Serial.print(list[9]);
          //          list[10].toLowerCase();
          //          Serial.print(list[10]);
          break;
        } else {
          Serial.println("測位できませんでした。");
        }
        Serial.println("");
      }
    }
  }

  /* pressure and bend -----------------*/
  int ain0 = analogRead(0);
  int ain1 = analogRead(1);
  int ain2 = analogRead(2);
  int ain3 = analogRead(3);
  //  //int ain4 = analogRead(4);
  //  //int ain5 = analogRead(5);
  //
  //  Serial.print("Pressure: ");
  //  Serial.print(ain0); Serial.print(",");
  //  Serial.print(ain1); Serial.print(",");
  //  Serial.print(ain2); Serial.print(",");
  //  Serial.println(ain3);
  //  /* -----------------------------------*/

  /* accelerometer ---------------------*/
  float ax = 0, ay = 0, az = 0;
  float gx = 0, gy = 0, gz = 0;
  float mx = 0, my = 0, mz = 0;
  float temp = 0;
  if (mpu_flag == 1) {
    fabo_9axis.readAccelXYZ(&ax, &ay, &az);
    fabo_9axis.readGyroXYZ(&gx, &gy, &gz);
    fabo_9axis.readMagnetXYZ(&mx, &my, &mz);
  } else {
    if (fabo_9axis.begin()) {
      Serial.println("configured FaBo 9Axis I2C Brick");
      mpu_flag = 1;
    } else {
      Serial.println("mpu9250 error");
    }
  }
  Serial.print("xyz: ");
  Serial.print(ax);
  Serial.print(",");
  Serial.print(ay);
  Serial.print(",");
  Serial.println(az);
  /*-----------------------------------*/

  sakuraio.enqueueTx((uint8_t)0, latitude_f);
  sakuraio.enqueueTx((uint8_t)1, longitude_f);
  sakuraio.enqueueTx((uint8_t)2, (float)ain0);
  sakuraio.enqueueTx((uint8_t)3, (float)ain1);
  sakuraio.enqueueTx((uint8_t)4, (float)ain2);
  sakuraio.enqueueTx((uint8_t)5, (float)ain3);
  sakuraio.enqueueTx((uint8_t)6, (float)ax);
  sakuraio.enqueueTx((uint8_t)7, (float)ay);
  sakuraio.enqueueTx((uint8_t)8, (float)az);
  sakuraio.send();

  delay(900);
}
