#include <PID_v1.h>
#include <Arduino_PortentaBreakout.h>

// Define the connections to the motor driver and the encoder
// const int motorPinA = 0; // PWM output for motor speed control  PWM6
// const int direction = 5; // PWM output for motor speed control  PWM1
// const int motorPin2A = 6; // PWM output for motor speed control  PWM0
// const int direction2 = 11; // PWM output for motor speed control  I2C0 SDA
// const int encoderPinA = 2; // Replace with your actual encoder pin A  PWM4
// const int encoderPinB = 3; // Replace with your actual encoder pin B PWM3
// const int encoderPin2A = 8; // Replace with your actual encoder pin A SPI1 COPI
// const int encoderPin2B = 9; // Replace with your actual encoder pin B SPI1 CE

breakoutPin motorPinA = PWM6; // PWM output for motor speed control  PWM6
breakoutPin direction = GPIO_2; // PWM output for motor speed control  PWM1
breakoutPin motorPin2A = PWM0; // PWM output for motor speed control  PWM0
breakoutPin direction2 = I2C_SDA_0; // PWM output for motor speed control  I2C0 SDA
breakoutPin encoderPinA = PWM5; // Replace with your actual encoder pin A  PWM4 maybe PWM5
breakoutPin encoderPinB = PWM3; // Replace with your actual encoder pin B PWM3
breakoutPin encoderPin2A = SPI1_COPI; // Replace with your actual encoder pin A SPI1 COPI
breakoutPin encoderPin2B = SPI1_CK; // Replace with your actual encoder pin B SPI1 CE
breakoutPin ultrasoundPin = GPIO_6;

volatile long counter = 0; // Counter to keep track of encoder counts
volatile long counterR = 0; // Counter to keep track of encoder counts
volatile long prev_counter = 0; // Counter to keep track of encoder counts
volatile long prev_counterR = 0; // Counter to keep track of encoder counts
volatile long state=2;
volatile long prevstate=1;
volatile long Rstate=2;
volatile long Rprevstate=1;
volatile int timetostop=0;

// PID parameters - Adjust these based on your system's response
double Kp = 0.25, Ki = 0.01, Kd = 0.05;

// Variables for distance measurement and control

double targetspeed = 650; // Target distance in meters
double currentspeed = 0.0; // Current distance traveled in meters
double currentspeedR = 0.0; // Current distance traveled in meters
double motorSpeed = 0; // Current motor speed (PWM value)
double motorSpeedR = 0; // Current motor speed (PWM value)
double setPoint, input, output;
double setPointR, inputR, outputR;
double current_time, prev_time;
double start_time=0;

// Setup the PID controller
PID myPID(&input, &output, &setPoint, Kp, Ki, Kd, DIRECT);
PID myPIDR(&inputR, &outputR, &setPointR, Kp, Ki, Kd, DIRECT);
// Encoder interrupt service routine

void updateEncoder() {
  // Check the state of both channels to determine the direction of rotation
  

  if (Breakout.digitalRead(encoderPinA) == 1) {
    if (Breakout.digitalRead(encoderPinB) == 1) {
      state =1;
    }
    else{
      state =4;
    }

    }else{
     if (Breakout.digitalRead(encoderPinB) == 1) {
      state =1;
    }
    else{
      state =3;
    
    }
    }
    if (state==1 && prevstate==4){
      counter++;
      prevstate=state;
    }
    else if (state==1 && prevstate==2){
      counter--;
      prevstate=state;
    }
    else if(state==2 && prevstate==1){
      counter++;
      prevstate=state;
    }else if(state==2 && prevstate==3){
      counter--;
      prevstate=state;
    }else if(state==3 && prevstate==2){
      counter++;
      prevstate=state;
    }
    else if(state==4 && prevstate==3){
      counter++;
      prevstate=state;
    }else if(state==4 && prevstate==1){
      counter--;
      prevstate=state;
    }else{
      //counter++;
      prevstate=state;
    }

}


