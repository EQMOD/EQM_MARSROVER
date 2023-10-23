
//---------------------------------------------------------------------
// Copyright © 2020 Raymund Sarmiento
//
// Permission is hereby granted to use this Software for any purpose
// including combining with commercial products, creating derivative
// works, and redistribution of source or binary code, without
// limitation or consideration. Any redistributed copies of this
// Software must include the above Copyright Notice.
//
// THIS SOFTWARE IS PROVIDED "AS IS". THE AUTHOR OF THIS CODE MAKES NO
// WARRANTIES REGARDING THIS SOFTWARE, EXPRESS OR IMPLIED, AS TO ITS
// SUITABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
//
//  DISCLAIMER:

//  You can use the information on this site COMPLETELY AT YOUR OWN RISK.
//  The modification steps and other information on this site is provided
//  to you "AS IS" and WITHOUT WARRANTY OF ANY KIND, express, statutory,
//  implied or otherwise, including without limitation any warranty of
//  merchantability or fitness for any particular or intended purpose.
//  In no event the author will  be liable for any direct, indirect,
//  punitive, special, incidental or consequential damages or loss of any
//  kind whether or not the author  has been advised of the possibility
//  of such loss.
//---------------------------------------------------------------------

#include <Servo.h>
#include <ServoEasing.h>
#include <IBusBM.h>
#include <Wire.h>


ServoEasing Motor1;
ServoEasing Motor2;
ServoEasing Motor3;
ServoEasing Motor4;

ServoEasing Motor5;
ServoEasing Motor6;
ServoEasing Motor7;
ServoEasing Motor8;

ServoEasing Motor9;
ServoEasing Motor10;
ServoEasing ServoING;
ServoEasing ServoRSM;


Servo Motor11;




#define SRVFLOFST 100
#define SRVRLOFST 108
#define SRVFROFST 90
#define SRVRROFST 90

#define CoringNeutral 1593

#define AboveMid 1501
#define BelowMid 1499
#define AtMid   1500
#define AtMid1  1501

#define servomid 90

#define ppmrange 500
#define EPAHIGH  2000
#define EPALOW   1000

#define EPANEARHIGH 1800
#define EPANEARLOW 1300


#define TURNRADHIGH 500 
#define TURNRADLOW  120

#define POS1  1000
#define POS2  1500
#define POS3  2000

#define TOACTIVE 150 
#define SPFACTOR 4

IBusBM IBus;

int ch1, ch2, ch3, ch4, ch5, ch6,TXch1, TXch2, TXch3, TXch4, TXch5, TXch6, Pch1, Pch2, Pch3, Pch4, Pch5, Pch6 = 0;
int TXBypass = AtMid;
int motorspeed = 0; // rover speed
int steerradius = 0; // turning radius
int RawMode = AtMid;

int rearactive = 0;
int toactive_cnt = 0;
int sfactor = 0;

float spd1, spd2, spd3, spd1ppm, spd2ppm, spd3ppm, OuterFront, OuterBack, InnerFront, InnerBack = 0;


float CentertoCorner = 95;  
float CentertoRear = 98;
float CentertoFront = 100;
float CentertoMid = 100;


