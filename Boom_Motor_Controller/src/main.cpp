#include <Arduino.h>

//各スイッチは負倫理（SW = ON,GPIO = LOW）
#define LIMIT_START_PIN 8
#define LIMIT_FINISH_PIN 9
#define AUTO_SW_PIN 10
#define MANUAl_SW_PIN 11
#define CW_SW_PIN 12
#define CCW_SW_PIN 13

//各スイッチの状態用変数
int limit[2] = {HIGH};  //limit[0]:始端リミットスイッチ状態,limit[1]:終端リミットスイッチ状態,ON = LOW,OFF = HIGH
int mode = HIGH;        //mode:自動or手動モード選択,LOW = 自動,HIGH = 手動
int rotation = HIGH;    //rotation:回転方向,LOW = CCW,HIGH = CW

//各スイッチの状態更新
void Upd_SW_Status(int *limit,int *mode,int *rotation);

void setup() {
  pinMode(LIMIT_START_PIN,INPUT);
  pinMode(LIMIT_FINISH_PIN,INPUT);
  pinMode(AUTO_SW_PIN,INPUT);
  pinMode(MANUAl_SW_PIN,INPUT);
  pinMode(CW_SW_PIN,INPUT);
  pinMode(CCW_SW_PIN,INPUT);
}

void loop() {
  Upd_SW_Status(limit,&mode,&rotation);
  switch (mode)
  {
  case LOW:
    break;
  case HIGH:
    break;
  default:
    break;
  }
}

void Upd_SW_Status(int *limit,int *mode,int *rotation){
  limit[0] = digitalRead(LIMIT_START_PIN);
  limit[1] = digitalRead(LIMIT_FINISH_PIN);
  if((LOW == digitalRead(AUTO_SW_PIN)) && (HIGH == digitalRead(MANUAl_SW_PIN))){
    *mode = LOW;   //自動
  }
  else if((HIGH == digitalRead(AUTO_SW_PIN)) && (LOW == digitalRead(MANUAl_SW_PIN))){
    *mode = HIGH;  //手動
  }
  else{
    *mode = -1;    //エラー
  }
  if((LOW == digitalRead(CCW_SW_PIN)) && (HIGH == digitalRead(CW_SW_PIN))){
    *rotation = LOW;   //CCW
  }
  else if((HIGH == digitalRead(CCW_SW_PIN)) && (LOW == digitalRead(CW_SW_PIN))){
    *rotation = HIGH;  //CW
  }
  else{
    *rotation = -1;    //入力無し
  }
}