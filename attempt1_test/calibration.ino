#define TIME 2000
inline void calibration(unsigned int *high_, unsigned int *low_){
  unsigned int cab_max = 0;
  unsigned int cab_min = 0;
  unsigned long cab_time = 0;
  unsigned int tmp = 0;
  
  cab_time = millis();
  cab_max = analogRead(0);
  cab_min = analogRead(0);

  Forward(150);
  while (  (millis()-cab_time) < TIME  ) {
    tmp = analogRead(0);
    
    if ( tmp > cab_max ) {
      cab_max = tmp;
    }
    
    if ( tmp < cab_min ) {
      cab_min = tmp;
    }
  
  }

  printf("max %u, min %u\n", cab_max, cab_min);
  // FIX ME 重み付きでできるといい?
  *high_ = cab_max - ( cab_max - cab_min ) / 3;
  *low_ = cab_min + ( cab_max - cab_min ) / 3;

  Back(150);
  delay(TIME);
  Stop();
  
  return;
}