void setup() {

  sfactor = SPFACTOR;

  Serial.begin(115200);
  IBus.begin(Serial, IBUSBM_NOTIMER);    // iBUS object connected to serial0 RX pin

  pinMode(A0, INPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);

  Wire.begin(4);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  
  // Wheel Motors
  
  Motor1.attach(2);  // Left Front Motor
  Motor2.attach(3);  // Lef Mid Motor
  Motor3.attach(4);  // Right Front Motor
  Motor4.attach(5);  // Right Mid Motor


  Motor9.attach(13); // Left Rear Motor
  Motor10.attach(10); // Rigth Rear Motor

  
  
  // Steering Motors 
  
  Motor5.attach(6);  // Front Left Steer Motor
  Motor6.attach(7);  // Rear Left Steer Motor
  Motor7.attach(8);  // Front Right Steer Motor
  Motor8.attach(9);  // Front Rear Steer Motor

  
 
  // Mast Servo 
  
  ServoRSM.attach(11);

   // Ingenuity Servo 
  
  ServoING.attach(12);


//Coring Drill

// Motor11.attach(A0);

  Motor1.writeMicroseconds(AtMid);
  Motor2.writeMicroseconds(AtMid);
  Motor3.writeMicroseconds(AtMid);
  Motor4.writeMicroseconds(AtMid);
  Motor9.writeMicroseconds(AtMid);
  Motor10.writeMicroseconds(AtMid);

  
  Motor5.write(SRVFLOFST);
  Motor6.write(SRVRLOFST);
  Motor7.write(SRVFROFST);
  Motor8.write(SRVRROFST);
  
  ServoRSM.write(servomid);
  ServoING.write(servomid);

 // Motor11.write(CoringNeutral);
 

  ch1 = AtMid;
  ch2 = AtMid;
  ch3 = AtMid;
  ch4 = POS1;
  ch5 = CoringNeutral;
  ch6 = AtMid;

  Pch1 = AtMid;
  Pch2 = AtMid;
  Pch3 = AtMid;
  Pch4 = POS1;
  Pch5 = CoringNeutral;
  Pch6 = AtMid;

  TXBypass = AtMid1;
  RawMode = AtMid;


}

