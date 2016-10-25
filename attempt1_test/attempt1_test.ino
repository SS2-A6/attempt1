#include <stdio.h>
 
static FILE uartout;
 
static int uart_putchar (char c, FILE *stream) {
    if (Serial.write(c) > 0) {
      return 0;
    } else {
      return -1;
    }
}

#define LED 2
unsigned int high = 0;
unsigned int low = 0;
unsigned int thres = 630;
byte flag = 0;
unsigned int count = 0;
word val = 0;
byte before = 0;
byte pri = 0;

#define Debug

#ifndef Debug

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout;

  printf("calibration start\n");
  calibration( &high, &low );
  printf("calibration end high:%d, low:%d\n", high, low);

  analogWrite(5, 255);
  analogWrite(6, 0);

}

void loop() {
  // put your main code here, to run repeatedly:
  val =analogRead(0);
  //printf("%d\n", val);
  if ( val >= high) {
    if ( flag == 1 ){
        printf("V 0 %d\n", val);
        //count++;
    }    
    flag = 0;
  }
  else if ( val <= low ) {
    if (flag == 0){
        printf("^ 1 %d %d\n", val, count);
        count++;
    }
    flag = 1;
  }

  if (count % 32 == 0) {
    if (pri == 0) {
      pri = 1;
      analogWrite(5, 0);
      analogWrite(6, 0);
      printf("one rail\n");
      delay(1000);
    } 
  } else {
    pri = 0;
    analogWrite(5, 255);
    analogWrite(6, 0);
  }
}

#else
void setup(){
  Serial.begin(9600);
  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout;
}
void loop(){
  val =analogRead(0);
  printf("%d\n", val);
}
#endif
