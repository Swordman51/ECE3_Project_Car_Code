#include <ECE3.h> // Used for encoder functionality


#define	IMPLEMENTATION	LIFO

//Global Variables

uint16_t sensorValues[8];

  int leftBaseSpeed = 20;
  int rightBaseSpeed = 20;

  float maxError = 2483.675;
  float prevError = 0.0;

  float min[] = {779,	687, 596,	664, 619,	687, 642,	756}; //change if recalibrated
  float max[] = {1721, 1813, 1904, 1836, 1881, 1813, 1858, 1744};  //change if recalibrated
  int weights[] = {-15, -14, -12, -8, 8, 12, 14, 15};

  //No Sleep Pins
  int nSlpLeft = 31;
  int nSlpRight = 11;

  //PWM Pins
  int PWMLeft = 40;
  int PWMRight = 39;

  //Direction Pins
  int Dir_L = 29;
  int Dir_R = 30;

  //Phantom Crosspiece values
  int prevCount;
  int currentCountIndex = 0;

  // Encoder Variables
long enc_bin_cnt;
const unsigned long enc_bin_len = 50; // 50 ms bins
    // Encoder Speed Calculation Explanation:
    // We wait for a set amount of time (enc_bin_len), and find how many
    // times the encoder has incremented in that period. We call 
    // this period a bin when refering to the encoder. The number 
    // encoder counts per bin is a proportional to speed.

int crossPieceCount = 0;

void setup() {

  //initialize left pins
  pinMode(nSlpLeft, OUTPUT);
  pinMode(Dir_L, OUTPUT);
  pinMode(PWMLeft, OUTPUT);

  //initialize right pins
  pinMode(nSlpRight, OUTPUT);
  pinMode(Dir_R, OUTPUT);
  pinMode(PWMRight, OUTPUT);

  //set pins inital Direction, CHANGE AS NEEDED
  digitalWrite(nSlpLeft, HIGH);
  digitalWrite(Dir_L, LOW);
  digitalWrite(nSlpRight, HIGH);  
  digitalWrite(Dir_R, LOW);

  ECE3_Init();

  //reseting Encoder Counts
  resetEncoderCount_left();
  resetEncoderCount_right();



  Serial.begin(19200);
}  

void loop() {
  
  ECE3_read_IR(sensorValues);
  
  int count = 0;
  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] >= 2000) {
      count++;
    }
  }
  

  float error = 0;

  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] < min[i]) {
      error += 0;
    } else {
      error += weights[i] * (1000.0 * (sensorValues[i] - min[i]) / (max[i]));
    }
    //Serial.print(sensorValues[i]);
    //Serial.print("    ");
   

  }
  //Serial.print("\n");
  error = error / 8;
  
  //Serial.println(error);
  float Kp = -0.0125;
  float Kd = -0.0125;

  int leftSpeed;
  int rightSpeed;

  //Calculate PID
  float PIDsum = (Kp * error) + (Kd * (error - prevError));

  // Crosspiece + Phantom crosspiece

  if (count >= 6) {
    bool crossencountered = true;
    // if (prevCount != 8) {
    //   crossencountered = false;
    // } 
    if (crossencountered){
      crossPieceCount++;
      //Serial.print(crossPieceCount);
      //if (crossPieceCount == 1) {
        turnCar();
        reset();
      //}

    }
  }

  if (leftBaseSpeed + PIDsum < 0) {
    leftSpeed = 0;
  } else {
    leftSpeed = leftBaseSpeed + PIDsum;
  }

  if (rightBaseSpeed - PIDsum < 0) {
    rightSpeed = 0;
  } else {
    rightSpeed = rightBaseSpeed - PIDsum;
  }
  //Serial.println(leftSpeed);
  //Serial.println(rightSpeed);
  //average();
  analogWrite(PWMLeft, leftSpeed);
  analogWrite(PWMRight, rightSpeed);
  prevError = error;
}

int average() {
  int getL = getEncoderCount_left();
  int getR = getEncoderCount_right();
    Serial.print(getL);Serial.print("\t");Serial.print(getR);Serial.print("\n");
  return ((getEncoderCount_left() + getEncoderCount_right()) / 2);
}

void reset() {
  //set pins inital Direction, CHANGE AS NEEDED
  digitalWrite(nSlpLeft, HIGH);
  digitalWrite(Dir_L, LOW);
  digitalWrite(nSlpRight, HIGH);  
  digitalWrite(Dir_R, LOW);
}

void turnCar() {
  //stop for now
  analogWrite(PWMLeft, 0);
  analogWrite(PWMRight, 0);
  delay(1000); //get rid

  //reseting Encoder Counts
  resetEncoderCount_left();
  resetEncoderCount_right();

  //switch direction
  digitalWrite(Dir_R, HIGH);

  int getL = getEncoderCount_left();
  int getR = getEncoderCount_right();

  while (getL < 772 && getR < -700) {
    analogWrite(PWMLeft, 20);
    analogWrite(PWMRight, 20);
  }

  delay(10000);

  // while(true) {
  //   analogWrite(PWMLeft, 20);
  //   analogWrite(PWMRight, 20);
  //   average();
  // }
  //772 -700 is one full turn

  


  
  

  
}
