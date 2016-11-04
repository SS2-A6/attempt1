/*#define TIME 2000
#define maxWeight1 40
#define minWeight1 40
#define maxWeight2 0
#define minWeight2 40*/

//  キャリブレーション開始直後の値を信用しない．cab_delay_TIMEだけ遅らせて，計測値を捨てる．
//  あとはcalibrationと同じ
inline void calibration3 ( unsigned int *high1_, unsigned int *low1_, unsigned int *high2_, unsigned int *low2_ ) {
  int cab_max1 = 0;
  int cab_min1 = 0;
  int cab_max2 = 0;
  int cab_min2 = 0;
  long cab_time = 0;
  int tmp1 = 0;
  int tmp2 = 0;
  long cab_delay_TIME = 200;  // 少し遅らせて(ms)読み込む．最初の計測値を捨てる．

  cab_time = millis();
  cab_max1 = analogRead(0);
  cab_min1 = analogRead(0);
  cab_max2 = analogRead(1);
  cab_min2 = analogRead(1);

  Forward(150);
  while (  (millis() - cab_time) < TIME  ) {

    tmp1 = analogRead(0);
    tmp2 = analogRead(1);

    if ( (millis() - cab_time) > cab_delay_TIME ) {
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

  }

  printf("max %u, min %u\n", cab_max1, cab_min1);
  printf("max %u, min %u\n", cab_max2, cab_min2);
  int median1 = ( cab_max1 + cab_min1 ) / 2;
  int median2 = ( cab_max2 + cab_min2 ) / 2;
  *high1_ = (cab_max1 - median1) * maxWeight1 / 100 + median1;
  *low1_  = (cab_min1 - median1) * minWeight1 / 100 + median1;
  *high2_ = (cab_max2 - median2) * maxWeight2 / 100 + median2;
  *low2_  = (cab_min2 - median2) * minWeight2 / 100 + median2;
  printf("median1 %d, %d\n", median1, (cab_min1 - median1)*minWeight1 / 100);
  printf("median2 %d, %d\n", median2, (cab_min2 - median2)*minWeight2 / 100);

  Back(150);
  delay(TIME);
  Stop();

  return;
}
