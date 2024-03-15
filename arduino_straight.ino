#include <PID_v1.h>

// Define the connections to the motor driver and the encoder
const int motorPinA = 0; // PWM output for motor speed control
const int direction = 5; // PWM output for motor speed control
const int motorPin2A = 6; // PWM output for motor speed control
const int direction2 = 11; // PWM output for motor speed control
const int encoderPinA = 2; // Replace with your actual encoder pin A
const int encoderPinB = 3; // Replace with your actual encoder pin B
const int encoderPin2A = 8; // Replace with your actual encoder pin A
const int encoderPin2B = 9; // Replace with your actual encoder pin B
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
double Kp = 0.6, Ki = 0, Kd = 0;

// Variables for distance measurement and control

double targetspeed = 650; // Target distance in meters
double currentspeed = 0.0; // Current distance traveled in meters
double currentspeedR = 0.0; // Current distance traveled in meters
double motorSpeed = 0; // Current motor speed (PWM value)
double motorSpeedR = 0; // Current motor speed (PWM value)
double setPoint, input, output;
double setPointR, inputR, outputR;
double current_time, prev_time;

// Setup the PID controller
PID myPID(&input, &output, &setPoint, Kp, Ki, Kd, DIRECT);
PID myPIDR(&inputR, &outputR, &setPointR, Kp, Ki, Kd, DIRECT);
// Encoder interrupt service routine

void updateEncoder() {
  // Check the state of both channels to determine the direction of rotation
  

  if (digitalRead(encoderPinA) == 1) {
    if (digitalRead(encoderPinB) == 1) {
      state =1;
    }
    else{
      state =4;
    }

    }else{
     if (digitalRead(encoderPinB) == 1) {
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
  

  if (digitalRead(encoderPin2A) == 1) {
    if (digitalRead(encoderPin2B) == 1) {
      Rstate =1;
    }
    else{
      Rstate =4;
    }

    }else{
     if (digitalRead(encoderPin2B) == 1) {
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
  Serial.begin(9600);

  // Setup motor control pin
  pinMode(motorPinA, OUTPUT);

  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(direction, OUTPUT);
  pinMode(12, INPUT);  
  
  // Set pin 1 to HIGH
  digitalWrite(direction, LOW);

  pinMode(motorPin2A, OUTPUT);

  pinMode(encoderPin2A, INPUT);
  pinMode(encoderPin2B, INPUT);
  pinMode(direction2, OUTPUT);
  
  // Set pin 1 to HIGH
  digitalWrite(direction2, LOW);

  // Attach interrupts to both rising and falling edges of the encoder signals
  attachInterrupt(digitalPinToInterrupt(encoderPinA), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), updateEncoder, CHANGE);

  attachInterrupt(digitalPinToInterrupt(encoderPin2A), updateEncoderR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPin2B), updateEncoderR, CHANGE);
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
  setPoint = 400; // Placeholder for conversion from meters to ticks
  setPointR = 400;
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
  analogWrite(motorPinA, output);
  analogWrite(motorPin2A, outputR);
  prev_time=current_time;
  prev_counter=counter;
  prev_counterR=counterR;
  Serial.print("encoderCounta: ");
  Serial.println(counterR); 

  if(digitalRead(12) == HIGH){
  while(digitalRead(12) == HIGH){
    analogWrite(motorPinA, 0);
    analogWrite(motorPin2A, 0);
  
    delay(4000);
   digitalWrite(direction2, LOW);
   int start_time= current_time;
   while((millis()-start_time)<9000)
   {
      analogWrite(motorPinA, 250);
      analogWrite(motorPin2A, 0);
   }
   analogWrite(motorPinA, 0);
    analogWrite(motorPin2A, 0);
    digitalWrite(direction, LOW);
   delay(2000);
  }
  }
  if ((current_time-timetostop)>102000){
     while (1){
       analogWrite(motorPinA, 0);
       analogWrite(motorPin2A, 0);
     }
   // Check if the target distance has been reached
   }
}
