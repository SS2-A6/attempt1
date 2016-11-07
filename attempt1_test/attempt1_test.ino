#include <stdio.h>
#include <MsTimer2.h>

static FILE uartout;

static int uart_putchar (char c, FILE *stream) {
  if (Serial.write(c) > 0) {
    return 0;
  } else {
    return -1;
  }
}

#define LED 2
unsigned int high1 = 0;
unsigned int low1 = 0;
unsigned int high2 = 0;
unsigned int low2 = 0;
unsigned int thres = 630;
byte flag1 = 0;  // encoder1が，1:枕木上，0:枕木間
long count = 0;
word val1 = 0;
word val2 = 0;
byte pri = 0;

//#define Debug
#define Forward(x) analogWrite(5, x); analogWrite(6, 0);
#define Back(x) analogWrite(5, 0); analogWrite(6, x);
#define Stop() analogWrite(5, 0); analogWrite(6, 0);

#ifndef Debug



// セットアップ関数
void setup() {
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(2,INPUT_PULLUP);

  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout;

  //printf("calibration start\n");
  //calibration ( &high1, &low1, &high2, &low2 );
  //printf("calibration end high1:%d, low1:%d\n", high1, low1);
  //printf("calibration end high2:%d, low2:%d\n", high2, low2);

  printf("Hello!\n");
  analogWrite(5, 255);
  analogWrite(6, 255);

  // タイマ割り込み設定
  //MsTimer2::set(10, encoder);
  //MsTimer2::start();

  while ( digitalRead(2) ) { }

  printf("start!\n");
}

//#define Encoder

// エンコーダ関数 (位相差を利用した，前進or後進判定機能付)
void encoder() {
#ifdef Encoder
  val1 = analogRead(0);
  val2 = analogRead(1);

  if ( val1 >= high1 ) {
    if ( flag1 == 1 ) {
      printf("V 0 %d %d\n", val1, val2);
    }
    flag1 = 0;
  }
  else if ( val1 <= low1 ) {
    if (flag1 == 0) {
      printf("^ 1 %d %d %d\n", val1, val2, count);
      // count++;
      if ( val2 >= high2 ) {
        printf("Moving F \n");
        count++;
      }
      else if ( val2 <= low2 ) {
        printf("Moving B \n");
        count--;
      }
    }
    flag1 = 1;
  }

#endif
}


#define final_screen_count 12  // スクリーンの最後の個数
#define stop_screen_count 13
byte screen_flag = 0;  //0：スクリーンなし，1：スクリーンあり
byte before_screen_flag = 0;
byte screen_count = 0;
unsigned int val3 = 0;
unsigned int val3_add = 0;
unsigned int val3_count = 0;
unsigned int val3_ave = 0;
long final_rail_count = 180000;  // 最終スクリーンまでの累積時間
long rest_rail_count = 180000;  // 残りの予想秒数を算出
long past_TIME;
byte stop_screen_flag = 0;

// PSDセンサでスクリーン検知・各種処理を行う関数
inline void psd_screen() {

  val3 = analogRead(2);
  val3_count ++;

  // スクリーンの有無判定 (最新5データの平均値で判定)
  if ( (val3_count % 5) != 0 ) {
    val3_add = val3_add + val3;
  }
  else {
    val3_add = val3_add + val3;
    val3_ave = val3_add / 5;
    if ( val3_ave > 500 ) {
      screen_flag = 1;
    }
    else {
      screen_flag = 0;
    }
    val3_add = 0;
  }

  // スクリーンの端判定，スクリーンカウント++，最終スクリーン(12個目)通過判定，残りの枕木数推定
  if ( before_screen_flag == 1 && screen_flag == 0 ) {
    screen_count ++;
    printf("screen out %d\n", screen_count);
    if ( screen_count == final_screen_count ) {
      rest_rail_count = final_rail_cal ( final_screen_count, past_TIME );
      final_rail_count = past_TIME;
      printf("rest_rail = %ld time = %ld\n", rest_rail_count, past_TIME );
    }
    if ( screen_count == stop_screen_count ) {
      stop_screen_flag = 1;
    }
  }
  else if ( before_screen_flag == 0 && screen_flag == 1 ) {
    printf("screen in %d\n", screen_count);
    delay(500);
  }
  before_screen_flag = screen_flag;

}



long final_rail_cal ( byte final_screen_count_, long count_ ) {
  switch ( final_screen_count_ ) {
    case 2:
      return (35 - 3 * 1) * count_ / (3 * 1);
    case 4:
      return (35 - 3 * 2) * count_ / (3 * 2);
    case 6:
      return (35 - 3 * 5) * count_ / (3 * 5);
    case 8:
      return (35 - 3 * 6) * count_ / (3 * 6);
    case 10:
      return (35 - 3 * 9) * count_ / (3 * 9);
    case 12:
      return (35 - 3 * 10) * count_ / (3 * 10);
  }
}