void loop() {
  
  // read the TX data from the Ibus
  IBus.loop();
  
  TXch1 = IBus.readChannel(0);
  TXch2 = IBus.readChannel(1);
  TXch3 = IBus.readChannel(2);
  TXch4 = IBus.readChannel(3);
  TXch5 = IBus.readChannel(4);
  TXch6 = IBus.readChannel(5); 

  if (TXBypass == AtMid)
  {
      ch1 = TXch1;
      ch2 = TXch2;
      ch3 = TXch3;
      ch4 = TXch4;
      ch5 = TXch5;
      ch6 = TXch6;
  }


// Serial.print("ch1 ");Serial.print(ch1); Serial.print(" ch2 ");Serial.print(ch2);Serial.print(" ch6 ");Serial.print(ch6);Serial.print(" TxBypass ");Serial.println(TXBypass);


if (RawMode == AtMid)
{ 
if ((ch3 < AtMid) && (ch6 > EPAHIGH))
 rearactive = 1;
 else
  if (ch2 == AtMid)
  {
      toactive_cnt = TOACTIVE;
      rearactive = 0;
  }
  else
  {
      toactive_cnt--;
      if (toactive_cnt <= 0)
      {
        toactive_cnt = 0;
        rearactive = 1;
      }
  }

  // Right
  if (ch1 > AboveMid ) 
    steerradius = map(ch1, AboveMid, EPAHIGH, TURNRADHIGH, TURNRADLOW); 
  // left
  else if (ch1 < BelowMid) 
    steerradius = map(ch1, BelowMid, EPALOW, TURNRADHIGH, TURNRADLOW); 

  
  
  if (ch2 > AtMid)
    motorspeed = map(ch2, AtMid, EPAHIGH, 0, 100); // speed from 0% to 100%
  else if (ch2 < AtMid)
    motorspeed = map(ch2, AtMid, EPALOW, 0, 100); // speed from 0% to 100%
   else
      motorspeed = 0;
  
  GetMotorsSpeed();
  GetMotorAngle();

  // update ch3 data on Ingenuity Deploy Servo
  ServoING.writeMicroseconds(ch3); 

  // update ch6 data on RSM Mast Servo
  ServoRSM.writeMicroseconds(ch6);

  // update Cording drill Motor data 
  //if (ch5 != 0)
    //  Motor11.writeMicroseconds(ch5);
  //else
    //  Motor11.writeMicroseconds(CoringNeutral);  // Make sure Drill Motor is not active in the absence of an RX

  switch (ch4)
  
  {
    
    case POS1:
  
          // right
          if (ch1 > AboveMid) {
          // Corner Steering Motors
          Motor7.write(SRVFROFST + InnerFront); 
          Motor8.write(SRVRROFST - InnerBack); 
          Motor5.write(SRVFLOFST + OuterFront); 
          Motor6.write(SRVRLOFST - OuterBack); 

          // Set speed of all wheel motors
          if (ch2 > AtMid) // Forward
          { 
            Motor1.writeMicroseconds(AtMid + spd1ppm);
            Motor2.writeMicroseconds(AtMid + spd1ppm);
            Motor3.writeMicroseconds(AtMid + spd2ppm);
            Motor4.writeMicroseconds(AtMid + spd3ppm);

            if (rearactive == 1)
            {
                Motor9.writeMicroseconds(AtMid + spd1ppm);
                Motor10.writeMicroseconds(AtMid + spd2ppm);
            }
            else
            {
                Motor9.writeMicroseconds(AtMid + (spd1ppm/sfactor));
                Motor10.writeMicroseconds(AtMid + (spd2ppm/sfactor));
            }
            
          }
          else if (ch2 < AtMid) // Reverse
          {
            Motor1.writeMicroseconds(AtMid - spd1ppm);
            Motor2.writeMicroseconds(AtMid - spd1ppm);
            Motor3.writeMicroseconds(AtMid - spd2ppm);
            Motor4.writeMicroseconds(AtMid - spd3ppm);

            if (rearactive == 1)
            {
                Motor9.writeMicroseconds(AtMid - spd1ppm);
                Motor10.writeMicroseconds(AtMid - spd2ppm);
            }            
            else
            {
                Motor9.writeMicroseconds(AtMid - (spd1ppm/sfactor));
                Motor10.writeMicroseconds(AtMid - (spd2ppm/sfactor));
            }
            
          }
          else  // At Rest
          {
            Motor1.writeMicroseconds(ch2);
            Motor2.writeMicroseconds(ch2);
            Motor3.writeMicroseconds(ch2);
            Motor4.writeMicroseconds(ch2);

            if (rearactive == 1)
            {
                Motor9.writeMicroseconds(ch2);
                Motor10.writeMicroseconds(ch2);
            }
            else
            {

               if (ch2 > AtMid)
               {
                   Motor9.writeMicroseconds(AtMid+((ch2-AtMid)/sfactor));
                   Motor10.writeMicroseconds(AtMid+((ch2-AtMid)/sfactor));
               }
               else
               {
                   Motor9.writeMicroseconds(AtMid-((AtMid-ch2)/sfactor));
                   Motor10.writeMicroseconds(AtMid-((AtMid-ch2)/sfactor));
               }
               
            }

          }

          }

          // left
          else if (ch1 < BelowMid) {
          // Corner Steering motors
          Motor7.write(SRVFROFST - OuterFront);
          Motor8.write(SRVRROFST + OuterBack);
          Motor5.write(SRVFLOFST - InnerFront);
          Motor6.write(SRVRLOFST + InnerBack);

          if (ch2 > AtMid) // Forward
          { 
            Motor1.writeMicroseconds(AtMid + spd2ppm);
            Motor2.writeMicroseconds(AtMid + spd3ppm);
            Motor3.writeMicroseconds(AtMid + spd1ppm);
            Motor4.writeMicroseconds(AtMid + spd1ppm);

            if (rearactive == 1)
            {
                Motor9.writeMicroseconds(AtMid + spd2ppm);
                Motor10.writeMicroseconds(AtMid + spd1ppm);
            }          
            else
            {
                Motor9.writeMicroseconds(AtMid + (spd2ppm/sfactor));
                Motor10.writeMicroseconds(AtMid + (spd1ppm/sfactor));
            }
          
          }
          else if (ch2 < AtMid) // Reverse
          { 
            Motor1.writeMicroseconds(AtMid - spd2ppm);
            Motor2.writeMicroseconds(AtMid - spd3ppm);
            Motor3.writeMicroseconds(AtMid - spd1ppm);
            Motor4.writeMicroseconds(AtMid - spd1ppm);

            if (rearactive == 1)
            {
                Motor9.writeMicroseconds(AtMid - spd2ppm);
                Motor10.writeMicroseconds(AtMid - spd1ppm);
            }
            else
            {
                Motor9.writeMicroseconds(AtMid - (spd2ppm/sfactor));
                Motor10.writeMicroseconds(AtMid - (spd1ppm/sfactor));
            }
          
          }
          else
          {
            Motor1.writeMicroseconds(ch2);
            Motor2.writeMicroseconds(ch2);
            Motor3.writeMicroseconds(ch2);
            Motor4.writeMicroseconds(ch2);

            if (rearactive == 1)
            {
                Motor9.writeMicroseconds(ch2);
                Motor10.writeMicroseconds(ch2);
            }
            else
            {
               if (ch2 > AtMid)
               {
                   Motor9.writeMicroseconds(AtMid+((ch2-AtMid)/sfactor));
                   Motor10.writeMicroseconds(AtMid+((ch2-AtMid)/sfactor));
               }
               else
               {
                   Motor9.writeMicroseconds(AtMid-((AtMid-ch2)/sfactor));
                   Motor10.writeMicroseconds(AtMid-((AtMid-ch2)/sfactor));
               }
            }
          
          }

          }

          else {

            // Straight driving (throttle only)
  
            Motor1.writeMicroseconds(ch2);
            Motor2.writeMicroseconds(ch2);
            Motor3.writeMicroseconds(ch2);
            Motor4.writeMicroseconds(ch2);

            if (rearactive == 1)
            {
                Motor9.writeMicroseconds(ch2);
                Motor10.writeMicroseconds(ch2);
            }
            else
            {
               if (ch2 > AtMid)
               {
                   Motor9.writeMicroseconds(AtMid+((ch2-AtMid)/sfactor));
                   Motor10.writeMicroseconds(AtMid+((ch2-AtMid)/sfactor));
               }
               else
               {
                   Motor9.writeMicroseconds(AtMid-((AtMid-ch2)/sfactor));
                   Motor10.writeMicroseconds(AtMid-((AtMid-ch2)/sfactor));
               }
            }
  
            // Corner Steering Motors in straight position
  
            Motor5.write(SRVFLOFST);
            Motor6.write(SRVRLOFST);
            Motor7.write(SRVFROFST);
            Motor8.write(SRVRROFST);

            }

          
          
          break;
    
    case POS2:    


          // Corner Steering Motors
          if  ((ch1 > AboveMid) ||  (ch1 < BelowMid))
          {
            Motor7.write(SRVFROFST - (InnerFront/2)); // front wheel steer right
            Motor8.write(SRVRROFST + (InnerBack/2)); // back wheel steer left for overall steering to the right of the rover
            Motor5.write(SRVFLOFST + (InnerFront/2)); // front wheel steer right
            Motor6.write(SRVRLOFST - (InnerBack/2)); // back wheel steer left for overall steering to the right of the rover
          }
          else
          {
            Motor5.write(SRVFLOFST);
            Motor6.write(SRVRLOFST);
            Motor7.write(SRVFROFST);
            Motor8.write(SRVRROFST);

          }


          if (ch1 > EPANEARHIGH)
          {
            // Set speed of all wheel motors
            if (ch2 > AtMid) // Forward
            { 
              Motor1.writeMicroseconds(AtMid + spd1ppm);
              Motor2.writeMicroseconds(AtMid + spd1ppm);
              Motor3.writeMicroseconds(AtMid - spd1ppm);
              Motor4.writeMicroseconds(AtMid - spd1ppm);

              Motor9.writeMicroseconds(AtMid + spd1ppm);
              Motor10.writeMicroseconds(AtMid - spd1ppm);
            
            
            }
            else if (ch2 < AtMid) // Reverse
            {
              Motor1.writeMicroseconds(AtMid - spd1ppm);
              Motor2.writeMicroseconds(AtMid - spd1ppm);
              Motor3.writeMicroseconds(AtMid + spd1ppm);
              Motor4.writeMicroseconds(AtMid + spd1ppm);

              Motor9.writeMicroseconds(AtMid - spd1ppm);
              Motor10.writeMicroseconds(AtMid + spd1ppm);
            
            
            }
            else  // At Rest
            {
              Motor1.writeMicroseconds(ch2);
              Motor2.writeMicroseconds(ch2);
              Motor3.writeMicroseconds(ch2);
              Motor4.writeMicroseconds(ch2);

              Motor9.writeMicroseconds(ch2);
              Motor10.writeMicroseconds(ch2);
            
            }
          }
          else
          if (ch1 < EPANEARLOW)
          {
            // Set speed of all wheel motors
            if (ch2 > AtMid) // Forward
            { 
              Motor1.writeMicroseconds(AtMid - spd1ppm);
              Motor2.writeMicroseconds(AtMid - spd1ppm);
              Motor3.writeMicroseconds(AtMid + spd1ppm);
              Motor4.writeMicroseconds(AtMid + spd1ppm);

              Motor9.writeMicroseconds(AtMid - spd1ppm);
              Motor10.writeMicroseconds(AtMid + spd1ppm);
            
            }
            else if (ch2 < AtMid) // Reverse
            {
              Motor1.writeMicroseconds(AtMid + spd1ppm);
              Motor2.writeMicroseconds(AtMid + spd1ppm);
              Motor3.writeMicroseconds(AtMid - spd1ppm);
              Motor4.writeMicroseconds(AtMid - spd1ppm);

              Motor9.writeMicroseconds(AtMid + spd1ppm);
              Motor10.writeMicroseconds(AtMid - spd1ppm);
            
            }
            else  // At Rest
            {
              Motor1.writeMicroseconds(ch2);
              Motor2.writeMicroseconds(ch2);
              Motor3.writeMicroseconds(ch2);
              Motor4.writeMicroseconds(ch2);

              Motor9.writeMicroseconds(ch2);
              Motor10.writeMicroseconds(ch2);
            
            }
          }
          else
          {
              Motor1.writeMicroseconds(ch2);
              Motor2.writeMicroseconds(ch2);
              Motor3.writeMicroseconds(ch2);
              Motor4.writeMicroseconds(ch2);          

              Motor9.writeMicroseconds(ch2);
              Motor10.writeMicroseconds(ch2);
          
          }
          break;
    default:

// check if testmode

        if ((ch3 < AtMid) && (ch6 > EPAHIGH))
        {
            // TESTMODE CH4 POS3, CH3 LOCK, CH6 PAN LEFT

            if ((ch5>=1000) && (ch5 < 1100)) Motor1.writeMicroseconds(ch2);
            else if ((ch5>=1100) && (ch5 < 1200)) Motor2.writeMicroseconds(ch2);
            else if ((ch5>=1200) && (ch5 < 1300)) Motor9.writeMicroseconds(ch2);
            else if ((ch5>=1300) && (ch5 < 1400)) Motor3.writeMicroseconds(ch2);
            else if ((ch5>=1400) && (ch5 < 1500)) Motor4.writeMicroseconds(ch2);
            else if ((ch5>=1500) && (ch5 < 1600)) Motor10.writeMicroseconds(ch2);
            else if ((ch5>=1600) && (ch5 < 1700)) Motor5.writeMicroseconds(ch2);
            else if ((ch5>=1700) && (ch5 < 1800)) Motor6.writeMicroseconds(ch2);
            else if ((ch5>=1800) && (ch5 < 1900)) Motor7.writeMicroseconds(ch2);
            else  Motor8.writeMicroseconds(ch2);

        }
        else
        {
        if ((ch2 < AboveMid) &&  (ch2 > BelowMid)) 
        {
          if  ((ch1 > AboveMid) ||  (ch1 < BelowMid))
          {
            Motor7.write(SRVFROFST - (InnerFront/2)); // front wheel steer right
            Motor8.write(SRVRROFST + (InnerBack/2)); // back wheel steer left for overall steering to the right of the rover
            Motor5.write(SRVFLOFST + (InnerFront/2)); // front wheel steer right
            Motor6.write(SRVRLOFST - (InnerBack/2)); // back wheel steer left for overall steering to the right of the rover
          }
          else
          {
            Motor5.write(SRVFLOFST);
            Motor6.write(SRVRLOFST);
            Motor7.write(SRVFROFST);
            Motor8.write(SRVRROFST);

          }
          
          if ((ch1 > EPANEARHIGH)|| (ch1 < EPANEARLOW))
          {
              Motor1.writeMicroseconds(ch1);
              Motor2.writeMicroseconds(ch1);
              Motor3.writeMicroseconds(AtMid - (ch1 - AtMid));
              Motor4.writeMicroseconds(AtMid - (ch1 - AtMid));

              Motor9.writeMicroseconds(ch1);
              Motor10.writeMicroseconds(AtMid - (ch1 - AtMid));
          
          }
          else
          {
              Motor1.writeMicroseconds(ch2);
              Motor2.writeMicroseconds(ch2);
              Motor3.writeMicroseconds(ch2);
              Motor4.writeMicroseconds(ch2);          

              Motor9.writeMicroseconds(ch2);
              Motor10.writeMicroseconds(ch2);
          
          }


          
        }
        else
        {
          // right
          if (ch1 > AboveMid) {
          // Corner Steering Motors
          Motor7.write(SRVFROFST + InnerFront); // front wheel steer right
          Motor8.write(SRVRROFST - InnerBack); // back wheel steer left for overall steering to the right of the rover
          Motor5.write(SRVFLOFST + OuterFront); // front wheel steer right
          Motor6.write(SRVRLOFST - OuterBack); // back wheel steer left for overall steering to the right of the rover

          // Set speed of all wheel motors
          if (ch2 > AtMid) // Forward
          { 
            Motor1.writeMicroseconds(AtMid + spd1ppm);
            Motor2.writeMicroseconds(AtMid + spd1ppm);
            Motor3.writeMicroseconds(AtMid + spd2ppm);
            Motor4.writeMicroseconds(AtMid + spd3ppm);

            Motor9.writeMicroseconds(AtMid + spd1ppm);
            Motor10.writeMicroseconds(AtMid + spd2ppm);
          
          }
          else if (ch2 < AtMid) // Reverse
          {
            Motor1.writeMicroseconds(AtMid - spd1ppm);
            Motor2.writeMicroseconds(AtMid - spd1ppm);
            Motor3.writeMicroseconds(AtMid - spd2ppm);
            Motor4.writeMicroseconds(AtMid - spd3ppm);

            Motor9.writeMicroseconds(AtMid - spd1ppm);
            Motor10.writeMicroseconds(AtMid - spd2ppm);

          
          }
          else  // At Rest
          {
            Motor1.writeMicroseconds(ch2);
            Motor2.writeMicroseconds(ch2);
            Motor3.writeMicroseconds(ch2);
            Motor4.writeMicroseconds(ch2);

            Motor9.writeMicroseconds(ch2);
            Motor10.writeMicroseconds(ch2);

          
          }

          }

          // left
          else if (ch1 < BelowMid) {
          // Corner Steering motors
          Motor7.write(SRVFROFST - OuterFront);
          Motor8.write(SRVRROFST + OuterBack);
          Motor5.write(SRVFLOFST - InnerFront);
          Motor6.write(SRVRLOFST + InnerBack);

          if (ch2 > AtMid) // Forward
          { 
            Motor1.writeMicroseconds(AtMid + spd2ppm);
            Motor2.writeMicroseconds(AtMid + spd3ppm);
            Motor3.writeMicroseconds(AtMid + spd1ppm);
            Motor4.writeMicroseconds(AtMid + spd1ppm);

            Motor9.writeMicroseconds(AtMid + spd2ppm);
            Motor10.writeMicroseconds(AtMid + spd1ppm);
          
          }
          else if (ch2 < AtMid) // Reverse
          { 
            Motor1.writeMicroseconds(AtMid - spd2ppm);
            Motor2.writeMicroseconds(AtMid - spd3ppm);
            Motor3.writeMicroseconds(AtMid - spd1ppm);
            Motor4.writeMicroseconds(AtMid - spd1ppm);

            Motor9.writeMicroseconds(AtMid - spd2ppm);
            Motor10.writeMicroseconds(AtMid - spd1ppm);

          }
          else
          {
            Motor1.writeMicroseconds(ch2);
            Motor2.writeMicroseconds(ch2);
            Motor3.writeMicroseconds(ch2);
            Motor4.writeMicroseconds(ch2);

            Motor9.writeMicroseconds(ch2);
            Motor10.writeMicroseconds(ch2);
          
          }

          }

          else {

            // Straight driving (throttle only)
  
            Motor1.writeMicroseconds(ch2);
            Motor2.writeMicroseconds(ch2);
            Motor3.writeMicroseconds(ch2);
            Motor4.writeMicroseconds(ch2);

            Motor9.writeMicroseconds(ch2);
            Motor10.writeMicroseconds(ch2);

  
            // Corner Steering Motors in straight position
  
            Motor5.write(SRVFLOFST);
            Motor6.write(SRVRLOFST);
            Motor7.write(SRVFROFST);
            Motor8.write(SRVRROFST);

            }

        } 
        }  
          break;
        
  }

} // Rawmode
}
void GetMotorsSpeed() {
  // if no steering, all wheels speed is the same - straight move
  if (ch1 == AtMid) {
    spd1 = spd2 = spd3 = motorspeed;
  }

  else {
    spd1 = motorspeed;
    spd2 = motorspeed * sqrt(pow(CentertoFront, 2) + 
       pow((steerradius - CentertoCorner), 2)) / 
       (steerradius + CentertoMid);
    spd3 = motorspeed * (steerradius - CentertoMid) / 
  (steerradius + CentertoMid);
  }

  spd1ppm = map(round(spd1), 0, 100, 0, ppmrange);
  spd2ppm = map(round(spd2), 0, 100, 0, ppmrange);
  spd3ppm = map(round(spd3), 0, 100, 0, ppmrange);
}

