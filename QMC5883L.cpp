#include "Arduino.h"
#include <math.h>
#include "QMC5883L.h"
#include <Wire.h>
/*
 * QMC5883L 	BZ-251
 * https://manuals.plus/ae/1005006633720232
 */

/* The default I2C address of this chip */
#define QMC5883L_ADDR 0x0D

/* Register numbers */
#define QMC5883L_X_LSB 0
#define QMC5883L_X_MSB 1
#define QMC5883L_Y_LSB 2
#define QMC5883L_Y_MSB 3
#define QMC5883L_Z_LSB 4
#define QMC5883L_Z_MSB 5
#define QMC5883L_STATUS 6
#define QMC5883L_TEMP_LSB 7
#define QMC5883L_TEMP_MSB 8
#define QMC5883L_CONFIG 9
#define QMC5883L_CONFIG2 10
#define QMC5883L_RESET 11
#define QMC5883L_RESERVED 12
#define QMC5883L_CHIP_ID 13

/* Bit values for the STATUS register */
#define QMC5883L_STATUS_DRDY 1
#define QMC5883L_STATUS_OVL 2
#define QMC5883L_STATUS_DOR 4

/* Oversampling values for the CONFIG register */
#define QMC5883L_CONFIG_OS512 0b00000000
#define QMC5883L_CONFIG_OS256 0b01000000
#define QMC5883L_CONFIG_OS128 0b10000000
#define QMC5883L_CONFIG_OS64  0b11000000

/* Range values for the CONFIG register */
#define QMC5883L_CONFIG_2GAUSS 0b00000000
#define QMC5883L_CONFIG_8GAUSS 0b00010000

/* Rate values for the CONFIG register */
#define QMC5883L_CONFIG_10HZ   0b00000000
#define QMC5883L_CONFIG_50HZ   0b00000100
#define QMC5883L_CONFIG_100HZ  0b00001000
#define QMC5883L_CONFIG_200HZ  0b00001100

/* Mode values for the CONFIG register */
#define QMC5883L_CONFIG_STANDBY 0b00000000
#define QMC5883L_CONFIG_CONT    0b00000001

/* Apparently M_PI isn't available in all environments. */
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

