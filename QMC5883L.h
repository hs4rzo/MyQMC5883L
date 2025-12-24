#ifndef QMC5883L_H
#define QMC5883L_H

class QMC5883L {
public:
  bool startInit();
  byte getBearing(int heading360);
  void init();
  void reset();
  int  ready();
  void reconfig();  
  int readHeading();
  int readHeading360();
  int readHeading180();
  void getDirection(char* myArray, int heading360);
  bool getXY(float& fx, float& fy);
  int readRaw( int16_t *x, int16_t *y, int16_t *z, int16_t *t );
  int readRaw_nonblock( int16_t *x, int16_t *y, int16_t *z, int16_t *t );
  void resetCalibration();
  void setSamplingRate( int rate );
  void setRange( int range );
  void setOversampling( int ovl );
  int getHeadingError(int targetHeading, int currentHeading);

private:
  int16_t xhigh, xlow;
  int16_t yhigh, ylow;
  uint8_t addr;
  uint8_t mode;
  uint8_t rate;
  uint8_t range;
  uint8_t oversampling;
  const char _bearings[16][3] =  {
	  {' ', ' ', 'N'},
	  {'N', 'N', 'E'},
	  {' ', 'N', 'E'},
	  {'E', 'N', 'E'},
	  {' ', ' ', 'E'},
	  {'E', 'S', 'E'},
	  {' ', 'S', 'E'},
	  {'S', 'S', 'E'},
	  {' ', ' ', 'S'},
	  {'S', 'S', 'W'},
	  {' ', 'S', 'W'},
	  {'W', 'S', 'W'},
	  {' ', ' ', 'W'},
	  {'W', 'N', 'W'},
	  {' ', 'N', 'W'},
	  {'N', 'N', 'W'},
  };

};

#endif
