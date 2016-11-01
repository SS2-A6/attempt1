#define TIME 2000
#define maxWeight 40
#define minWeight 40

inline void calibration(unsigned int *high_, unsigned int *low_){
  int cab_max = 0;
  int cab_min = 0;
  long cab_time = 0;
  int tmp = 0;
  
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
  int median = (cab_max + cab_min)/2;
  *high_ = (cab_max - median)*maxWeight/100 + median;
  *low_  = ((cab_min - median)*minWeight/100 + median);
  printf("median %d, %d\n", median, (cab_min - median)*minWeight/100);

  Back(150);
  delay(TIME);
  Stop();
  
  return;
}