static void write_register( int addr, int reg, int value )
{
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

static int read_register( int addr, int reg, int count )
{
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  
  Wire.requestFrom(addr,count);
  int n = Wire.available();
  if(n!=count) return 0;

  return n;
}

void QMC5883L::reconfig()
{
  write_register(addr,QMC5883L_CONFIG,oversampling|range|rate|mode);  
}

void QMC5883L::reset()
{
  write_register(addr,QMC5883L_RESET,0x01);
  reconfig();
}

void QMC5883L::setOversampling( int x )
{
  switch(x) {
    case 512:
      oversampling = QMC5883L_CONFIG_OS512;
      break;
    case 256:
      oversampling = QMC5883L_CONFIG_OS256;
      break;
    case 128:
      oversampling = QMC5883L_CONFIG_OS128;
      break;
    case 64:
      oversampling = QMC5883L_CONFIG_OS64;
      break;
  } 
  reconfig();
}

void QMC5883L::setRange( int x )
{
  switch(x) {
    case 2:
      range = QMC5883L_CONFIG_2GAUSS;
      break;
    case 8:
      range = QMC5883L_CONFIG_8GAUSS;
      break;
  }
  reconfig();
}

void QMC5883L::setSamplingRate( int x )
{
  switch(x) {
    case 10:
      rate = QMC5883L_CONFIG_10HZ;
      break;
    case 50:
      rate = QMC5883L_CONFIG_50HZ;
      break;
    case 100:
      rate = QMC5883L_CONFIG_100HZ;
      break;
    case 200:
      rate = QMC5883L_CONFIG_200HZ;
      break;
  }
  reconfig();
}



//ของเดิม  แต่เราจะไม่ใช้
void QMC5883L::init() {	
  Wire.begin();
  /* This assumes the wire library has been initialized. */
  addr = QMC5883L_ADDR;
  oversampling = QMC5883L_CONFIG_OS512;
  range = QMC5883L_CONFIG_2GAUSS;
  rate = QMC5883L_CONFIG_100HZ;  // DIY
  mode = QMC5883L_CONFIG_CONT;
  reset();
}

int QMC5883L::ready()
{
  if(!read_register(addr,QMC5883L_STATUS,1)) return 0;
  uint8_t status = Wire.read();
  return status & QMC5883L_STATUS_DRDY; 
}

int QMC5883L::readRaw( int16_t *x, int16_t *y, int16_t *z, int16_t *t )
{
  while(!ready()) {}
  if(!read_register(addr,QMC5883L_X_LSB,6)) return 0;
  *x = Wire.read() | (Wire.read()<<8);
  *y = Wire.read() | (Wire.read()<<8);
  *z = Wire.read() | (Wire.read()<<8);

  return 1;
}
int QMC5883L::readRaw_nonblock( int16_t *x, int16_t *y, int16_t *z, int16_t *t )
{

   if(!ready()) return 0;
  if(!read_register(addr,QMC5883L_X_LSB,6)) return 0;

  *x = Wire.read() | (Wire.read()<<8);
  *y = Wire.read() | (Wire.read()<<8);
  *z = Wire.read() | (Wire.read()<<8);

  return 1;
}
void QMC5883L::resetCalibration() {
  xhigh = yhigh = 0;
  xlow = ylow = 0;
}


//ของใหม่ที่จะลอง
bool QMC5883L::startInit() {
    Wire.begin();

    addr = QMC5883L_ADDR;
    oversampling = QMC5883L_CONFIG_OS512;
    range        = QMC5883L_CONFIG_2GAUSS;
    rate         = QMC5883L_CONFIG_100HZ;
    mode         = QMC5883L_CONFIG_CONT;

    // reset chip
    if (!write_register(addr, QMC5883L_RESET, 0x01))
        return false;

    delay(10);

    // config
    if (!write_register(addr, QMC5883L_CONFIG,
        oversampling | range | rate | mode))
        return false;

    // ---- PROBE ครั้งเดียว ----
    uint8_t status;
    if (!read_register(addr, QMC5883L_STATUS, &status))
        return false;

    return true;
}

int QMC5883L::readHeading()
{
  int16_t x, y, z, t;
  if(!readRaw(&x,&y,&z,&t)) return 0;
  if(x<xlow) xlow = x;
  if(x>xhigh) xhigh = x;
  if(y<ylow) ylow = y;
  if(y>yhigh) yhigh = y;
  if( xlow==xhigh || ylow==yhigh ) return 0;
  x -= (xhigh+xlow)/2;
  y -= (yhigh+ylow)/2;
   
  float fx = (float)x/(xhigh-xlow);
  float fy = (float)y/(yhigh-ylow);
  int heading = 180.0*atan2(fy,fx)/M_PI;
  if(heading<=0) heading += 360;  
  return (360 - heading); // ทำให้กลับเป็นองศาเดิมในมุมมองปกติบนภาคพื้น
}

int QMC5883L::readHeading360()
{
  int16_t x, y, z, t;
  if(!readRaw(&x,&y,&z,&t)) return 999;
  if(x<xlow) xlow = x;
  if(x>xhigh) xhigh = x;
  if(y<ylow) ylow = y;
  if(y>yhigh) yhigh = y;
  if( xlow==xhigh || ylow==yhigh ) return 999;
  x -= (xhigh+xlow)/2;
  y -= (yhigh+ylow)/2;
   
  float fx = (float)x/(xhigh-xlow);
  float fy = (float)y/(yhigh-ylow);
  int heading = 180.0*atan2(fy,fx)/M_PI;
  if(heading<=0) heading += 360;  
  return (360 - heading); // ทำให้กลับเป็นองศาเดิมในมุมมองปกติบนภาคพื้น
}


int QMC5883L::readHeading180()
{
  int16_t x, y, z, t;
  if(!readRaw(&x,&y,&z,&t)) return 999;
  if(x<xlow) xlow = x;
  if(x>xhigh) xhigh = x;
  if(y<ylow) ylow = y;
  if(y>yhigh) yhigh = y;
  if( xlow==xhigh || ylow==yhigh ) return 999;
  x -= (xhigh+xlow)/2;
  y -= (yhigh+ylow)/2;   
  float fx = (float)x/(xhigh-xlow);
  float fy = (float)y/(yhigh-ylow);
  int heading = atan2(fy, fx) * 180.0 / M_PI;
 // if (heading > 180)  heading -= 360;
 // if (heading < -180) heading += 360; 
  return heading;
}

bool QMC5883L::getXY(float& fx, float& fy)
{
  int16_t x, y, z, t;
  if(!readRaw(&x,&y,&z,&t)) return false;

  if(x<xlow)  xlow = x;
  if(x>xhigh) xhigh = x;
  if(y<ylow)  ylow = y;
  if(y>yhigh) yhigh = y;
  if(xlow==xhigh || ylow==yhigh) return false;

  x -= (xhigh + xlow)/2;
  y -= (yhigh + ylow)/2;

  fx = (float)x / (xhigh - xlow);
  fy = (float)y / (yhigh - ylow);

  return true;
}

byte QMC5883L::getBearing(int heading360) {
    // heading360 = 0..360
    if (heading360 < 0) heading360 += 360;

    float sector = heading360 / 22.5;  // 360/16 = 22.5
    float frac = sector - (int)sector;

    byte result = (frac >= 0.5) ? ceil(sector) : floor(sector);
    if (result >= 16) result = 0;  // wrap around
    return result;
}

void QMC5883L::getDirection(char* myArray, int heading360) {
    byte d = getBearing(heading360);
    myArray[0] = _bearings[d][0];
    myArray[1] = _bearings[d][1];
    myArray[2] = _bearings[d][2];
}
// คืนค่า error ระหว่าง target กับ current heading (-180..+180)
int QMC5883L::getHeadingError(int targetHeading, int currentHeading) {
    int error = targetHeading - currentHeading;

    // ปรับให้อยู่ในช่วง -180 ถึง +180
    if (error > 180) error -= 360;
    if (error < -180) error += 360;

    return error;
}
