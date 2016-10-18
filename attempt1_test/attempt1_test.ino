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
byte led = 1;
unsigned int thres = 630;
byte flag = 0;
unsigned int count = 0;
word val = 0;
byte before = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED, OUTPUT);

  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout;

  calibration( &high, &low );
  printf("%d, %d\n", high, low);

  while(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  //  Serial.println(analogRead(0));
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
}
