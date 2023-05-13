/*** ヘッダーファイルインクルード↓ ***/
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <VL53L0X.h>
/*** ヘッダーファイルインクルード↑ ***/

VL53L0X sensor;

/*** サーバ,WiFi設定↓ ***/
const char *ssid = "MKZ4";
const char *password = "";

ESP8266WebServer server(80);
ESP8266WebServer server_8080(8080);
/*** サーバ,WiFi設定↑ ***/

/*** I2C設定（アドレス）↓ ***/
#include <Wire.h>
#define SENSOR_ADRS  0x40          //GP2Y0E03のI2Cアドレス
#define MPU6050_ADDR 0x68          //MPU6050用
#define ADDRESS      0x52          //mtof用アドレス
/*** I2C設定↑ ***/

/*<<----------------------
　サーボモータ
---------------------->>*/
#define ADDR1        0x64          //
#define COMMAND_START  0
#define COMMAND_STOP   1
#define COMMAND_BACK   2
#define FORWARD       0x01
#define RESERVE       0x02
#define SERVO_LEFT    65
#define SERVO_RIGHT   150


/*<<----------------------
　LED
----------------------->*/
#define LED_H  (digitalWrite( 12, HIGH ))
#define LED_L  (digitalWrite( 12, LOW ))


/*<<---------------------
　GP2Y0E03 
　赤外線距離センサ
--------------------->>*/
#define DISTANCE_ADRS 0x5E              // Distance Value のデータアドレス


/*<<---------------------
　MPU6050
　加速度＆角速度計測センサ
--------------------->>*/
#define MPU6050_AX  0x3B
#define MPU6050_AY  0x3D
#define MPU6050_AZ  0x3F
#define MPU6050_TP  0x41                //  data not used
#define MPU6050_GX  0x43
#define MPU6050_GY  0x45
#define MPU6050_GZ  0x47

/*<<---------------------
  MTOF
  TOF距離センサモジュール
--------------------->>*/
#define MTOF_PIN  13

/*-------------------------------------------------------------
変数管理
-------------------------------------------------------------*/
#if 1
/* 超音波センサ用変数 */
int TRIG = 5;
int ECHO = 13;
double duration = 0;
double distance = 0;
double speed_of_sound = 331.5 + 0.6 * 25; 
#endif

/* MPU6050用変数 */
short int AccX, AccY, AccZ;
short int Temp;
short int GyroX, GyroY, GyroZ;
char state = COMMAND_STOP;
int offset = 10;
short int offset_Ax, offset_Ay, offset_Az, offset_Gx, offset_Gy, offset_Gz;
char duty;
int angle;
float AccX_c;
float AccY_c;
float AccZ_c;
float GyroX_c;
float GyroY_c;
float GyroZ_c;

#if 1
/* MTOF用変数 */
uint16_t distance_m;
uint16_t distance_tmp;
uint8_t data_cnt;
#endif

#include "server.h"   //Go to http://192.168.4.1 in a web browser

/*==============================================================
  setup↓
==============================================================*/
void setup() {
  delay(500);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");

  int SDA = 4;
  int SCL = 14;
  Wire.begin(SDA, SCL);
  delay(40);

  pinMode(ECHO, INPUT );
  pinMode(TRIG, OUTPUT );
  
  /*kasokudo*/
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission();

/*Front_measure*/
  sensor.init();
  sensor.setTimeout(1000);
  sensor.startContinuous(10);
  
  /* You can remove the password parameter if you want the AP to be open. */
  #include "remote.h"

  pinMode(16,OUTPUT);
  pinMode(12,OUTPUT);

  LED_H;
  
  delay(100);
  
  /*Front_measure*///////
  //タイムアウトまでの時間をセット
//  sensor.setTimeout(500);
  //距離センサ(VL53L0X)の初期化
//  if (!sensor.init()){
//    Serial.println("Failed to detect and initialize sensor!");
//    while (1) {}
//  }
//  sensor.startContinuous(50);


  #include "offset_kasoku.h"

  delay(1000);
}
/*==============================================================
  setup↑
==============================================================*/

#include "Front_measure.h"
#include "Right_measure.h"
#include "Left_measure.h"     
#include "kasokudo.h"
#include "Mtof.h"

int L_side;
unsigned short int Front;
int R_side;
int f_obs, r_obs, l_obs = 0;
int count = 0;

/*前右左のセンサ値をまとめて取る関数*/
void measure_FRL(){
//  L_side = Left_measure();
//  L_side = L_side / 10;
  Front = Mtof();
  R_side = Right_measure();

  if (Front < 20){        //前15cm以内に障害があればf_obsを1とする
    f_obs = 1;
  }else{                  //障害物がなければ0とする
    f_obs = 0;
  }
  if (R_side < 20){       //右10cm以内に障害があればr_obsを1とする
    r_obs = 1;
  }else{                  //障害物がなければ0とする
    r_obs = 0;
  }
  if (L_side < 20){       //左10cm以内に障害があればl_obsを1とする
    l_obs = 1;
  }else{                  //障害物がなければ0とする
    l_obs = 0;
  }

  delay(100);
#if 0  
  Serial.print("  front:"); Serial.print(f_obs);
  Serial.print("  right:"); Serial.print(r_obs);
  Serial.print("  left:"); Serial.println(l_obs);
#endif

#if 1
  Serial.print("F:"); Serial.print(Front);
  Serial.print("  R:"); Serial.print(R_side);
  Serial.print("  L:"); Serial.println(L_side);
#endif
}

