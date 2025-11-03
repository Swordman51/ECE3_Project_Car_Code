void setup() {
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

  uint16_t sensorValues[8];
}

void loop() {
  ECE3_read_IR(sensorValues);
  
  analog.Write(nSlpLeft, HIGH);
  analog.Write(nSlpRight, HIGH);
  analog.Write(PWMLeft, HIGH);
  analog.Write(PWMRight, HIGH);

  analog.Write(Dir_L, HIGH);
  analog.Write(Dir_R, HIGH);

}
