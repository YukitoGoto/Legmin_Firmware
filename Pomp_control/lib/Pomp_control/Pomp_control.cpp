#include "Pomp_control.h"

Pomp_control::Pomp_control(int Pomp_pwmpin_,int Flowmeter_pin_,int Pressure_pin_){
  //ピン設定
  Pomp_pwmpin = Pomp_pwmpin_,Flowmeter_pin = Flowmeter_pin_,Pressure_pin = Pressure_pin_;
  //諸変数の初期化
  shutoff = false,TargetLpm = 0.0,CurrentLpm = 0.0,CurrentPress = 0.0;
  pulseCounter = 0.0,CurrentHz = 0.0,DT = 0.0,Vout = 0.0;
}

void Pomp_control::begin(void) {
  //PIDmotorクラスのインスタンス化
  motor = new PID(Kp,Ki,Kd);

  //割込みセットアップ
  pinMode(Flowmeter_pin, INPUT_PULLUP);

  //PID制御セットアップ
  motor->setOutputLimits(DT_MIN,DT_MAX);

  //ポンプ制御に用いるPWMセットアップ
  ledcSetup(Pomp_pwmch,Pomp_pwmHz,Pomp_pwmrenge);
  ledcAttachPin(Pomp_pwmpin,Pomp_pwmch);
}

void Pomp_control::update_power(void){
  ledcWrite(Pomp_pwmch,(int)(DT * Pomp_pwmmax));
}

void Pomp_control::control_val(void){
  //流量の入力値を0.0~Max_targetLpmに制限する。
  (TargetLpm > Max_targetLpm) ? TargetLpm = Max_targetLpm : TargetLpm = TargetLpm;
  (TargetLpm < 0.0) ? TargetLpm = 0.0 : TargetLpm = TargetLpm;
  //各値の更新を行う。
  getCurrentLpm();
  Lead_P();
  //求めた値からPID制御して、操作量としてDT(デューティー比)を求める。ポンプ内のモーターへ反映。
  DT = motor->update_val(TargetLpm,CurrentLpm,Control_interval);
  //シャットオフ圧力を超えていたら、DTを零にしてモーターを停止する。
  if(shutoff){
    DT = 0.0;
    shutoff = false;
  }
  update_power();
}

void Pomp_control::onRising(void) {
  ++pulseCounter;
}

void Pomp_control::getCurrentLpm(void) {
  CurrentHz = (double) pulseCounter * (1.0 / Control_interval);
  CurrentLpm = (float) CurrentHz / DIV_NUMBER_FOR_LPM_FROM_HZ;
  if(CurrentLpm < 0.0)
    CurrentLpm = 0.0;
  pulseCounter = 0;
}

void Pomp_control::Lead_P(void){
  int cnt = 0,Vout_val = 0;
  float Vout_sum = 0.0;
  
  for(cnt = 0;cnt < Sample_cnt;cnt++){
    Vout_val = analogRead(Pressure_pin);
    Vout = (float)Vout_val * Analog_Max_Vout / Analog_Max;
    Vout_sum += Vout;
  }
  Vout = Vout_sum / (float)Sample_cnt;              //Sample_cnt回測った電圧の平均値を採用する。
  CurrentPress = (Vout - Vout_offset) / Dv_Dp;      //出力電圧から現在の圧力を求める。
  if(CurrentPress < 0.0)
    CurrentPress = 0.0;
  //シャットオフ圧力を超えていたら有効にする。
  if(CurrentPress > Max_pomp_p)
    shutoff = true;
}