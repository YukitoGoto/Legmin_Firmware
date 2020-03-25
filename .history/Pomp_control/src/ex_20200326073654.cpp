//動作に最低限必要なコード
#include <Arduino.h>
#include "Pomp_control.h"

Pomp_control ex;  //Pomp_controlクラスをインスタンス化
Ticker ctr;       //Pomp_controlに必要 ctr:任意のインスタンス名

//loop関数より上に実装する。静的記憶域に居る関数。割込み処理で呼び出す。
void onRising(void){
  ex.onRising();
}

void control_val(void){
  ex.control_val();
}

void setup() {
  //割込み処理
  attachInterrupt(digitalPinToInterrupt(Default_Flowmeter_pin), onRising, FALLING);
  ctr.attach(Control_interval,control_val);
}

void loop() {
}