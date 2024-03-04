
// #include <Arduino.h>
#include <RPLidar.h>
RPLidar lidar;
// Define UART and Serial baud rates
#define LIDAR_BAUD_RATE 115200  // Example baud rate for RP1 LiDAR
#define SERIAL_BAUD_RATE 115200   // Baud rate for communication with Mac

#define RPLIDAR_MOTOR D4 // The PWM pin for control the speed of RPLIDAR's motor.


#include <Servo.h>

Servo myservo;  // Create a servo object to control a servo

float minDistance = 100000;
float angleAtMinDist = 0;
// int pos = 50;  
//50 horizontal
//Anything above 65 do not do it it goes backwards
//16 forward max



int pos = 1400;

const int max_hoz_angle = 30;
const int min_hoz_angle = 330;
const int hor_multiplier = 1;

const int max_vert_angle = 1000;
const int min_vert_angle = 700;
const int hor_scan_size = ((max_hoz_angle-0) +(360-min_hoz_angle))*hor_multiplier;
int horizontal_scan[hor_scan_size+1];

void setup() {
  // Initialize Serial communication with the Mac
  // myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(SERIAL_BAUD_RATE);
  Serial1.begin(LIDAR_BAUD_RATE);
  // Initialize UART communication with the LiDAR
  lidar.begin(Serial1);  // Assuming LiDAR is connected to Serial1

  UART myUART1(PA_9,  PA_10, NC, NC);
                
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  myservo.attach(D10);  // Attaches the servo on pin D10 to the servo object


}


  void loop() {
    if (IS_OK(lidar.waitPoint())) {
      //perform data processing here...
      float distance = lidar.getCurrentPoint().distance;
      float angle = lidar.getCurrentPoint().angle;  // 0-360 deg


      if (pos > max_vert_angle-1){
        pos = min_vert_angle;
        myservo.write(pos);
      }
      else if (lidar.getCurrentPoint().startBit){
        pos  = pos +2;
        myservo.writeMicroseconds(pos);
        Serial.print("POS");
        Serial.println(pos);

        // for (int i = 0; i < hor_scan_size; i++) {
        //   // Serial.print("Angle ");
        //   // Serial.print(i+min_hoz_angle);
        //   // Serial.print("Distance ");
        //   // Serial.println(horizontal_scan[i]);
        // }

        for (int i = 0; i <= hor_scan_size*hor_multiplier; i++) {
          Serial.print("Angle ");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(horizontal_scan[i]);
        }
      }

      // Serial.print("Angle_rough ");
      int angle_2 = angle*hor_multiplier;

      // Serial.println(angle_2);
      int angle_int = (int)round(angle_2);
      // Serial.print("Angle:");
      // Serial.print(angle_int);
      // Serial.println();
      // if  (angle_int >= min_hoz_angle | angle_int <= max_hoz_angle){
      //   // horizontal_scan[angle_int-min_hoz_angle] = distance;

      //   Serial.print("Angle:");
      //   Serial.print(angle_int);
      //   Serial.print("Distance:");
      //   Serial.print(distance);
      //   Serial.println();
      // }


      if  (angle_int >= min_hoz_angle*hor_multiplier){
        // Serial.print("Size");
        // Serial.print(hor_scan_size);

        // Serial.print("Index:");
        // int index = angle_int-min_hoz_angle;
        // Serial.print(index);

        horizontal_scan[angle_int-min_hoz_angle*hor_multiplier] = distance;
        // Serial.print("Angle:");
        // Serial.print(angle_int);
        // Serial.print("Distance:");
        // Serial.print(distance);
        // Serial.println();
      }

      if  (angle_int <= max_hoz_angle*hor_multiplier){
        // Serial.print("Index:");
        // int index = angle_int+max_hoz_angle;
        // Serial.print(index);
        horizontal_scan[angle_int+max_hoz_angle*hor_multiplier] = distance;
        // Serial.print("Angle:");
        // Serial.print(angle_int);
        // Serial.print("Distance:");
        // Serial.print(distance);
        // Serial.println();
      }





      // if (lidar.getCurrentPoint().startBit) {
      //    // a new scan, display the previous data...
      //    printData(angleAtMinDist, minDistance);
      //    minDistance = 100000;
      //    angleAtMinDist = 0;
      // } else {
      //    if ( distance > 0 &&  distance < minDistance) {
      //       minDistance = distance;
      //       angleAtMinDist = angle;
      //    }
      // }
    }
    else {
      analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor
      // Try to detect RPLIDAR
      rplidar_response_device_info_t info;
      if (IS_OK(lidar.getDeviceInfo(info, 100))) {
        // Detected
        lidar.startScan();
        analogWrite(RPLIDAR_MOTOR, 200);
        delay(1000);
      }
    }
}
