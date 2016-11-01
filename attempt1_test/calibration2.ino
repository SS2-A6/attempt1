#define TIME1 1000
#define TIME2 4000

void calibration2( unsigned int *high_, unsigned int *low_ ) {
  unsigned long cab_time = 0;
  unsigned int tmp = 0;
  unsigned long cab_add1 = 0;
  unsigned long cab_count1 = 0;
  unsigned int cab_half = 0;
  unsigned long cab_count2_high = 0;
  unsigned long cab_count2_low = 0;
  unsigned long cab_add2_high = 0;
  unsigned long cab_add2_low = 0;

  cab_time = millis();
  while ( (millis() - cab_time) < TIME1 ) {
    tmp = analogRead(0);
    cab_add1 = cab_add1 + tmp;
    cab_count1 ++;
  }

  printf( "%u\n", cab_add1 );
  printf("%u\n", cab_count1);
  cab_half = cab_add1 / cab_count1;
  printf("%u\n", cab_half);

  cab_time = millis();  
  while ( (millis() - cab_time) < TIME2 ) {
    tmp = analogRead(0);
    if ( tmp > cab_half ) {
      cab_add2_high = cab_add2_high + tmp;
      cab_count2_high ++;
    }
    if ( tmp < cab_half ) {
      cab_add2_low = cab_add2_low + tmp;
      cab_count2_low ++;
    }
  }

  *high_ = cab_add2_high / cab_count2_high - 10;
  *low_ = cab_add2_low / cab_count2_low + 10;

}