byte enemy_flag = 0;  //0：相手が隣にいない，1：相手が隣にいる
byte before_enemy_flag = 0;
byte enemy_state = 0;  //0：相手が未通過，1：相手が通過済み
unsigned int val4 = 0;
unsigned int val4_add = 0;
unsigned int val4_count = 0;
unsigned int val4_ave = 0;

// PSDセンサで相手検知・各種処理を行う関数
inline void psd_enemy() {

  val4 = analogRead(3);
  val4_count ++;

  // 相手が隣にいるかどうかどうかの判定
  if ( (val4_count % 5) != 0 ) {
    val4_add = val4_add + val4;
  }
  else {
    val4_add = val4_add + val4;
    val4_ave = val4_add / 5;
    if ( val4_ave > 450 ) {
      enemy_flag = 1;
    }
    else {
      enemy_flag = 0;
    }
    val4_add = 0;
  }

  // 相手との位置関係の計算
  if ( before_enemy_flag == 1 && enemy_flag == 0 ) {
    enemy_state = 1;
    before_enemy_flag = enemy_flag;
  }
  else {
    before_enemy_flag = enemy_flag;
  }

}




#define threeMIN 180000
#define deadTime 5000
long rest_TIME;  // 残り時間
long initial_TIME;  // 競技開始時間
byte first_loop_flag = 1;  // 1：loopの1回目を表す，0：それ以降
byte end_flag = 0;

// ループ関数 (メインの動作)
void loop() {

  // 競技開始時間を記録 (1度目のloopにて)
  if ( first_loop_flag == 1 ) {
    initial_TIME = millis();
    first_loop_flag = 0;
  }
  past_TIME = ( millis() - initial_TIME );
  rest_TIME = threeMIN - past_TIME;  // 残り時間を常に計算

  psd_screen();
  psd_enemy();

  // 残り時間に余裕があり，かつ，まだゴール直前まで到達していない場合，次の動作を実施．
  if ( (rest_TIME > deadTime) && (past_TIME < final_rail_count + rest_rail_count) ) {
    analogWrite(5, 255);
    analogWrite(6, 0);
  }

  // 残り時間に余裕があり，かつ，ゴール直前まで到達した場合，次の動作を実施．
  else if ( (rest_TIME > deadTime) && (past_TIME > final_rail_count + rest_rail_count) ) {

    printf("enemy_state %d rest time %ld\n", enemy_state, rest_TIME);
    // 相手が前方に通過したなら前進する．
    if ( enemy_state == 1 ) {
      if ( stop_screen_flag == 1 ) {
        analogWrite(5, 0);
        analogWrite(6, 255);
        delay(1000);
        stop_screen_flag = 0;
        end_flag = 1;
      }
      else if ( end_flag == 1 ) {
        analogWrite(5, 0);
        analogWrite(6, 0);
      }
      else {
        analogWrite(5, 200);
        analogWrite(6, 0);
      }
    }
    // 相手が通過していないなら，停止し続ける．(時間が来るまで)
    else {
      analogWrite(5, 0);
      analogWrite(6, 0);
    }
  }

  // 残り時間少なければとにかく前進．13個目のついたて検知で後進&停止．
  if (rest_TIME < deadTime) {
    printf("!danger! stopflag = %d, end_flag = %d\n", stop_screen_flag, end_flag);
    if ( stop_screen_flag == 1 ) {
      analogWrite(5, 0);
      analogWrite(6, 255);
      stop_screen_flag = 0;
      end_flag = 1;
      delay(1000);
    }
    else if ( end_flag == 1 ) {
      analogWrite(5, 0);
      analogWrite(6, 0);
    }
    else {
      analogWrite(5, 255);
      analogWrite(6, 0);
    }

  }

}



#else
byte flag = 0;
void setup() {
  Serial.begin(9600);
  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout;
  analogWrite(5, 255);
  analogWrite(6, 255);
}
void loop() {
  printf("%d\t%d\n", analogRead(0), analogRead(1));
  /*
    val1 = analogRead(0);
    val2 = analogRead(1);

    if ( val1 >= 350 ) {
      if ( flag1 == 1 ) {
        printf("0\t%d\n", val2 > 350 ? 0 : 100);
      }
      flag1 = 0;
    }
    else if ( val1 <= 350 ) {
      if (flag1 == 0) {
        printf("100\t%d\n", val2 > 350 ? 0 : 100);
        // count++;
        if( val2 >= high2 ) {
          //printf("Moving F \n");
          count++;
        }
        else if( val2 <= low2 ) {
          //printf("Moving B \n");
          count--;
        }
      }
      flag1 = 1;
    }*/
}
#endif
