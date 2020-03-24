//開発版・デバック用
#include<Arduino.h>
#include <EEPROM.h>
#include <M5Stack.h>

#include <ros.h>
#include <std_msgs/Float32.h>

#define Vout_pin 35     //analogpin
#define Pressure_pin 36 //analogpin
#define Flowmeter_pin 5 //digitalpin
#define Pomp_pwmpin 17  //pwmpin
#define Read_cnt_ 1000

#define EEP_ADRS_OFF 0    //offset用のアドレス:0~3
#define EEP_SIZE 4        //EEPROMで扱うデータのサイズ

//個体毎に調整が必要なdefineを以下に示す。
#define Analog_Max_Vout 3.30
/*---Analog_Max_Vout:Analog値がAnalog_Maxの時の出力電圧[v] promicro:4.56[v] esp32:3.30[v]---*/

#define Analog_Max 4095.0
/*---Analog値がMaxの時の値 promicro:1023.0 esp32:4095.0---*/

#define Dv_Dw 0.01780
//ゴトウユキトが手動で調整
/*---Dv_Dw:電圧を重さに変換する定数 promicro:0.02400 esp32:0.01780---*/

typedef union{//前回起動時のデータを共用体に記憶しておく。
  float all_data;
  //この処理系ではfloatは4バイト
  uint8_t part_data[4];
}ROM;

ROM offset;                   //offset:Vout_offset用の共用体

ros::NodeHandle nh;
std_msgs::Float32 weight_msg;
ros::Publisher weight_pub("weight_pub",&weight_msg);

float Vout,W;                 //Vout:出力電圧[v] W:重さ[kg]
float Vout_offset;            //Vout_offset:体重計起動時のVoutの値(オフセット)
float dv_dw;                  //dv_dw:グラフの傾き自動調整される予定だった。初期値DV_DWのまま。

void Read_Vout(int Read_cnt);            //Read_Vout:出力電圧測定関数 Read_cnt:読み取り回数
void Lead_W(float Vout_);                //Lead_W:出力電圧を重さに変換する関数 Vout:出力電圧[v]
void Re_Vout_offset(void);               //Re_Vout_offset:オフセットを求める関数。求めた値をEEPROMに格納する。0[kg]調整に使用する。
void write_data(int adr);                //write_data:EEPROMに1バイトごとに書き込む adr:アドレス
void read_data(int adr);                 //read_data:EEPROMから1バイトごとに読み込む

void setup() {
  //Serial.begin(9600);
  //M5Stack初期化
  M5.begin();
  M5.Lcd.setTextSize(2);

  //esp32ではeepromの初期宣言が必要
  EEPROM.begin(EEP_SIZE);

  //諸変数の初期化
  Vout = 0.0,W = 0.0;
  dv_dw = Dv_Dw;

  //Vout_offsetの取得
  read_data(EEP_ADRS_OFF);
  Vout_offset = offset.all_data;

  //ROS関係
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  nh.advertise(weight_pub);
}

void loop() {
  //スイッチの状態を更新
  M5.update();
  
  //スイッチが押されたらRE_Vout_offsetを呼び出す。0[kg]調整される。
  if(M5.BtnA.isPressed()){
    Re_Vout_offset();
    
    //Serial.print("\n----------------------------------\n");
    //Serial.print("Vout_offset[V] = ");  Serial.println(Vout_offset,4);

    M5.Lcd.setCursor(0,0);
    //1行目
    M5.Lcd.println("offset!");
    //2行目
    M5.Lcd.println("reset!");
    
    delay(1000);
  }
  else{
    Read_Vout(Read_cnt_);
    Lead_W(Vout);
    //Serial.print("\n----------------------------------\n");
    //Serial.print("Vout[V] = "); Serial.println(Vout,4);
    //Serial.print("W[Kg] = "); Serial.println(W,4);
    
    //1行目
    M5.Lcd.setCursor(0,0);
    M5.Lcd.println("Nowvalue");

    //2行目
    M5.Lcd.print(" ");
    if(W < 10.0){
      if(W <= 5.0){
      //led反転
      }
      (W < 0) ? M5.Lcd.print(0.0,2) : M5.Lcd.print(W,2);
    }
    else{
      M5.Lcd.print(W,1);
    }
    M5.Lcd.print("[L]");

    weight_msg.data = W;
    weight_pub.publish(&weight_msg);
    
    delay(200);
  }
  nh.spinOnce();
}

void Read_Vout(int Read_cnt){
  int cnt = 0,Vout_val = 0;
  float Vout_sum = 0.0;
  
  for(cnt = 0;cnt < Read_cnt;cnt++){
    Vout_val = analogRead(Vout_pin);
    Vout = (float)Vout_val * Analog_Max_Vout / Analog_Max;
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
      //Serial.print("\n----------------------------------\n");
      //Serial.print("write_Error!\n");
      break;
    }
    //eps32では書き込み後にコミットが必要
    EEPROM.commit();
  }
}

void read_data(int adr){
  int i;
  for(i = 0;i < 4;i++){
    if(adr == EEP_ADRS_OFF)
      offset.part_data[i] = EEPROM.read(adr + i);
    else{
      //Serial.print("\n----------------------------------\n");
      //Serial.print("read_Error!\n");
      break;
    }
  }
}