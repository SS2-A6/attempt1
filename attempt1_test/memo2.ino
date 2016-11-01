unsigned int countFirst = 0;
byte flagFirst = 1;

// 枕木（makuragi）の数だけ前進する関数
void MoveF( unsigned int makuragi ) {

  if( flagFirst == 1 ) {
    countFirst = count;
    flagFirst = 0;
  }
  
  if ( (count-countFirst) > makuragi ) {
    
      analogWrite(5, 0);
      analogWrite(6, 0);
      flagFirst = 1;
      printf("one rail\n");
      
  } else {
    
    analogWrite(5, 255);
    analogWrite(6, 0);
    flagFirst = 0;
  
  }

}