void GetMotorAngle() {

  OuterFront = round((atan((CentertoFront / (steerradius + CentertoCorner)))) * 180 / PI);
  OuterBack = round((atan((CentertoRear / (steerradius + CentertoCorner)))) * 180 / PI);
  InnerFront = round((atan((CentertoFront / (steerradius - CentertoCorner)))) * 180 / PI);
  InnerBack = round((atan((CentertoRear / (steerradius - CentertoCorner)))) * 180 / PI);

}

void requestEvent() {

  // Always change 2 bytes for every request if there are changes in TX values Lowbyte first

  if (Pch1 != TXch1) { Pch1 = TXch1; Wire.write(Pch1 & 0xff);Wire.write((Pch1 >> 8)+0x10);}
  else if (Pch2 != TXch2) { Pch2 = TXch2; Wire.write(Pch2 & 0xff);Wire.write((Pch2 >> 8)+0x20);}
  else if (Pch3 != TXch3) { Pch3 = TXch3; Wire.write(Pch3 & 0xff);Wire.write((Pch3 >> 8)+0x30);}
  else if (Pch4 != TXch4) { Pch4 = TXch4; Wire.write(Pch4 & 0xff);Wire.write((Pch4 >> 8)+0x40);}
  else if (Pch5 != TXch5) { Pch5 = TXch5; Wire.write(Pch5 & 0xff);Wire.write((Pch5 >> 8)+0x50);}
  else if (Pch6 != TXch6) { Pch6 = TXch6; Wire.write(Pch6 & 0xff);Wire.write((Pch6 >> 8)+0x60);}
  else { Wire.write(0);Wire.write(0);}
  
}
void receiveEvent(int howMany)
{
  int chan,chanval,changroup;
  unsigned char c1, c2;
  
  while(1 < Wire.available()) // loop through all but the last
     c1 = Wire.read(); // receive byte as a character
  while(0 < Wire.available())   
     c2 = Wire.read();    // receive byte as an integer

    chan = (c2 & 0xF0) >> 4;
    chanval = ((c2 & 0x07)<<8) + c1;
    changroup = c2 & 0x08;
    if ((chanval > 2000) || (chanval < 1000)) chan = -1; 

//Serial.print("ch ");Serial.print(chan);Serial.print(" ");Serial.println(chanval);

    if (changroup == 0)
    switch (chan)
    {
      case 1:
                ch1 = chanval;
                break;
      case 2:
                ch2 = chanval;
                break;
      case 3:
                ch3 = chanval;
                break;
      case 4:
                ch4 = chanval;
                break;
      case 5:
                ch5 = chanval;
                break;
      case 6:
                ch6 = chanval;
                break;

      case 0xa:
                TXBypass = chanval;
                break;
      case 0xb:
                digitalWrite(A1, HIGH);
                break;
      case 0xc:
                digitalWrite(A2, HIGH);
                break;
      case 0xd:
                digitalWrite(A3, HIGH);
                break;
      case 0xe:
                digitalWrite(A1, LOW);
                digitalWrite(A2, LOW);
                digitalWrite(A3, LOW);
                break;
      case 0xf:
                switch(chanval & 0x7)
                {
                    case 1:
                            Pch1 = 0;break;
                    case 2:
                            Pch2 = 0;break;
                    case 3:
                            Pch3 = 0;break;
                    case 4:
                            Pch4 = 0;break;
                    case 5:
                            Pch5 = 0;break;
                    case 6:
                            Pch6 = 0;break;
                    default:        
                            Pch1 = 0;
                            Pch2 = 0;
                            Pch3 = 0;
                            Pch4 = 0;
                            Pch5 = 0;
                            Pch6 = 0;
                            break;

                }
                    break;
      
      default:
                break;
    }           
    else
     switch (chan)
    {

      case 0:   RawMode = chanval;
                break;
      
      case 1:
                Motor1.writeMicroseconds(chanval);
                break;
      case 2:
                Motor2.writeMicroseconds(chanval);
                break;
      case 3:
                Motor3.writeMicroseconds(chanval);
                break;
      case 4:
                Motor4.writeMicroseconds(chanval);
                break;
      case 5:
                Motor5.writeMicroseconds(chanval);
                break;
      case 6:
                Motor6.writeMicroseconds(chanval);
                break;

      case 7:
                Motor7.writeMicroseconds(chanval);
                break;
      case 8:
                Motor8.writeMicroseconds(chanval);
                break;
      case 9:
                Motor9.writeMicroseconds(chanval);
                break;
      case 10:
                Motor10.writeMicroseconds(chanval);
                break;
      case 11:
                ServoRSM.writeMicroseconds(chanval);
                break;
      case 12:
                ServoING.writeMicroseconds(chanval);
                break;
     
      default:
                break;
    }                   
    

}
