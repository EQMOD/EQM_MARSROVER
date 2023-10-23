#include <Wire.h>
#include <Servo.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <HardwareBLESerial.h>

HardwareBLESerial &bleSerial = HardwareBLESerial::getInstance();

#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0

#include "NRF52_MBED_TimerInterrupt.h"

#include "NRF52_MBED_ISR_Timer.h"

NRF52_MBED_Timer ITimer(NRF_TIMER_3);

// Init NRF52_MBED_ISRTimer
// Each NRF52_MBED_ISRTimer can service 16 different ISR-based timers
NRF52_MBED_ISRTimer ISR_Timer;

#define TIMER_INTERVAL_100mS             100L
#define HW_TIMER_INTERVAL_MS      1

void TimerHandler()
{
  ISR_Timer.run();
}


Adafruit_MPU6050 mpu;

Servo Motor11;

#define MPU_INT0 D2
#define AboveMid 1501
#define BelowMid 1499
#define AtMid   1500
#define AtMid1  1501
#define AtStart 1000
#define AtEnd 2000

#define DEFGAIN  90
#define PWMMULTIP 50
#define INGENDEPDELAY 1000    // Deployment Servo return delay in ms
#define INGENOFFSET 100
#define GYROSTABILIZEDELAY 20  // Routine delay for 2 secs (multiple of 100 ms)
#define DEFDURATION 1000

#define CoringNeutral 1625

//int x = 0 ;


float gr1,gr2,gr3,gr4,gr5;
float gy1,gy2,gy3,gy4,gy5;
float gx1,gx2,gx3,gx4,gx5;


int dropflag;
int revflag;

int ch1,ch2,ch3,ch4,ch5,ch6;
int TXch1,TXch2,TXch3,TXch4,TXch5,TXch6;

int timcount,gdefspeed,gdefgainX, gdefgainY,gdefgainZ,gpull_duration;
int ackstartL,ackstart2L,ackcurL,ackangleL,ackdurL,ackflgL,ackstartR,ackstart2R,ackcurR,ackangleR,ackdurR,ackflgR;
 
 


void setup() {
int i;
     Wire.begin();        // join i2c bus (address optional for master)
   Serial.begin(115200);  // start serial for output
   timcount = 0;
   pinMode(LED_BUILTIN,  OUTPUT);


  Motor11.attach(3);  // Coring Motor
  Motor11.writeMicroseconds(CoringNeutral);

  for(i=0;i<10;i++)
     readArduSlave();


   sendArduSlaveCMD(0x0a,1501,0); // Tx Direct Bypass
 //   sendArduSlaveCMD(0x0a,1500,0); // Tx Direct 
 //   sendArduSlaveCMD(0,1501,1);  // Raw Mode
   sendArduSlaveCMD(0,1500,1);  // Non Raw
   sendArduSlaveCMD(0x0f,1500,0);  /// Send a Arduino Slave commmand to push all TX data to the Master Arduino
   sendArduSlaveCMD(5,1500,1);



  dropflag = 0;
  revflag = 0;
  gdefspeed = 0;
  gdefgainX = DEFGAIN;
  gdefgainY = DEFGAIN;
  gdefgainZ = DEFGAIN;

  gpull_duration = DEFDURATION;

  ackstartL = 0;
  ackstart2L = 0;
  ackcurL =0;
  ackangleL = 0;
  ackdurL = 0;
  ackflgL = 0;

  ackstartR = 0;
  ackstart2R = 0;
  ackcurR =0;
  ackangleR = 0;
  ackdurR = 0;
  ackflgR = 0;
 
 
  sendArduSlaveCMD(3,AtMid,0);
  sendArduSlaveCMD(6,AtMid,0);
  

  // Try to initialize the MPU6050
  
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }



  delay(100);
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  delay(100);

  gr1=0;gr2=0;gr3=0;gr4=0;gr5=0;
  gy1=0;gy2=0;gy3=0;gy4=0;gy5=0;
  gx1=0;gx2=0;gx3=0;gx4=0;gx5=0;

  ch1 = 1500;ch2 = 1500;ch3 = 1000;ch4 = 1500;ch5 = 1500;ch6 = 1500;
  TXch1 = 1500;TXch2 = 1500;TXch3 = 1000;TXch4 = 1500;TXch5 = 0;TXch6 = 0;

  if (!bleSerial.beginAndSetupBLE("PERSE_MARS")) {
      Serial.println("failed to initialize HardwareBLESerial!");
      delay(1000);
    }
  
  ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 100, TimerHandler);
  ISR_Timer.setInterval(TIMER_INTERVAL_100mS, UpdateCounter);
} 

