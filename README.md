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







