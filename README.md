# EQM_MARSROVER
A 3d printed Mars Rover with DUAL Arduino Electronics

A 3D Printed Perseverance Mars Rover of NASA with dual Arduino Electronics.  The first Arduino a NANO, managages all 6 motors (speed and direction) of the 6-Wheel Rover where each wheel
is independently controlloable using the Arduino Servo libary. The motor speed controller uses the SG90 Servo board including the feedback potentiometer as an Electronic Speed Controller 
for each of the 6 wheels. Four more servo boards are added to manage the steering process of all four corner wheels. So thats 6+4 SG90 Servo boards, 6 acting as Motor ESC and 4 as 
wheel steering. A 2nd BLE-33 Sense NANO Arduino is used to process sensor data (built-in sensors within the Arduino, LIDARS, PIXYCAM, and MPU Gyro Sensor) The BLE-33 Sens NANO communicates
with the 1st NANO Arduino via I2c.




![image](https://github.com/EQMOD/EQM_MARSROVER/assets/29789200/4328c751-df54-4b1c-b8ee-8e21aec6d2a7)

