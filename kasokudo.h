float Kasokudo() {
  //  send start address
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_AX);
  Wire.endTransmission();  
  //  request 14bytes (int16 x 7)
  Wire.requestFrom(MPU6050_ADDR, 14);
  //  get 14bytes
  AccX = Wire.read() << 8;  AccX |= Wire.read();
  AccY = Wire.read() << 8;  AccY |= Wire.read();
  AccZ = Wire.read() << 8;  AccZ |= Wire.read();
  Temp = Wire.read() << 8;  Temp |= Wire.read();  //  (Temp-12421)/340.0 [degC]
  GyroX = Wire.read() << 8; GyroX |= Wire.read();
  GyroY = Wire.read() << 8; GyroY |= Wire.read();
  GyroZ = Wire.read() << 8; GyroZ |= Wire.read();

  /*キャリブレーション*/
  AccX_c = AccX - offset_Ax;
  AccY_c = AccY - offset_Ay;
  AccZ_c = AccZ - offset_Az;
  GyroX_c = GyroX - offset_Gx;
  GyroY_c = GyroY - offset_Gy;
  GyroZ_c = GyroZ - offset_Gz;
  
  /*生データを実データへ変換*/
  float LSB_g = 16384.0;
  float g = 9.81; 
  float  LSB_d = 131.0;

  /*加速度値を分解能で割って加速度(G)に変換する*/
  float acc_x = (AccX_c / LSB_g) * g;     //加速度
  float acc_y = (AccY_c / LSB_g) * g;     //gx = data / 131
  float acc_z = (AccZ_c / LSB_g) * g;
  /*角速度値を分解能で割って角速度(degrees per sec)に変換する*/
  float gyro_x = GyroX / LSB_d;         //角速度
  float gyro_y = GyroY / LSB_d;         //gx = data / 131
  float gyro_z = GyroZ / LSB_d;
  
  //  debug monitor
  Serial.print("acceleration_X："); Serial.print(acc_x);
  Serial.print(" acceleration_Y："); Serial.print(acc_y);
  Serial.print(" acceleration_Z："); Serial.println(acc_z);
  Serial.print("angle_X："); Serial.print(gyro_x);
  Serial.print( "angle_y："); Serial.print(gyro_y);
  Serial.print(" angle_Z："); Serial.println(gyro_z); 
  //Serial.println("");
}

//  Wire.beginTransmission(MPU6050_ADDR);
//  Wire.write(MPU6050_AX);
//  Wire.endTransmission();  
//  //  request 14bytes (int16 x 7)
//  Wire.requestFrom(MPU6050_ADDR, 14);
//  //  get 14bytes
//  AccX = Wire.read() << 8;  AccX |= Wire.read();
//  AccY = Wire.read() << 8;  AccY |= Wire.read();
//  AccZ = Wire.read() << 8;  AccZ |= Wire.read();
//  Temp = Wire.read() << 8;  Temp |= Wire.read();  //  (Temp-12421)/340.0 [degC]
//  GyroX = Wire.read() << 8; GyroX |= Wire.read();
//  GyroY = Wire.read() << 8; GyroY |= Wire.read();
//  GyroZ = Wire.read() << 8; GyroZ |= Wire.read();
//
//  /*キャリブレーション*/
//  AccX_c = AccX - offset_Ax;
//  AccY_c = AccY - offset_Ay;
//  AccZ_c = AccZ - offset_Az;
//  GyroX_c = GyroX - offset_Gx;
//  GyroY_c = GyroY - offset_Gy;
//  GyroZ_c = GyroZ - offset_Gz;
//  
//  /*生データを実データへ変換*/
//  float LSB_g = 16384.0;
//  float g = 9.81; 
//  float  LSB_d = 131.0;
//
//  /*加速度値を分解能で割って加速度(G)に変換する*/
//  float acc_x = (AccX_c / LSB_g) * g;     //加速度
//  float acc_y = (AccY_c / LSB_g) * g;     //gx = data / 131
//  float acc_z = (AccZ_c / LSB_g) * g;
//  /*角速度値を分解能で割って角速度(degrees per sec)に変換する*/
//  float gyro_x = GyroX / LSB_d;         //角速度
//  float gyro_y = GyroY / LSB_d;         //gx = data / 131
//  float gyro_z = GyroZ / LSB_d;
//  
//  //  debug monitor
//
//  //Serial.println("");
//  delay(20);
//  
//    Serial.println("///////////////////////////////////////////////////////");
//    Serial.println(" ");
//    Serial.println("------------コントロール--------------------------------------");
//    Serial.println(" ");
//    Serial.print("モータ:");
//    Serial.print(duty, HEX);
//    Serial.print(" サーボ:");
//    Serial.print(angle);
//    Serial.print(" フロント:");
//    Serial.print(Front);
//    Serial.print("cm ");
//    Serial.print(" 右サイド:");
//    Serial.print(R_side);
//    Serial.print("cm ");
//    Serial.print(" 左サイド:");
//    Serial.print(L_side);
//    Serial.println("cm");
//    Serial.println(" ");
//    Serial.println("------------加速度--------------------------------------");
//    Serial.println(" ");
//    //Serial.println(Kasokudo() );
//      Serial.print("加速度_X："); Serial.print(acc_x);
//    Serial.print(" 加速度_Y："); Serial.print(acc_y);
//    Serial.print(" 加速度_Z："); Serial.println(acc_z);
//    Serial.print("角速度_X："); Serial.print(gyro_x);
//    Serial.print(" 角速度_y："); Serial.print(gyro_y);
//    Serial.print(" 角速度_Z："); Serial.println(gyro_z); 
//    Serial.println(" ");
//    Serial.println("///////////////////////////////////////////////////////");
