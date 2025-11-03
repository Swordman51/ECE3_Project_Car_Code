#include <ECE3.h> // Used for encoder functionality

//Global Variables

  int leftBaseSpeed = 50;
  int rightBaseSpeed = 50;

  int min[] = {779,	687, 596,	664, 619,	687, 642,	756}; //change if recalibrated
  int max[] = {1721, 1813, 1904, 1836, 1881, 1813, 1858, 1744};  //change if recalibrated
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
  uint16_t sensorValues[8];

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


  digitalWrite(nSlpRight, HIGH);  
  digitalWrite(Dir_R, LOW);

  Serial.begin(19200);
}

void loop() {
  //ECE3_read_IR(sensorValues);
  
  analogWrite(PWMLeft, leftBaseSpeed);
  analogWrite(PWMRight, rightBaseSpeed);



  int sum = 0;

  for (i = 0; i < 7; i++) {
    if (sensorValues[i] < min)
      min = sensorValues[i];
    sum += Weight[i] * ((sensorValues[i] - min[i]) / (max[i] - min[i])) * 1000;
}