void updateEncoderR() {
  // Check the state of both channels to determine the direction of rotation
  

  if (Breakout.digitalRead(encoderPin2A) == 1) {
    if (Breakout.digitalRead(encoderPin2B) == 1) {
      Rstate =1;
    }
    else{
      Rstate =4;
    }

    }else{
     if (Breakout.digitalRead(encoderPin2B) == 1) {
      Rstate =1;
    }
    else{
      Rstate =3;
    
    }
    }
    if (Rstate==1 && Rprevstate==4){
      counterR++;
      Rprevstate=Rstate;
    }
    else if (Rstate==1 && Rprevstate==2){
      counterR--;
      Rprevstate=Rstate;
    }
    else if(Rstate==2 && Rprevstate==1){
      counterR++;
      Rprevstate=Rstate;
    }else if(Rstate==2 && Rprevstate==3){
      counterR--;
      Rprevstate=Rstate;
    }else if(Rstate==3 && Rprevstate==2){
      counterR++;
      Rprevstate=Rstate;
    }
    else if(Rstate==4 && Rprevstate==3){
      counterR++;
      Rprevstate=Rstate;
    }else if(Rstate==4 && Rprevstate==1){
      counterR--;
      Rprevstate=Rstate;
    }else{
      //counter++;
      Rprevstate=Rstate;
    }

}


void setup() {
  // Initialize serial communication for debugging
  //Serial.begin(9600);

  // // Setup motor control pin
  Breakout.pinMode(motorPinA, OUTPUT);

  Breakout.pinMode(encoderPinA, INPUT);
  Breakout.pinMode(encoderPinB, INPUT);
  Breakout.pinMode(direction, OUTPUT);
  Breakout.pinMode(ultrasoundPin, INPUT);  
  
  // // Set pin 1 to HIGH
  Breakout.digitalWrite(direction,HIGH);

  Breakout.pinMode(motorPin2A, OUTPUT);

  Breakout.pinMode(encoderPin2A, INPUT);
  Breakout.pinMode(encoderPin2B, INPUT);
  Breakout.pinMode(direction2, OUTPUT);
  
  // // Set pin 1 to HIGH
  Breakout.digitalWrite(direction2, LOW);

  // Attach interrupts to both rising and falling edges of the encoder signals
  attachInterrupt(digitalPinToInterrupt(2), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), updateEncoder, CHANGE);

  attachInterrupt(digitalPinToInterrupt(8), updateEncoderR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(9), updateEncoderR, CHANGE);
  delay(5000);
  // Initialize the PID controller
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(50);
  myPID.SetOutputLimits(0, 255); // PWM range for most Arduino boards

  myPIDR.SetMode(AUTOMATIC);
  myPIDR.SetSampleTime(50);
  myPIDR.SetOutputLimits(0, 255); // PWM range for most Arduino boards

  // Convert target distance to encoder ticks, assuming 1 meter = x encoder ticks
  // This will need to be adjusted based on your specific encoder and wheel diameter
  setPoint = 250; // Placeholder for conversion from meters to ticks
  setPointR = 250;
  timetostop=prev_time=current_time=millis();// start the timer
    
}

float calculatespeed(float prevcounter, float newcounter, float dt){
  float speed = (newcounter-prevcounter)/(dt*0.001);
  return speed;
}

void loop() {

  float dt;
  delay(50);
  current_time = millis(); // gives back the elapsed time in milli or micro seconds
  // Calculate the current distance in encoder ticks
  dt= current_time-prev_time;
  
  currentspeed = calculatespeed(prev_counter, counter, dt); // Adjust this calculation based on your encoder
  currentspeedR = calculatespeed(prev_counterR, counterR, dt);
  // Update PID controller
  input = currentspeed;
  inputR = currentspeedR;
  myPID.Compute();
  myPIDR.Compute();
  Breakout.analogWrite(motorPinA, 90);
  Breakout.analogWrite(motorPin2A, 90);
  prev_time=current_time;
  prev_counter=counter;
  prev_counterR=counterR;
  //Serial.print("encoderCounta: ");
  //Serial.println(current_time); 

   if(Breakout.digitalRead(ultrasoundPin) == HIGH){
     while(Breakout.digitalRead(ultrasoundPin) == HIGH){
   Breakout.analogWrite(motorPinA, 0);
   Breakout.analogWrite(motorPin2A, 0);
}
    delay(3000);
   Breakout.digitalWrite(direction2, HIGH);
   start_time= millis();
  
    Breakout.analogWrite(motorPinA, 80);
    Breakout.analogWrite(motorPin2A, 80);
    delay(2000);

    Breakout.analogWrite(motorPinA, 0);
    Breakout.analogWrite(motorPin2A, 0);
    delay(2000);
    Breakout.digitalWrite(direction2, LOW);
    prev_time=current_time= millis();
    prev_counter=counter;
    prev_counterR=counterR;
  }
  
  if ((current_time-timetostop)>52000){
     while (1){
       Breakout.analogWrite(motorPinA, 0);
       Breakout.analogWrite(motorPin2A, 0);
     }
   // Check if the target distance has been reached
   }
}
