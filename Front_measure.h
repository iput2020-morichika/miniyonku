
float Front_measure() {

  digitalWrite(TRIG, LOW); 
  delayMicroseconds(2); 
  digitalWrite( TRIG, HIGH );
  delayMicroseconds( 10 ); 
  digitalWrite( TRIG, LOW );
  duration = pulseIn( ECHO, HIGH ); // 往復にかかった時間が返却される[マイクロ秒]

  if (duration > 0) {
    duration = duration / 2; // 往路にかかった時間
    distance = duration * speed_of_sound * 100 / 1000000;
  }
   return distance;
}
//  double duration = 0;
//  double speed_of_sound = 331.5 + 0.6 * 20; // 25℃の気温の想定
//  
//  digitalWrite(TRIG, LOW); 
//  delayMicroseconds(2); 
//  digitalWrite( TRIG, HIGH );
//  delayMicroseconds( 10 ); 
//  digitalWrite( TRIG, LOW );
//  duration = pulseIn( ECHO, HIGH ); // 往復にかかった時間が返却される[マイクロ秒]
//
//  if (duration > 0) {
//    duration = duration / 2; // 往路にかかった時間
//    distance = duration / speed_of_sound * 100 / 1000000;
//  }
//  return distance;



//
//unsigned short int Front_measure() {
//  double duration = 0;
//  double distance = 0;
// double speed_of_sound = 331.5 + 0.6 * 25; // 25℃の気温の想定
// 
//  digitalWrite(TRIG, LOW); 
//  delayMicroseconds(2); 
//  digitalWrite( TRIG, HIGH );
//  delayMicroseconds( 10 ); 
//  digitalWrite( TRIG, LOW );
//  duration = pulseIn( ECHO, HIGH ); // 往復にかかった時間が返却される[マイクロ秒]
//
//  if (duration > 0) {
//    duration = duration / 2; // 往路にかかった時間
//    distance = duration * speed_of_sound * 100 / 1000000;
//  }
//  return distance;
//}
