unsigned int countFirst = 0;  // moveFを始めた瞬間のcountを記憶
byte flagFirst = 1;  // 0:前進未完，1:前進完了

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
      
  } else {
    
    analogWrite(5, 255);
    analogWrite(6, 0);
    flagFirst = 0;
  
  }

}

