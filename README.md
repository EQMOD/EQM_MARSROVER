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