/*前右左でスペースのある方にハンドルを切る関数*/
int Choice_handle(int F, int L, int R){
  if (F > R){
    handle_forward();
  }else if (F > L){
    handle_forward();
  }else if (R > L){
    handle_right();
  }else{
    handle_left();
  }
}
//////////////////////////////////loop/////
/*やりたいこと
  ・ログの取得
  ・PID制御
  ・choice_handle*/
void loop() {
  int i;
//  server.handleClient();                        
//  server_8080.handleClient();
  
//  handle_forward();
  measure_FRL();

//  Kasokudo(); 

#if 0
  if (R_side <10 && L_side < 10){
//    handle_back();
//    delay(1500);
//    handle_f_right();
//    delay(1000);
    measure_FRL();
  }else if (l_obs == 0){
    for (i=0; i<2000; i++){
      handle_f_right();
        measure_FRL();
      if (r_obs == 1){
        break;
      }
      delay(10);
    }
    handle_stop();
    delay(1000);
    measure_FRL();
  }else if (r_obs ==1){
    handle_f_left();
    delay(500);
    measure_FRL();
  }
#endif

#if 0
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
 
 nDevices = 0;
 for (address = 1; address < 127; address++ ){
   Wire.beginTransmission(address);
   error = Wire.endTransmission();
   if (error == 0){
     Serial.print("I2C device found at address 0x");
     if (address < 16)
       Serial.print("0");
       Serial.print(address, HEX);
       Serial.println("  !");
       nDevices++;
       }else if (error == 4){
          Serial.print("Unknown error at address 0x");
          if (address < 16)
            Serial.print("0");
            Serial.println(address, HEX);
        }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
    delay(5000);           // wait 5 seconds for next scan
#endif
//delay(100);
}
/////////////////////////////////////loop/////
void handleRoot() {
  server.send(200, "text/html", form);
}

void handle_stop() {
//  Serial.print("stop\r\n");
  LED_L;
    stop_motor();
    state = COMMAND_STOP;
  LED_H;
  server_8080.send(200, "text/html", "");
}

int handle_forward() {
// Serial.print("forward\r\n");
  drive();
  servo_control(110);
  server_8080.send(200, "text/html", "");
//  return TRUE;
}

void handle_back() {
//  Serial.print("back\r\n");
  back();
  servo_control(110);
  server_8080.send(200, "text/html", "");
}

void handle_left(){
//  Serial.print("left\r\n");
  servo_control(SERVO_LEFT);
  server_8080.send(200, "text/html", "");
}

void handle_right(){
//  Serial.print("right\r\n");
  int  right;
  servo_control(SERVO_RIGHT);
  server_8080.send(200, "text/html", "");
}

void handle_f_left(){
//  Serial.print("f_left\r\n");
  drive();
  servo_control(SERVO_LEFT);
  server_8080.send(200, "text/html", "");
}

void handle_f_right(){
//  Serial.print("f_right\r\n");
  drive();
  servo_control(SERVO_RIGHT);
 server_8080.send(200, "text/html", "");
}

void handle_b_left(){
//  Serial.print("b_left\r\n");
  back();
  servo_control(SERVO_LEFT);
  server_8080.send(200, "text/html", "");
}


void handle_b_right(){
//  Serial.print("b_right\r\n");
  back();
  servo_control(SERVO_RIGHT);
  server_8080.send(200, "text/html", "");
}

void drive(){
    if(state == COMMAND_BACK){
     stop_motor();
     delay(10);
     start_motor();
  
  }else if(state == COMMAND_STOP){
    start_motor();
  }
  state = COMMAND_START;
}

void back(){
    if(state == COMMAND_START){
     stop_motor();
     delay(10);
     reverse_motor();
  }else if(state == COMMAND_STOP){
    reverse_motor();
  }
  state = COMMAND_BACK;
}

void start_motor(){
  char i, volt;
  volt = 0x20;
  
  for(i=0;i<5;i++){ 
    volt = volt * 0x04;
    volt = volt | FORWARD;
    motor_func(ADDR1 , volt);
    delay(10);
//    Serial.println(volt, HEX);
  }
}

void reverse_motor(){
  char i, volt;
  volt = 0x20;
  
  for(i=0;i<5;i++){
    volt = volt + 0x04;
    volt = volt | RESERVE;
    motor_func(ADDR1 , volt);
    delay(10);
//    Serial.println(volt, HEX);
  }
}

void stop_motor(){
  motor_func(ADDR1 , 0x18);
  delay(10);
  motor_func(ADDR1 , 0x1B);
  delay(10);
}

char motor_func(char add , char duty0){
  Wire.beginTransmission(add);
  Wire.write(0x00);
  Wire.write(duty0);
  Wire.endTransmission();
  duty = duty0;
}

int servo_control(int ang){
  int microsec,i;
  LED_L;
  angle = ang;
  microsec = (5*(ang+offset))+ 1000;
    
  for(i=0; i<20 ;i++){
    digitalWrite( 16, HIGH );
    delayMicroseconds( microsec ); 
    digitalWrite( 16, LOW );
    delayMicroseconds( 10000 - microsec ); 
  }
   LED_H;
}