void loop() {

sensors_event_t a, g, temp;
int chan, chanval,val, i;
unsigned char c1, c2;
unsigned char c;


   val = readArduSlave();
   c2 = val >> 8;
   c1 = val & 0xff;
   chan = (c2 & 0xF0) >> 4;
   chanval = ((c2 & 0x07)<<8) + c1;


   switch(chan)
   {
     case 1 :     
                ch1 = chanval;
                sendArduSlaveCMD(1,ch1,0);
                break;
     case 2 : 
                ch2 = chanval;break;
     case 3 :
                ch3 = chanval;
                sendArduSlaveCMD(3,ch3,0);
                break;
     case 4 :
                ch4 = chanval;
                sendArduSlaveCMD(4,ch4,0);
                break;
     case 5 : 
                ch5 = chanval;
                TXch5 = ch5 - AtStart;
                break;
     case 6 :  
                ch6 = chanval;
                TXch6 = ch6 - AtStart;
                break;
     default:
                break;
    
   }


  if (TXch5 == 0)
  {
    sendArduSlaveCMD(2,ch2,0);

    dropflag = 0;
    revflag = 0;   
  }
  else
  {

     mpu.getEvent(&a, &g, &temp);

     gr1=gr2;gr2=gr3;gr3=gr4;gr4=gr5;
     gy1=gy2;gy2=gy3;gy3=gy4;gy4=gy5;

     gr5 = abs(g.gyro.z*800);
     gy5 = abs(g.gyro.y*400);

     if ((((gr1+gr2+gr3+gr4+gr5)/5) > TXch6) || (((gy1+gy2+gy3+gy4+gy5)/5) > TXch6))
     {
        dropflag = 1;
        sendArduSlaveCMD(2,AtMid,0);

        if (revflag == 0)
        {
            sendArduSlaveCMD(2,AtMid - TXch5,0);
            delay(1000);
            sendArduSlaveCMD(2,AtMid,0);
            revflag = 1;        
        }
        
     }

     if (dropflag == 0)
     {   
            sendArduSlaveCMD(2,AtMid + TXch5,0);
     }

  }

  bleSerial.poll();
  while (bleSerial.available() > 0) {
    c = bleSerial.read();

    switch(c)
    {

       case 'A' :    // Ackermann Turn P1 - L or R, P2P3 - Start delay, P4 - Speed, P5P6 - Duration 
                     if (bleSerial.available() > 0) ACKERMAN_START(bleSerial.read(),bleSerial.read() & 0xf,bleSerial.read() & 0xf,bleSerial.read() & 0xf,bleSerial.read() & 0xf,bleSerial.read() & 0xf );break;
       case 'F' :    // Move Forward P1P2 - Duration
                     if (bleSerial.available() > 0) MOT_FORWARD(bleSerial.read() & 0xf ,bleSerial.read() & 0xf);break;
       case 'B' :    // Move Backward P1P2 - Duration
                     if (bleSerial.available() > 0) MOT_BACKWARD(bleSerial.read() & 0xf,bleSerial.read() & 0xf);break;
       case 'L' :    // Turn-ON-SPOT Left P1P2 - Duration
                     if (bleSerial.available() > 0) MOT_LEFT(bleSerial.read() & 0xf,bleSerial.read() & 0xf);break;
       case 'R' :    // Turn-ON-SPOT Right P1P2 - Duration   
                     if (bleSerial.available() > 0) MOT_RIGHT(bleSerial.read() & 0xf,bleSerial.read() & 0xf);break;
       case 'S' :    // Set Default Motor Speed P1 - PWM * 50
                     if (bleSerial.available() > 0) MOT_SPEED(bleSerial.read() & 0xf);break;
       case 'I' :    // Ingenuity DEployment
                     if (bleSerial.available() > 0) INGENDEP();break;
       case 'M' :    // Global Gain Settings for MPU6050 P1P2P3 - Gain value (0 - 999)
                     if (bleSerial.available() > 0) SET_MAST(bleSerial.read() & 0xf, bleSerial.read() & 0xf, bleSerial.read() & 0xf);break;  
       case 'D' :    // Blocking Delay P1P2 - mulitples of 100ms
                     if (bleSerial.available() > 0) ROV_DELAY(bleSerial.read() & 0xf,bleSerial.read() & 0xf);break;     
       case 'P' :    // Terrain Anomaly Pull Back Duration  P1P2 - mulitples of 100ms
                     if (bleSerial.available() > 0) PULL_DURATION(bleSerial.read() & 0xf,bleSerial.read() & 0xf);break;                         
       case 'G' :    // Global Gain Settings for MPU6050 P1P2P3 - Gain value (0 - 999)
                     if (bleSerial.available() > 0) SET_GAIN(bleSerial.read() & 0xf, bleSerial.read() & 0xf, bleSerial.read() & 0xf);break;  
       case 'X' :    // MPU6050 X-Axis Gain setting P1P2P3 - Gain Value (0 - 999)
                     if (bleSerial.available() > 0) SET_GAINX(bleSerial.read() & 0xf, bleSerial.read() & 0xf, bleSerial.read() & 0xf);break; 
       case 'Y' :    // MPU6050 Y-Axis Gain setting P1P2P3 - Gain Value (0 - 999)
                     if (bleSerial.available() > 0) SET_GAINY(bleSerial.read() & 0xf, bleSerial.read() & 0xf, bleSerial.read() & 0xf);break;    
       case 'Z' :    // MPU6050 Z-Axis Gain setting P1P2P3 - Gain Value (0 - 999)
                     if (bleSerial.available() > 0) SET_GAINZ(bleSerial.read() & 0xf, bleSerial.read() & 0xf, bleSerial.read() & 0xf);break;    

       default : 
                  break;
    }
  }


  process_steering();   // Process the Ackerman execution based on the submitted parameters.


#ifdef DEBUG
   Serial.print(ch1);Serial.print(",");Serial.print(ch2);Serial.print(",");
   Serial.print(ch3);Serial.print(",");Serial.print(ch4);Serial.print(",");
   Serial.print(ch5);Serial.print(",");Serial.print(ch6);
   Serial.println("");
#endif

}


