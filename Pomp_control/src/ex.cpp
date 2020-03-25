//動作に最低限必要なコード
#include <Arduino.h>
#include "Pomp_control.h"

Pomp_control ex;
Ticker ctr;

void onRising(void){
  ex.onRising();
}

void control_val(void){
  ex.control_val();
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(Default_Pomp_pwmpin), onRising, FALLING);
  ctr.attach(Control_interval,control_val);
}

void loop() {
}