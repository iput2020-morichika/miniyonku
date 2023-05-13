int Right_measure(){
     int  measure  ;
     byte c[2] ;

     Wire.beginTransmission(SENSOR_ADRS) ;        // 通信の開始処理
     Wire.write(DISTANCE_ADRS) ;                  // 距離値を格納したテーブルのアドレスを指定する
     measure = Wire.endTransmission() ;               // データの送信と終了処理
     if (measure == 0) {
          measure = Wire.requestFrom(SENSOR_ADRS,2) ; // GP2Y0E03にデータ送信要求をだし、2バイト受信する
          c[0] = Wire.read()  ;                   // データの11-4ビット目を読み出す
          c[1] = Wire.read()  ;                   // データの 3-0ビット目を読み出す
          measure = ((c[0]*16+c[1]) / 16) / 4 ;       // 取り出した値から距離(cm)を計算する
          if (measure >= 63){
            measure = 8888;
          }
//          Serial.print(measure) ;                     // シリアルモニターに表示させる
//          Serial.println("cm") ;
     } else {
//          Serial.print("ERROR NO.=") ;            // GP2Y0E03と通信出来ない
//          Serial.println(measure) ;
     }

     return measure;
}