void edge_scanYZ(int startcnt)
{
sensors_event_t a, g, temp;


     if ((timcount - startcnt) < GYROSTABILIZEDELAY) return;  // Wait for the Rover to Stabilize before enabling Gyro Scanning

     mpu.getEvent(&a, &g, &temp);

     gr1=gr2;gr2=gr3;gr3=gr4;gr4=gr5;
     gy1=gy2;gy2=gy3;gy3=gy4;gy4=gy5;

     gr5 = abs(g.gyro.z*800);
     gy5 = abs(g.gyro.y*400);
  
     if ((((gr1+gr2+gr3+gr4+gr5)/5) > gdefgainZ) || (((gy1+gy2+gy3+gy4+gy5)/5) > gdefgainY))
     {

       if (((gr1+gr2+gr3+gr4+gr5)/5) > gdefgainZ)
             bleSerial.println("Terrain Anomaly(Z)");
       if (((gy1+gy2+gy3+gy4+gy5)/5) > gdefgainY)
             bleSerial.println("Terrain Anomaly(Y)");
             
        dropflag = 1;
        sendArduSlaveCMD(2,AtMid,0);

        sendArduSlaveCMD(2,AtMid - gdefspeed,0);
        gdefspeed = 0;
        delay(gpull_duration);
        sendArduSlaveCMD(2,AtMid,0);
     }

}



