#include <ECE3.h> // Used for encoder functionality

//Global Variables

uint16_t sensorValues[8];

  int leftBaseSpeed = 50;
  int rightBaseSpeed = 50;

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

  // Encoder Variables
long enc_bin_cnt;
const unsigned long enc_bin_len = 50; // 50 ms bins
    // Encoder Speed Calculation Explanation:
    // We wait for a set amount of time (enc_bin_len), and find how many
    // times the encoder has incremented in that period. We call 
    // this period a bin when refering to the encoder. The number 
    // encoder counts per bin is a proportional to speed.


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

  ECE3_Init();

  //reseting Encoder Counts
  resetEncoderCount_left();
  resetEncoderCount_right();

  digitalWrite(nSlpRight, HIGH);  
  digitalWrite(Dir_R, LOW);

  Serial.begin(19200);
}

void loop() {
  ECE3_read_IR(sensorValues);
  
  // analogWrite(PWMLeft, leftBaseSpeed);
  // analogWrite(PWMRight, rightBaseSpeed);


  float error = 0;

  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] < min[i]) {
      error += 0;
    } else {
      error += weights[i] * (1000.0 * (sensorValues[i] - min[i]) / (max[i]));
    }
      
  }
  error = error / 8;
  
  //Serial.println(error);
  float Kp = -0.0125;
  float Kd = -0.0125;

  int leftSpeed;
  int rightSpeed;

  //Calculate PID
  float PIDsum = (Kp * error) + (Kd * (error - prevError));

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
  average();
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
