#include <PS2X_lib.h>
#include <Arduino.h>

PS2X ps2x; // create PS2 Controller Class

int error = 0;
byte vibrate = 0;

void setup(){
Serial.begin(57600);

error = ps2x.config_gamepad(32,33,25,26);
//setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

if(error == 0){
  Serial.println("\nFound Controller, configured successful");
}

else if(error == 1)
  Serial.println("\nNo controller found, check wiring, see readme.txt to enable debug. ");

else if(error == 2)
  Serial.println("\nController found but not accepting commands. see readme.txt to enable debug. ");

else if(error == 3)
  Serial.println("\nController refusing to enter Pressures mode, may not support it. ");
  delay(1000);
}


void loop(){

ps2x.read_gamepad(false, vibrate); //ゲームパッド・ボタンの読み込み

if(ps2x.Button(PSB_START)) //スタートボタン
  Serial.println("Start");
if(ps2x.Button(PSB_SELECT)) //セレクトボタン
  Serial.println("Select");

if(ps2x.Button(PSB_PAD_UP)) //上ボタン
  Serial.println("U");
if(ps2x.Button(PSB_PAD_RIGHT)) //右ボタン
  Serial.println("R");
if(ps2x.Button(PSB_PAD_LEFT)) //左ボタン
  Serial.println("L");
if(ps2x.Button(PSB_PAD_DOWN)) //下ボタン
  Serial.println("D");

if(ps2x.Button(PSB_GREEN)) //三角ボタン
  Serial.println("G");
if(ps2x.Button(PSB_RED)) //丸ボタン
  Serial.println("O");
if(ps2x.Button(PSB_PINK)) //四角ボタン
  Serial.println("P");
if(ps2x.Button(PSB_BLUE)) //エックスボタン
  Serial.println("X");

if(ps2x.Button(PSB_L3))
  Serial.println("L3");
if(ps2x.Button(PSB_R3))
  Serial.println("R3");
if(ps2x.Button(PSB_L2))
  Serial.println("L2");
if(ps2x.Button(PSB_R2))
  Serial.println("R2");
if(ps2x.Button(PSB_R1))
  Serial.println("R1");

if(ps2x.Button(PSB_L1)) // Ｌ１を押しながら、アナログスチックコントロール
{
  Serial.print(ps2x.Analog(PSS_LY), DEC);
  Serial.print(",");
  Serial.print(ps2x.Analog(PSS_LX), DEC);
  Serial.print(",");
  Serial.print(ps2x.Analog(PSS_RY), DEC);
  Serial.print(",");
  Serial.println(ps2x.Analog(PSS_RX), DEC);
}

delay(50);

}