void edge_scanXY(int startcnt, int tspeed)
{
sensors_event_t a, g, temp;


     if ((timcount - startcnt) < GYROSTABILIZEDELAY) return;  // Wait for the Rover to Stabilize before enabling Gyro Scanning

     mpu.getEvent(&a, &g, &temp);

     gx1=gx2;gx2=gx3;gx3=gx4;gx4=gx5;
     gy1=gy2;gy2=gy3;gy3=gy4;gy4=gy5;

     gx5 = abs(g.gyro.x*400);
     gy5 = abs(g.gyro.y*400);
  
     if ((((gx1+gx2+gx3+gx4+gx5)/5) > gdefgainX) || (((gy1+gy2+gy3+gy4+gy5)/5) > gdefgainY))
     {

       if (((gx1+gx2+gx3+gx4+gx5)/5) > gdefgainX)
             bleSerial.println("Terrain Anomaly(X)");
       if (((gy1+gy2+gy3+gy4+gy5)/5) > gdefgainY)
             bleSerial.println("Terrain Anomaly(Y)");
             
        dropflag = 1;
        sendArduSlaveCMD(1,AtMid,0);

        sendArduSlaveCMD(1,tspeed,0);
        gdefspeed = 0;
        delay(gpull_duration);
        sendArduSlaveCMD(1,AtMid,0);
     }

}

void process_steering()
{
    if (ackcurL != 0)
    {
      if (ackstartL != 0)
      {
        if ((timcount - ackcurL) > ackstartL)
            ackstartL = 0;
      }
      else
      {
         if (ackflgL == 0)
            {
                sendArduSlaveCMD(4,AtStart,0);
                sendArduSlaveCMD(1,AtMid-ackangleL,0);
                ackflgL = 1;
            }
            else
            {
                if((timcount - (ackcurL+ackstart2L)) > ackdurL)
                {
                     sendArduSlaveCMD(1,AtMid,0);  
                     ackcurL = 0;
                     ackflgL = 0;
                }
              
            }
      }
    }

    if (ackcurR != 0)
    {
      if (ackstartR != 0)
      {
        if ((timcount - ackcurR) > ackstartR)
            ackstartR = 0;
      }
      else
      {
        if (ackflgR == 0)
            {
                sendArduSlaveCMD(4,AtStart,0);
                sendArduSlaveCMD(1,AtMid+ackangleR,0);
                ackflgR = 1;
            }
            else
            {
                if((timcount - (ackcurR+ackstart2R)) > ackdurR)
                {
                     sendArduSlaveCMD(1,AtMid,0);  
                     ackcurR = 0;
                     ackflgR = 0;
                }             
            }
      }
    }
  
}

void ACKERMAN_START(char d1, int d2, int   d3, int  d4 , int d5, int d6)
{
  
 
  
     if (d1 == 'L')
     {
           ackstartL = (d2*10)+d3;;
           ackstart2L = ackstartL;
           ackcurL = timcount;
           ackangleL = d4 * PWMMULTIP;
           ackdurL = (d5*10)+d6;
           ackflgL = 0;
     }

     if (d1 == 'R')
     {
           ackstartR = (d2*10)+d3;
           ackstart2R = ackstartR;
           ackcurR = timcount;
           ackangleR = d4 * PWMMULTIP;
           ackdurR = (d5*10)+d6;
           ackflgR = 0;
         
     }

}

void PULL_DURATION(int d1, int d2)
{
    gpull_duration = (d1*1000) + (d2*100);

}


void ROV_DELAY(int d1, int d2)
{
  int startcnt;
  int rdelay;

  rdelay = (d1*10) + d2;
  startcnt = timcount;
  do {
      delay(10);
  }
  while(timcount <= (startcnt + rdelay));
}

void MOT_SPEED(int slevel)
{
      if((slevel >= 0) && (slevel <=9))
          gdefspeed = slevel * PWMMULTIP;
}

