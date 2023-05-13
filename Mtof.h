int Mtof(){
  digitalWrite(MTOF_PIN, LOW);
  delay(5);
  Wire.beginTransmission(ADDRESS);
  Wire.write(0xD3);
  Wire.endTransmission(false);
  Wire.requestFrom(ADDRESS, 2);
  data_cnt = 0;
  distance_m = 0;
  distance_tmp = 0;
  while(Wire.available())
  {
    distance_tmp = Wire.read();
    distance_m = (distance_m << (data_cnt*8)) | distance_tmp;
    data_cnt++;
  }
#if 0 
/*--------------------
ESP-WROOM-02用delay

[explanation]
100msec以上待たないと正常にデータが取得できない。
--------------------*/ 
  delay(500);     
#endif

  distance_m = distance_m / 10;
  digitalWrite(MTOF_PIN, HIGH);
  return distance_m;
}

/*---------------------
外部から呼び込み用の関数
または
マスターと1対1での通信用関数
---------------------*/
int readDistance(byte reg){
  delay(10);
  Wire.beginTransmission(ADDRESS);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(ADDRESS, 2);
  data_cnt = 0;
  distance_m = 0;
  distance_tmp = 0;
  while(Wire.available())
  {
    distance_tmp = Wire.read();
    distance_m = (distance_m << (data_cnt*8)) | distance_tmp;
    data_cnt++;
  return distance_m;
  }
  
}
