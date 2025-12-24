Arduino for Magnesium Nitro Control System/Compass QMC5883L

This library supports the QMC5883L, which will address the BZ-251.
The BZ-251 function and the I2C address of the metal are 0x0C, but it returns zero. Most live displays have the QMC5883L! Hilton. This library looks good.

Check

Get the latest updates from GitHub:
https://github.com/hs4rzo/MyQMC5883L.git

In your Arduino app, menu selection:
Sketch → Include Library → Add Library .ZIP, then select the downloaded file.

Preliminary
Direction declaration
#include <Wire.h>
#include <QMC5883L.h>

Compass QMC5883L;

Start the game
// Wire.begin(); // Never called because the library calls it automatically
Compass.startInit();

Read raw magnetic field and temperature values
intR;
int16_t x, y, z, t;
r = Compass.readRaw(&x, &y, &z, &t);

Read the calibrated direction value.

The readHeading() function may be 0 to initialize that the compass is not calibrated.
Call readHeading() continuously while controlling on all axes.

int heading = Compass.readHeading(); // The remainder 1–360 after that is calibrated.

The reader is not a block.

The readHeading360() and readHeading180() functions error or 999 if data is not yet available.

int heading360 = Compass.readHeading360();
int heading180 = Compass.readHeading180();

Or the ready() method before reading.

if (compass.ready()) {
int heading = Compass.readHeading();
- Do nothing further.
Compass.setSamplingRate(rate); // 10, 50, 100, 200 Hz
Compass setRange(range); // 2 or 8 Gauss
Compass setOversampling(ovl); // 512, 256, 128, 64

Above auxiliary functions

int getHeadingError(int targetHeading, int currentHeading);

int readHeading();

int readHeading360();

int readHeading180();

void getDirection(char *myArray, int heading360);

boolean getXY(float & fx, float & fy);

byte getBearing(int heading 360);


ไลบรารี Arduino สำหรับแม็กนีโตมิเตอร์/เข็มทิศ QMC5883L

ไลบรารีนี้รองรับชิป QMC5883L ซึ่งอยู่กับเซ็นเซอร์ BZ-251
หากคุณมีเซ็นเซอร์ BZ-251 และสังเกตเห็นที่อยู่ I2C เป็น 0x0C แต่อ่านค่ากลับมาเป็นศูนย์ แสดงว่าคุณน่าจะมีชิป QMC5883L! ลองใช้ไลบรารีนี้ดู

การติดตั้ง

ดาวน์โหลดไฟล์เวอร์ชันล่าสุดจาก GitHub:
https://github.com/hs4rzo/MyQMC5883L.git

ในแอป Arduino ให้เลือกเมนู:
Sketch → Include Library → Add .ZIP Library แล้วเลือกไฟล์ที่ดาวน์โหลดมา

การใช้งานเบื้องต้น
ประกาศอินสแตนซ์เข็มทิศ
#include <Wire.h>
#include <QMC5883L.h>

QMC5883L compass;

เริ่มต้นเข็มทิศ
// Wire.begin(); // ไม่ต้องเรียกเพราะไลบรารีเรียกให้แล้ว
compass.startInit();

อ่านค่าสนามแม่เหล็กและอุณหภูมิแบบดิบ
int r;
int16_t x, y, z, t;
r = compass.readRaw(&x, &y, &z, &t);

อ่านค่าทิศทางที่ปรับเทียบแล้ว

ฟังก์ชัน readHeading() จะคืนค่า 0 ในตอนเริ่มต้น เพื่อบ่งชี้ว่าเข็มทิศยังไม่ได้ปรับเทียบ
ให้เรียก readHeading() ต่อเนื่องในขณะที่หมุนเข็มทิศบนทุกแกน

int heading = compass.readHeading(); // คืนค่า 1–360 หลังจากปรับเทียบ

การอ่านแบบไม่บล็อก

ฟังก์ชัน readHeading360() และ readHeading180() คืนค่า error หรือ 999 หากข้อมูลยังไม่พร้อมใช้งาน

int heading360 = compass.readHeading360();
int heading180 = compass.readHeading180();


หรือใช้เมธอด ready() เพื่อตรวจสอบก่อนอ่าน

if (compass.ready()) {
    int heading = compass.readHeading();
}

ปรับประสิทธิภาพของชิป
compass.setSamplingRate(rate);   // 10, 50, 100, 200 Hz
compass.setRange(range);         // 2 หรือ 8 Gauss
compass.setOversampling(ovl);    // 512, 256, 128, 64

ฟังก์ชันเสริมที่มีประโยชน์

int getHeadingError(int targetHeading, int currentHeading);

int readHeading();

int readHeading360();

int readHeading180();

void getDirection(char* myArray, int heading360);

bool getXY(float& fx, float& fy);

byte getBearing(int heading360);