void SET_GAIN(int d1, int d2, int d3)
{
     gdefgainZ = (d1 * 100) + (d2 * 10) + d3;
     gdefgainX = gdefgainZ;
     gdefgainY = gdefgainZ;
       
}
void SET_GAINZ(int d1, int d2, int d3)
{
     gdefgainZ = (d1 * 100) + (d2 * 10) + d3;  
}
void SET_GAINY(int d1, int d2, int d3)
{
     gdefgainY = (d1 * 100) + (d2 * 10) + d3;
}
void SET_GAINX(int d1, int d2, int d3)
{
     gdefgainX = (d1 * 100) + (d2 * 10) + d3;
}

void SET_MAST(int d1, int d2, int d3)
{
     sendArduSlaveCMD(6,AtStart+(d1 * 100) + (d2 * 10) + d3,0); 
}


void INGENDEP()
{

  int startcnt;

  sendArduSlaveCMD(3,AtMid,0);
  sendArduSlaveCMD(3,AtMid+INGENOFFSET,0);
  delay(INGENDEPDELAY);
  sendArduSlaveCMD(3,AtMid,0);
  
}

void MOT_LEFT(int d1, int d2)
{

  int startcnt;
  int duratio;

  duratio = (d1*10) + d2;

  startcnt = timcount;
  sendArduSlaveCMD(4,AtEnd,0);
  sendArduSlaveCMD(1,AtMid-gdefspeed,0);
  gx1=0;gx2=0;gx3=0;gx4=0;gx5=0;
  gy1=0;gy2=0;gy3=0;gy4=0;gy5=0; 
  do{
         edge_scanXY(startcnt,AtMid+gdefspeed);
    }
  while(timcount <= (startcnt + duratio));
  sendArduSlaveCMD(1,AtMid,0);
}


void MOT_RIGHT(int d1, int d2)
{
  int startcnt,duratio;

  duratio = (d1*10) + d2;
  startcnt = timcount;
  sendArduSlaveCMD(4,AtEnd,0);
  sendArduSlaveCMD(1,AtMid+gdefspeed,0);
  gx1=0;gx2=0;gx3=0;gx4=0;gx5=0;
  gy1=0;gy2=0;gy3=0;gy4=0;gy5=0;
  do{
        edge_scanXY(startcnt,AtMid-gdefspeed);
    }
  while(timcount <= (startcnt + duratio));
  sendArduSlaveCMD(1,AtMid,0);
}


void MOT_FORWARD(int d1, int d2)
{

  int startcnt,duratio;

  duratio = (d1*10) + d2;

  startcnt = timcount;
  gr1=0;gr2=0;gr3=0;gr4=0;gr5=0;
  gy1=0;gy2=0;gy3=0;gy4=0;gy5=0;
  sendArduSlaveCMD(2,AtMid + gdefspeed,0);
  do{
        edge_scanYZ(startcnt);
        process_steering();
    }
  while(timcount <= (startcnt + duratio));
  sendArduSlaveCMD(2,AtMid ,0);
}

void MOT_BACKWARD(int d1, int d2)
{
  int startcnt,duratio;

  duratio = (d1*10) + d2;

  startcnt = timcount;
  gr1=0;gr2=0;gr3=0;gr4=0;gr5=0;
  gy1=0;gy2=0;gy3=0;gy4=0;gy5=0;
  sendArduSlaveCMD(2,AtMid - gdefspeed,0);
  do{
        edge_scanYZ(startcnt);
        process_steering();
    }
  while(timcount <= (startcnt + duratio));
  sendArduSlaveCMD(2,AtMid ,0);
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
unsigned char c1, c2;
int val;


  Wire.requestFrom(4, 2);    // request 2 bytes from slave device #8

  while(1 < Wire.available()) // loop through all but the last
     c1 = Wire.read(); // receive byte as a character
  while(0 < Wire.available())
     c2 = Wire.read();    // receive byte as an integer
  val = c1 +  (c2 << 8) ;
  return(val);

}

void UpdateCounter()
{
  timcount++;
}
