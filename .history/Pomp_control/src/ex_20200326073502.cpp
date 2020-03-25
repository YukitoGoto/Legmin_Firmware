//動作に最低限必要なコード
#include <Arduino.h>
#include "Pomp_control.h"

Pomp_control ex;  //Pomp_controlクラスをインスタンス化
Ticker ctr;       //Pomp_controlに必要 

void onRising(void){
  ex.onRising();
}

void control_val(void){
  ex.control_val();
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(Default_Flowmeter_pin), onRising, FALLING);
  ctr.attach(Control_interval,control_val);
}

void loop() {
}