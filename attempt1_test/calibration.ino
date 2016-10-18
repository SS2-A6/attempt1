void calibration(unsigned int *high_, unsigned int *low_){
  unsigned int cab_max = 0;
  unsigned int cab_min = 0;
  unsigned long cab_time = 0;
  unsigned int tmp = 0;
  
  cab_time = millis();
  cab_max = analogRead(0);
  cab_min = analogRead(0);
  
  while (  (millis()-cab_time) < 1000  ) {
    tmp = analogRead(0);
    
    if ( tmp > cab_max ) {
      cab_max = tmp;
    }
    
    if ( tmp < cab_min ) {
      cab_min = tmp;
    }
  
  }

  printf("max %u, min %u\n", cab_max, cab_min);
  *high_ = cab_max - ( cab_max - cab_min ) / 3;
  *low_ = cab_min + ( cab_max - cab_min ) / 3;
  
  return;
}

