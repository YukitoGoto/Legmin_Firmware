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
  attachInterrupt(digitalPinToInterrupt(ex.Flowmeter_Signal_Pin), onRising, FALLING);
  ctr.attach(Control_interval,control_val);
}

void loop() {
  // put your main code here, to run repeatedly:
}