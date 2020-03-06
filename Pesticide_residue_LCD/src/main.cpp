//製品版
#include<Arduino.h>
#include <EEPROM.h>
#include <Wire.h>
#include <ST7032.h>

ST7032 lcd;

#define Vout_pin 21
#define SW_pin 15
#define SW_length_short 1000
#define SW_length_long 100000
#define Read_cnt_ 1000
#define Sample_cnt_ 100

#define EEP_ADRS_OFF 0    //offset用のアドレス:0~3

//個体毎に調整が必要なdefineを以下に示す。
#define Vcc 4.56
/*---Vcc:promicroのVcc端子の電位[V]である。テスターで測った値は次のようになった。:4.54 < Vcc < 4.58:体重計に何の乗せない時のVout[V]の値が、テスターで測った値と同じくらい(許容10[mV]以内)になるように調整する。---*/
//#define Dv_Dw 0.02362
//重りの場合
//#define Dv_Dw 0.028344
//水の場合
#define Dv_Dw 0.02400
//ゴトウユキトが手動で調整
/*---Dv_Dw:dv_dwの初期値---*/

typedef union{//前回起動時のデータを共用体に記憶しておく。
  float all_data;
  //この処理系ではfloatは4バイト
  unsigned char part_data[4];
}ROM;

ROM offset;                   //offset:Vout_offset用の共用体

float Vout,W;                 //Vout:出力電圧[v] W:重さ[kg]
float Vout_offset;            //Vout_offset:体重計起動時のVoutの値(オフセット)
float dv_dw;                  //dv_dw:グラフの傾き自動調整される予定だった。初期値DV_DWのまま。
int sw;                       //sw:スイッチの状態:負論理

void Read_Vout(int Read_cnt);            //Read_Vout:出力電圧測定関数 Read_cnt:読み取り回数
void Lead_W(float Vout_);                //Lead_W:出力電圧を重さに変換する関数 Vout:出力電圧[v]
void Re_Vout_offset(void);               //Re_Vout_offset:オフセットを求める関数。求めた値をEEPROMに格納する。0[kg]調整に使用する。
void write_data(int adr);                //write_data:EEPROMに4バイトごとに書き込む adr:アドレス
void read_data(int adr);                 //read_data:EEPROMから4バイトごとに読み込む

void setup() {
  Serial.begin(9600);
  // LCD表示領域設定(8桁, 2行)
  lcd.begin(8, 2);
  // コントラスト設定(0〜63)
  lcd.setContrast(30);
  pinMode(SW_pin,INPUT);

  //諸変数の初期化
  Vout = 0.0,W = 0.0,sw = LOW;
  dv_dw = Dv_Dw;

  //Vout_offsetの取得
  read_data(EEP_ADRS_OFF);
  Vout_offset = offset.all_data;
}

void loop() {
  sw = digitalRead(SW_pin);

  //スイッチが押されたらRE_Vout_offsetを呼び出す。0[kg]調整される。
  if(sw == LOW){
    long int sw_cnt = 0;
    Re_Vout_offset();
    
    Serial.print("\n----------------------------------\n");
    Serial.print("Vout_offset[V] = ");  Serial.println(Vout_offset,4);

    //1行目
    lcd.setCursor(0,0);
    lcd.print("offset!");
    //2行目
    lcd.setCursor(0,1);
    lcd.print("reset!");
    
    delay(1000);
  }
  else{
    Read_Vout(Read_cnt_);
    Lead_W(Vout);
    Serial.print("\n----------------------------------\n");
    Serial.print("Vout[V] = "); Serial.println(Vout,4);
    Serial.print("W[Kg] = "); Serial.println(W,4);
    
    //1行目
    lcd.setCursor(0,0);
    lcd.print("Nowvalue");
    //2行目
    lcd.setCursor(0,1);//表示の統一を図るための処理
    lcd.println(" ");
    if(W < 10.0){
      if(W <= 5.0){
      //led反転
      }
      lcd.setCursor(1,1);
      (W < 0) ? lcd.println(0.0,2) : lcd.println(W,2);
    }
    else{
      lcd.setCursor(1,1);
      lcd.println(W,1);
    }
    lcd.setCursor(5,1);
    lcd.print("[L]");
    
    delay(200);
  }
}

void Read_Vout(int Read_cnt){
  int cnt = 0,Vout_val = 0;
  float Vout_sum = 0.0;
  
  for(cnt = 0;cnt < Read_cnt;cnt++){
    Vout_val = analogRead(Vout_pin);
    Vout = (float)Vout_val * Vcc / 1023.0;
    Vout_sum += Vout;
  }
  
  Vout = Vout_sum / (float)Read_cnt;    //Read_cnt回測った電圧の平均値を採用する。
}

void Lead_W(float Vout_){
  W = (Vout_ - Vout_offset) / dv_dw;
}

void Re_Vout_offset(void){
  Read_Vout(Read_cnt_);
  Vout_offset = Vout;
  offset.all_data = Vout;
  write_data(EEP_ADRS_OFF);
}

void write_data(int adr){
  int i;
  for (i = 0;i < 4;i++){
    if(adr == EEP_ADRS_OFF)  
      EEPROM.write(adr + i,offset.part_data[i]);
    else{
      Serial.print("\n----------------------------------\n");
      Serial.print("write_Error!\n");
      break;
    }
  }
}

void read_data(int adr){
  int i;
  for(i = 0;i < 4;i++){
    if(adr == EEP_ADRS_OFF)  
      offset.part_data[i] = EEPROM.read(adr + i);
    else{
      Serial.print("\n----------------------------------\n");
      Serial.print("read_Error!\n");
      break;
    }
  }
}