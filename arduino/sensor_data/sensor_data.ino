// Include necessary libraries
#include <Arduino.h>
#include <RPLidar.h> // For LiDAR sensor operations
#include <Servo.h>   // For servo control
#include <Arduino_PortentaBreakout.h>

// Define constants for baud rates and the RPLIDAR motor control pin
#define LIDAR_BAUD_RATE 115200 // Baud rate for RP1 LiDAR communication
#define SERIAL_BAUD_RATE 115200 // Baud rate for serial communication with PC
#define RPLIDAR_MOTOR  PWM4//PWM4 // PWM pin for controlling the speed of RPLIDAR's motor

// Create objects for the RPLidar and Servo
RPLidar lidar;
Servo myservo;

// Variables for managing distance and angle measurements
float minDistance = 100000; // Initialize with a high value
float angleAtMinDist = 0;

// Positioning for the servo
int pos = 1400; // Starting position

// Angle configurations for horizontal and vertical movements
const int max_hoz_angle = 30;
const int min_hoz_angle = 330;
const int hor_multiplier = 1;
const int max_vert_angle = 1000;
const int min_vert_angle = 700;
const int hor_scan_size = ((max_hoz_angle - 0) + (360 - min_hoz_angle)) * hor_multiplier;
int horizontal_scan[hor_scan_size + 1]; // Array to store horizontal scan data

void setup() {
  // Initialize Serial communication
  Serial.begin(SERIAL_BAUD_RATE);
  Serial2.begin(LIDAR_BAUD_RATE); // Setup Serial1 for LiDAR communication

  // Setup RPLIDAR
  lidar.begin(Serial2); // Assuming LiDAR is connected to Serial1
  pinMode(RPLIDAR_MOTOR, OUTPUT); // Set RPLIDAR motor pin as output
  myservo.attach(10);  //PWM 8



}

void loop() {
  analogWrite(RPLIDAR_MOTOR, 200); // Start the RPLIDAR motor

  if (IS_OK(lidar.waitPoint())) {
    Serial.print("hey1: ");
    // Data processing when a point is successfully read from LiDAR
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle; // Angle in degrees (0-360)

    // Servo control logic based on LiDAR readings
    if (pos > max_vert_angle - 1) {
      pos = min_vert_angle;
      myservo.write(pos);
    } else if (lidar.getCurrentPoint().startBit) {
      pos += 2;
      myservo.writeMicroseconds(pos);
      Serial.print("POS: ");
      Serial.println(pos);
    }

    // Display angle and distance readings
    int angle_2 = angle * hor_multiplier;
    int angle_int = (int)round(angle_2);

    if (angle_int >= min_hoz_angle * hor_multiplier) {
      horizontal_scan[angle_int - min_hoz_angle * hor_multiplier] = distance;
    }

    if (angle_int <= max_hoz_angle * hor_multiplier) {
      horizontal_scan[angle_int + max_hoz_angle * hor_multiplier] = distance;
    }
  } else {
    // If LiDAR is not OK, stop the motor and try reinitializing
    analogWrite(RPLIDAR_MOTOR, 0); // Stop the RPLIDAR motor

    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
      // RPLIDAR detected, start scan and motor
      lidar.startScan();
      analogWrite(RPLIDAR_MOTOR, 200); // Start the RPLIDAR motor
      delay(1000);
    }
  }

 
}
