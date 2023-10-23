#include <Wire.h>
#include <Servo.h>
//#include <phyphoxBle.h> 
//#include <Arduino_LSM9DS1.h>
//#include <Arduino_LPS22HB.h>
//#include <Arduino_HTS221.h>
//#include <Arduino_APDS9960.h>


#define X_CENTER    160L
#define Y_CENTER    100L
#define X_MAX_DIFF  50
#define RCS_MIN_POS     1000
#define RCS_MAX_POS     2000
#define RCS_CENTER_POS  ((RCS_MAX_POS-RCS_MIN_POS)/2)

// Define the BLOB min and maximum area (width x height) for a motor stop command

#define MIN_AREA    4800
#define MAX_AREA    6000

// Uncomment one of these to enable another type of serial interface
#define I2C

#include <Pixy2I2C.h>
Pixy2I2C pixy;

#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();  

#define CoringNeutral 1625

int lastch2,lastch4;
int count;

int margin = 50;
int jitter_for = 0;
int jitter_aft = 0;

Servo Motor11;

int x = 0 ;

void setup() {
int i;
   Wire.begin();        // join i2c bus (address optional for master)
   Serial.begin(115200);  // start serial for output

   Motor11.attach(3);  // Coring Motor
   Motor11.writeMicroseconds(CoringNeutral);

   // Enable TX Bypass

  for(i=0;i<10;i++)
     readArduSlave();

    sendArduSlaveCMD(0x0a,1501,0); // Tx Direct Bypass
 //   sendArduSlaveCMD(0x0a,1500,0); // Tx Direct 
//   sendArduSlaveCMD(0,1501,1);  // Raw Mode
     sendArduSlaveCMD(0,1500,1);  // Non Raw

  lox1.begin(0x2A);   // RSM Mast
  delay(100);
  sendArduSlaveCMD(0x0c,1500,0);
  delay(100);
  lox2.begin(0x2B);   //Front Left Wheel
  delay(100);
  sendArduSlaveCMD(0x0b,1500,0);
  delay(100);
  lox3.begin(0x29);  // Left Left Wheel

//  pixy.init();
  sendArduSlaveCMD(4,1500,0);
  lastch2 = 0;
  lastch4 = 0;
}

void loop() {

unsigned char c1, c2;
int chan, chanval,val, i;

  int j;
  char buf[32]; 
  int32_t panError, tiltError; 
  long int blockarea;

  VL53L0X_RangingMeasurementData_t measure1,measure2,measure3;

  byte error, address;
  int nDevices;

     val = readArduSlave();
     c2 = val >> 8;
     c1 = val & 0xff;
     

     chan = (c2 & 0xF0) >> 4;
     chanval = ((c2 & 0x07)<<8) + c1;
    
     if(chan == 2) lastch2 = chanval;
     if(chan == 4) lastch4 = chanval;

  if ((lastch2 <= 1520) && (lastch2 >= 1480)&& (lastch4 == 1500))
  {

     j=1;
     for(i=1500;i>1010;i--)
     {

      sendArduSlaveCMD(1,i,0);
     delay(500);
     lox2.rangingTest(&measure2, false); 
     delay(500);
     Serial.print(j);Serial.print(",");Serial.print(measure1.RangeMilliMeter); Serial.println(" ");
     delay(500)
  
  }
    lsstch4 = 0;
  }
*/

     
/*
 * 
 * 

    lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
    lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
    lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!


    Serial.print("lox1 ");Serial.print(measure1.RangeMilliMeter); Serial.print(" ");
    Serial.print("lox2 ");Serial.print(measure2.RangeMilliMeter); Serial.print(" ");
    Serial.print("lox3 ");Serial.print(measure3.RangeMilliMeter); Serial.println(" ");
    
  */  

}

void sendArduSlaveCMD(int chan,int chanval,int changrp)
{
unsigned char c1, c2;

    c1 = chanval & 0xff;
    c2 = ((chanval & 0x700) >> 8) + ((chan & 0xf) << 4) + ((changrp & 1) << 3);
    Wire.beginTransmission(4); // transmit to device #4
    Wire.write(c1);
    Wire.write(c2);
    Wire.endTransmission();
}

int readArduSlave()
{
int c1, c2;
int val;
  
  Wire.requestFrom(4, 2);    // request 2 bytes from slave device #8

  while(1 < Wire.available()) // loop through all but the last
     c1 = Wire.read(); // receive byte as a character
  while(0 < Wire.available())
     c2 = Wire.read();    // receive byte as an integer

  val = c1 +  (c2 << 8) ;
  return(val);

}

void motor_stop()
{

  Serial.println("motor_stop");
      sendArduSlaveCMD(4,1500,0);
      sendArduSlaveCMD(1,1500,0);
      sendArduSlaveCMD(2,1500,0);


}

void motor_forward()
{

  Serial.println("motor_forward");
      sendArduSlaveCMD(4,1500,0);
      sendArduSlaveCMD(1,1500,0);
      sendArduSlaveCMD(2,1900,0);

}

void motor_reverse()
{
  Serial.println("motor_reverse");
      sendArduSlaveCMD(4,1500,0);
      sendArduSlaveCMD(1,1500,0);
      sendArduSlaveCMD(2,1100,0);

}

void motor_bankleft()
{

  Serial.println("motor_bankleft");
      sendArduSlaveCMD(4,1500,0);
      sendArduSlaveCMD(1,1999,0);
      sendArduSlaveCMD(2,1800,0);

}

void motor_bankright()
{
Serial.println("motor_bankright");
      sendArduSlaveCMD(4,1500,0);
      sendArduSlaveCMD(1,1999,0);
      sendArduSlaveCMD(2,1100,0);

}

void motor_turnleftforward()
{

}

void motor_turnleftreverse()
{

}

void motor_turnrightforward()
{

}

void motor_turnrightreverse()
{

} 
