Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(MPU6050_AX);
  Wire.endTransmission();  
  //  request 14bytes (int16 x 7)
  Wire.requestFrom(MPU6050_ADDR, 14);
  //  get 14bytes  

  int i;
  short int Ax, Ay, Az, Gx, Gy, Gz;
  
   
  for (i=0; i>=1000; i++){
    AccX = Wire.read() << 8;  AccX |= Wire.read();
    AccY = Wire.read() << 8;  AccY |= Wire.read();
    AccZ = Wire.read() << 8;  AccZ |= Wire.read();
    Temp = Wire.read() << 8;  Temp |= Wire.read();  //  (Temp-12421)/340.0 [degC]
    GyroX = Wire.read() << 8; GyroX |= Wire.read();
    GyroY = Wire.read() << 8; GyroY |= Wire.read();
    GyroZ = Wire.read() << 8; GyroZ |= Wire.read();

    Ax += AccX;
    Ay += AccY;
    Az += AccZ;
    
    Gx += GyroX;
    Gy += GyroY;
    Gz += GyroZ;
  }

   offset_Ax = (Ax / 1000);
   offset_Ay = (Ay / 1000);
   offset_Az = (Az / 1000);
   
   offset_Gx = (Gx / 1000);
   offset_Gy = (Gy / 1000);
   offset_Gz = (Gz / 1000);
