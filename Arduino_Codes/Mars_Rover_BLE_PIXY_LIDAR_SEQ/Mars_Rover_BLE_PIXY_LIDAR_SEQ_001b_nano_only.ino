#include <Wire.h>
#include <Servo.h>
//#include <phyphoxBle.h> 
//#include <Arduino_LSM9DS1.h>
//#include <Arduino_LPS22HB.h>
//#include <Arduino_HTS221.h>
//#include <Arduino_APDS9960.h>


#define POS1  1000   // CH4 in Ackerman mode
#define POS2  1500   // CH4 in Rotate on Spot Mode
#define POS3  2000   // CH4 in Ackerman and On Spot Mode


#define X_CENTER    160L
#define Y_CENTER    100L
#define X_MAX_DIFF  50
#define RCS_MIN_POS     1000
#define RCS_MAX_POS     2000
#define RCS_CENTER_POS  ((RCS_MAX_POS-RCS_MIN_POS)/2)

// Define the BLOB min and maximum area (width x height) for a motor stop command

#define MIN_AREA    4800
#define MAX_AREA    6000

#define COLLISION_MM 120 // Collision distance in milimeters

// Uncomment one of these to enable another type of serial interface
#define I2C

#include <Pixy2I2C.h>
Pixy2I2C pixy;

#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();  
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();  

#define CoringNeutral 1625

int lastch2;
int count;
int botspeed = 0;

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

  lox1.begin(0x2A); //RSM MAST
  delay(100);
  sendArduSlaveCMD(0x0b,1500,0);
  delay(100);
  lox2.begin(0x2B);  //RIGHT FRONT WHEEL LIDAR
  delay(100);
  sendArduSlaveCMD(0x0d,1500,0);
  delay(100);
  lox3.begin(0x2C); // LEFT FRONT WHEEL LIDAR
   delay(100);
  sendArduSlaveCMD(0x0c,1500,0);
  delay(100);
  lox4.begin(0x29); // REAR LEFT WHEEL LADAR

  pixy.init();


 sendArduSlaveCMD(0x0f,1500,0);  /// Send a Arduino Slave commmand to push all TX data to the Master Arduino
}

void loop() {

unsigned char c1, c2;
int chan, chanval,val, i;

  int j;
  char buf[32]; 
  int32_t panError, tiltError; 
  long int blockarea;

  VL53L0X_RangingMeasurementData_t measure1,measure2,measure3,measure4;

  byte error, address;
  int nDevices;

     val = readArduSlave();
     c2 = val >> 8;
     c1 = val & 0xff;
     

     chan = (c2 & 0xF0) >> 4;
     chanval = ((c2 & 0x07)<<8) + c1;


     //Serial.print("ch ");Serial.print(chan); Serial.print(" ");Serial.println(chanval);
     
     if (chan == 5)
     {
          if (chanval >= 1500)
            botspeed = chanval-1500;
          else
            botspeed = 0;
     }
     else
     {
        sendArduSlaveCMD(chan,chanval,0);
        if (chan == 2)
        {
            lastch2 = chanval;
        }
      }

  lox2.rangingTest(&measure2, false);
  lox3.rangingTest(&measure3, false);


   Serial.print("lox2 ");Serial.print(measure2.RangeMilliMeter); Serial.print("lox3 ");Serial.println(measure3.RangeMilliMeter);

  if (botspeed > 10 ) 
  {
  // grab blocks!
  pixy.ccc.getBlocks();
  

  if (pixy.ccc.numBlocks)
  {

    panError = X_CENTER-pixy.ccc.blocks[0].m_x;
    blockarea = pixy.ccc.blocks[0].m_width * pixy.ccc.blocks[0].m_height;
   if (abs(panError) > X_MAX_DIFF)
        {
           if (panError > 0)
                motor_bankleft();
           else
                 motor_bankright();
        
        }
        else
   {
    
    if ((blockarea >= MIN_AREA) && (blockarea <= MAX_AREA))
    {
          motor_stop();
    }
    else
    {
      int xpos = pixy.ccc.blocks[0].m_x;
      //sprintf(buf, "xpos %d:\n", xpos  );
      //Serial.print(buf);
    
      if (blockarea > MAX_AREA)
      {   
          motor_reverse();
      }
      else
      {
       if (blockarea < MIN_AREA)
       {
          motor_forward();
       }
       else
         motor_stop();
     
      }   
      
    } 
  }

  } // Num Blocks


}


  // Collision Avoidance starts here;

  if ((measure2.RangeMilliMeter < COLLISION_MM) && (measure3.RangeMilliMeter < COLLISION_MM))
    {
   //   motor_stop();
    }

  else
   if  (blockarea < (MAX_AREA-500))
    {
    
    if (measure2.RangeMilliMeter < COLLISION_MM)
   {
        while(measure2.RangeMilliMeter < COLLISION_MM)
        {
            //  motor_turnrightreverse();
            //  delay(2200);
              motor_turnleftforward();
              delay(3200);
              motor_turnrightforward();
              delay(1800);
              pixy.ccc.getBlocks();
              
              while (pixy.ccc.numBlocks == 0) // re acquire target
              {
                 delay(100);
                 pixy.ccc.getBlocks();                
              }
              lox2.rangingTest(&measure2, false);
        }
        motor_forward();
    
   }

   if (measure3.RangeMilliMeter < COLLISION_MM)
   {

        while(measure3.RangeMilliMeter < COLLISION_MM)
        {
             // motor_turnleftreverse();
             // delay(2200);
              motor_turnrightforward();
              delay(3200);
              motor_turnleftforward();
              delay(1800);
                
              pixy.ccc.getBlocks();
              while (pixy.ccc.numBlocks == 0)  // re acquire target
              {
                 delay(100);
                 pixy.ccc.getBlocks();                
              }

              lox3.rangingTest(&measure3, false);
        }
        motor_forward();
   }
  }

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
      sendArduSlaveCMD(4,POS1,0);
      sendArduSlaveCMD(1,1500,0);
      sendArduSlaveCMD(2,1500+botspeed,0);

}

void motor_reverse()
{
  Serial.println("motor_reverse");
      sendArduSlaveCMD(4,POS1,0);
      sendArduSlaveCMD(1,1500,0);
      sendArduSlaveCMD(2,1500-botspeed,0);

}

void motor_bankleft()
{

  Serial.println("motor_bankleft");
      sendArduSlaveCMD(4,POS2,0);
      sendArduSlaveCMD(1,1999,0);
      sendArduSlaveCMD(2,1500+botspeed,0);

}

void motor_bankright()
{
Serial.println("motor_bankright");
      sendArduSlaveCMD(4,POS2,0);
      sendArduSlaveCMD(1,1999,0);
      sendArduSlaveCMD(2,1500-botspeed,0);

}

void motor_turnleftforward()
{
      sendArduSlaveCMD(4,POS1,0);
      sendArduSlaveCMD(1,101  ,0);
      sendArduSlaveCMD(2,1500+botspeed,0);

}

void motor_turnrightforward()
{
      sendArduSlaveCMD(4,POS1,0);
      sendArduSlaveCMD(1,1990,0);
      sendArduSlaveCMD(2,1500+botspeed,0);

}

void motor_turnleftreverse()
{
      sendArduSlaveCMD(4,POS1,0);
      sendArduSlaveCMD(1,1100,0);
      sendArduSlaveCMD(2,1500-botspeed,0);

}



void motor_turnrightreverse()
{
      sendArduSlaveCMD(4,POS1,0);
      sendArduSlaveCMD(1,1900,0);
      sendArduSlaveCMD(2,1500-botspeed,0);

} 
