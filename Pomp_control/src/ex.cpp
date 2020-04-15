//デバッグ用
#include <Arduino.h>
#include "Pomp_control.h"

#define TARGET_LPM 9.5
//#define DEBUG

Pomp_control ex;  //Pomp_controlクラスをインスタンス化
Ticker ctr;       //Pomp_controlに必要 ctr:任意のインスタンス名
unsigned long msec;

//loop関数より上に実装する。静的記憶域に居る関数。割込み処理で呼び出す。
void onRising(void){
  ex.onRising();
}

void control_val(void){
  ex.control_val();
}

void setup() {
  //初期化処理を行う。
  Serial.begin(9600);
  ex.begin();
  //割込み処理を行う。
  attachInterrupt(digitalPinToInterrupt(Default_Flowmeter_pin), onRising, FALLING);
  ctr.attach(Control_interval,control_val);
  //プログラム開始時刻[ms]のリセットを行う。
  msec = millis();
}

void loop() {
  if((millis() - msec) > 1000UL){  
    #ifdef DEBUG
    Serial.println("TargetLpm " + String(ex.TargetLpm) + "[l/m]");
    Serial.println("CurrentLpm " + String(ex.CurrentLpm) + "[l/m]");
    Serial.println("CurrentPress " + String(ex.CurrentPress) + "[MPa]");
    #endif  //DEBUG
    msec = millis();
  }
  else{
    ex.TargetLpm = TARGET_LPM;
  }
}