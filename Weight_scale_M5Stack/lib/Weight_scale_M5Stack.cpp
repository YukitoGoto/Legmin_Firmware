#include "Weight_scale_M5Stack.h"

Weight_M5::Weight_M5(int Signal_Pin,int Lcd_text_size){
  //諸変数の初期化
  Signal_Pin_ = Signal_Pin,Lcd_text_size_ = Lcd_text_size;
  Vout_ = 0.0,Weight_ = 0.0;
  btn_a = false;
}

void Weight_M5::begin(void) {
  //M5Stack初期化
  M5.begin();
  M5.Lcd.setTextSize(Lcd_text_size_);

  //EEPROMの初期化 esp32ではeepromの初期宣言が必要
  EEPROM.begin(EEP_SIZE);

  //Vout_offsetの取得
  read_data(EEP_ADRS_OFF);
  Vout_offset_ = ROM.all_data;
}

void Weight_M5::update_val(void) {
  //スイッチの状態を更新
  M5.update();

  //スイッチが押されたらRe_Vout_offsetを呼び出す。0[kg]調整される。
  if(M5.BtnA.isPressed())
  {
    Re_Vout_offset();
    btn_a = true;
  }
  else{
    Read_Vout(Sample_cnt_);
    Lead_W(Vout_);
    btn_a = false;
  }
}

void Weight_M5::update_lcd(void){
  M5.Lcd.setCursor(0,0);
  if(btn_a){
    btn_a = false;
    //1行目
    M5.Lcd.println("offset!");
    //2行目
    M5.Lcd.println("reset!");
    
    //lcd表示を行うための待機 適宜コメントアウトしてもらって構いません。
    delay(1000);
  }
  else{
    //1行目
    M5.Lcd.println("Nowvalue");
    //2行目
    M5.Lcd.print(" ");
    if(Weight_ < 10.0){
      if(Weight_ <= 5.0){
      //背景色変更（未実装）
      }
      M5.Lcd.print(Weight_,2);
    }
    else{
      M5.Lcd.print(Weight_,1);
    }
    M5.Lcd.println("[L]");
  }
}

void Weight_M5::Read_Vout(int Sample_cnt){
  int cnt = 0,Vout_val = 0;
  float Vout_sum = 0.0;
  
  for(cnt = 0;cnt < Sample_cnt;cnt++){
    Vout_val = analogRead(Signal_Pin_);
    Vout_ = (float)Vout_val * Analog_Max_Vout / Analog_Max;
    Vout_sum += Vout_;
  }
  
  Vout_ = Vout_sum / (float)Sample_cnt;    //Sample_cnt回測った電圧の平均値を採用する。
}

void Weight_M5::Lead_W(float Vout){
  Weight_ = (Vout - Vout_offset_) / Dv_Dw;
  (Weight_ < 0) ? Weight_ = 0.0 : Weight_ = Weight_;
}

void Weight_M5::Re_Vout_offset(void){
  Read_Vout(Sample_cnt_);
  Vout_offset_ = Vout_;
  ROM.all_data = Vout_;
  write_data(EEP_ADRS_OFF);
}

void Weight_M5::write_data(int adr){
  int i;
  for (i = 0;i < EEP_SIZE;i++){
    EEPROM.write(adr + i,ROM.part_data[i]);
    //eps32では書き込み後にコミットが必要
    EEPROM.commit();
  }
}

void Weight_M5::read_data(int adr){
  int i;
  for(i = 0;i < EEP_SIZE;i++){
    ROM.part_data[i] = EEPROM.read(adr + i);
  }
}