# EQM_MARSROVER
A 3d printed Mars Rover with DUAL Arduino Electronics

A 3D Printed Perseverance Mars Rover of NASA with dual Arduino Electronics.  The first Arduino a NANO, managages all 6 motors (speed and direction) of the 6-Wheel Rover where each wheel
is independently controlloable using the Arduino Servo libary. The motor speed controller uses the SG90 Servo board including the feedback potentiometer as an Electronic Speed Controller 
for each of the 6 wheels. Four more servo boards are added to manage the steering process of all four corner wheels. So thats 6+4 SG90 Servo boards, 6 acting as Motor ESC and 4 as 
wheel steering. A 2nd BLE-33 Sense NANO Arduino is used to process sensor data (built-in sensors within the Arduino, LIDARS, PIXYCAM, and MPU Gyro Sensor) The BLE-33 Sens NANO communicates
with the 1st NANO Arduino via I2c. The entire platform is 3D-Printed with STL files downloaded from the NASA Website/GITHUB and were modified to accomodate the electronics and motors/servos. 

https://github.com/nasa/NASA-3D-Resources/tree/master/3D%20Models/M2020-Model-Rover-STLs512020


![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/4328c751-df54-4b1c-b8ee-8e21aec6d2a7)

The platform also include a mini version of the Mars Ingenuity Helicopter to demonstrate the deployment process using a SG90 Servo to deploy the model under the perseverance rover belly;

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/79b43e7b-d5b3-4601-8570-b12e8613e1d1)

Ingenuity Deployment assembly  (Version 1)

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/6351f5d2-54e2-4c00-9d4d-262704d052c5)

Ingenuity Deployment assembly  (Version 2)

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/df9eada2-5fa6-46ed-8619-a233e9c0022b)


The Platform uses multiple Sensors ((built-in sensors within the Arduino, LIDARS, PIXYCAM, and MPU Gyro Sensor) which can be used for multi-processing experiments

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/b5dcec39-2dc2-437f-a3ca-f72d1ef5a5eb)


Below is the connection diagram of all the motors, servos, and sensors. Motor/Servo Control is distributed on the Arduino NANO (Acting as Slave) and the rest of the sensors including 
those connected to the I2C Bus are being controlled by the Arduino BLE-33 NANO Sense which is acting as the Master.  The Coring Motor (Rover Drill Motor) is connected to the BLE33 Sense
as well the Servo Library Pins on the Arduino NANO side are fully utilized.

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/c45d3ab0-8066-4891-a9b8-de1cc6d34da8)


ARDUINO MASTER-SLAVE CONFIGURATION

The Arduninos operates in a master-slave configuration with the Master (NANO BLE Sense) defining 3 modes of operation;

Mode 1: RC Mode

        In this mode, control of the Rover is via a FLYSKY GT5 TX and FS-82 Receiver attached to the NANO Slave. The arduino NANO simply receives the channel info data from the 
        transmitter via I-BUS connected to the Arduino RX pin and passes control to the Arduino Servo Library which performs the Motor/Servo control of the Rover.

Mode 2: RC with Sensor Mode

        In This mode, The Arduino NANO Slave cascades the FLYSKY Transmitter data to the NANO BLE Sense Master via I2C. The NANO BLE Sense Master then processes the transmitter 
        data together with the sensor data attached to it and sends back to the NANO Slave the Motor/Servo control commands.
        
Mode 3: Autonomous Mode

        In this mode, there is no human intervention from the FLSKY Transmitter. Instead the Arduino BLE NANO Master reads the sensor data , decides on the what motor/servo
        commands to pass to the Arduino NANO Slave based on the information gathered from the sensors (LIDAR, PIXY, Audio, MPU6050 Gyro/Accelerometer, etc.


![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/4f3efcbb-e6b2-4392-8def-7a0c5cb85eeb)


LIDARS and the CMU PIXY Camera

The platform has four LIDAR sensors and one CMY PIXY Camera for Visual Recognition

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/6f479b8b-03a4-46ec-9c55-92a704540526)

Twp of the LIDAR Sensors are attached on each front wheel and one on the back wheel;

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/702ff8fd-c07d-44d7-92da-44afd81b931b)

And another LIDAR sensor is located on the RSM Mast head side by side with a stand-alone wifi camera ;

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/32e32e55-f442-497a-bd93-60bd3ce435c4)

A CMU PIXY Camera is attached on the front of the rover for realtime "front-of-rover" image analysis;

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/8d61770e-5f0d-4a02-8d5b-9912a3dd4130)

Arduino - PIXY CAMERA Processing

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/8d23974c-3e57-4073-850f-e0413a448563)

LIDAR and CMU Pixy Camera I2C BUS Addressing;

Anything attached on the I2C bus are addressable. The included code manages the pre-programming stuff of setting up the I2C address on the multiple LIDAR sensors so there
will be no bus conflicts;

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/fbca01ad-cfc3-45f9-81b4-4de3b7c4f917)



RC Electronics

The platform utlizes the FLYSKY TX-RX modules for receiving user command data from a FLYSKY FS-GT5 Transmitter and is received by the Ardruino Nano Slave via Flysky's I-BUS protocol 
which is directly fed to the Arduino NANO RX Pin.


![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/be19a366-b1a7-445b-8527-a3817f2438f4)


![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/70fd4236-a379-401d-a45a-9f005ba30900)


Rover Wheels Geared Motors

Each wheel is driven independently by geared motors attaced on the bogie arm assembly.

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/cfda113e-3f94-4ccc-8b49-5045ec8c9d28)

The motor drivers are from the SG90 Servo boards acting as Electronic Speed Controllers with the feedback potentiometers adjusted for home position (where the motors stop 
rotating at TX / Steering wheel mid stick position)

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/d2f1dbe1-41c6-468d-8b2d-bcc20d1cbefe)



Steering Servos. 

To reduce the space occupied by the servos, the SG90 servos where disassembled and the electronics attached to the 3d-printed bogie arm. The same Geared Motors on the 
wheels were used as motors for the servo arms.

![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/77acacf9-babd-4d45-926f-62904f1f3a32)


![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/4a9c4fce-a9e6-4535-bd4f-a6fa789cf5eb)































