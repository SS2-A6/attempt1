#define TIME 2000
#define maxWeight1 20
#define minWeight1 20
#define maxWeight2 5 //-20
#define minWeight2 5 //45
#define offset1 (0)
#define offset2 (-0)

inline void calibration( unsigned int *high1_, unsigned int *low1_, unsigned int *high2_, unsigned int *low2_ ){
  int cab_max1 = 0;
  int cab_min1 = 0;
  int cab_max2 = 0;
  int cab_min2 = 0;
  long cab_time = 0;
  int tmp1 = 0;
  int tmp2 = 0;
  long cab_count = 0;
  long cab_add1 = 0;
  long cab_add2 = 0;
  
  cab_time = millis();
  cab_max1 = analogRead(0);
  cab_min1 = analogRead(0);
  cab_max2 = analogRead(1);
  cab_min2 = analogRead(1);

  Forward(150);
  while (  (millis()-cab_time) < TIME  ) {
    tmp1 = analogRead(0);
    tmp2 = analogRead(1);
    cab_add1 = cab_add1 + tmp1;
    cab_add2 = cab_add2 + tmp2;
    cab_count ++;
    
    if ( tmp1 > cab_max1 ) {
      cab_max1 = tmp1;
    }   
    if ( tmp1 < cab_min1 ) {
      cab_min1 = tmp1;
    }
    if ( tmp2 > cab_max2 ) {
      cab_max2 = tmp2;
    }
    if ( tmp2 < cab_min2 ) {
      cab_min2 = tmp2;
    }
  
  }

  printf("max %u, min %u, sum %u\n", cab_max1, cab_min1, cab_add1);
  printf("max %u, min %u, sum %u\n", cab_max2, cab_min2, cab_add2);
  int median1 = cab_add1 / cab_count;
  int median2 = cab_add2 / cab_count;
  *high1_ = (cab_max1 - median1)*maxWeight1/100 + median1 + offset1;
  *low1_  = (cab_min1 - median1)*minWeight1/100 + median1 + offset1;
  *high2_ = (cab_max2 - median2)*maxWeight2/100 + median2 + offset2;
  *low2_  = (cab_min2 - median2)*minWeight2/100 + median2 + offset2;
  printf("median1 %d, %d\n", median1, (cab_min1 - median1)*minWeight1/100);
  printf("median2 %d, %d\n", median2, (cab_min2 - median2)*minWeight2/100);

  Back(150);
  delay(TIME);
  Stop();
  
  return;
}

