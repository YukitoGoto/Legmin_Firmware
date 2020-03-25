#ifndef WEIGHT_SCALE_M5STACK_H_
#define WEIGHT_SCALE_M5STACK_H_

#include<Arduino.h>
#include<EEPROM.h>
#include<M5Stack.h>

//EEPROMに使う定数
#define EEP_ADRS_OFF 0          //Vout_offset用のアドレス EEP_ADRS_OFFを先頭にEEP_SIZEバイト消費される。
#define EEP_SIZE 4              //EEPROMで扱うデータのサイズ この処理系(Arduino)ではfloat型が4バイトとなる。

//アナログ信号処理に使う定数
#define Analog_Max_Vout 3.30    //Analog_Max_Vout:Analog値がAnalog_Maxの時の出力電圧[v] promicro:4.56[v] esp32:3.30[v]
#define Analog_Max 4095.0       //Analog値がMaxの時の値 promicro:1023.0 esp32:4095.0
#define Sample_cnt_ 1000        //アナログ信号のサンプリング回数
#define Dv_Dw 0.01780           //ゴトウユキトが手動で調整 Dv_Dw:電圧を重さに変換する定数 promicro:0.02400 esp32:0.01780

//ピンアサイン,LCD文字大きさ
#define Default_signal_pin 35   //Signal_Pinのデフォルト値
#define Default_lcd_text_size 4 //Lcd_text_sizeのデフォルト値

class Weight_M5{

public:

    Weight_M5(int Signal_Pin = Default_signal_pin,int Lcd_text_size = Default_lcd_text_size);//Signal_Pin:アンプからのアナログ信号を読み取るピン Lcd_text_size:LCDに表示するテキストのサイズ

    void begin(void);                       //begin:setup関数内で必ず実行すること! M5Stackの初期化やEEPROMの初期化を行う。

    void update_val(void);                  //update_val:Weight_ Vout_offset_ 等の更新

    void update_lcd(void);                  //update_lcd:LCDの内容を現在の値で更新

    float Weight_;                          //Weight_:重さ[kg]

private:

    bool btn_a;                             //btn_a:ボタンAの状態 LCD制御に使用 

    int Signal_Pin_;                        //Signal_Pin_:アンプからのアナログ信号を読み取るピン

    int Lcd_text_size_;                     //Lcd_text_size_:LCDに表示するテキストのサイズ

    float Vout_;                            //Vout:出力電圧[v]
    
    float Vout_offset_;                     //Vout_offset:体重計起動時のVoutの値(オフセット)  

    void Read_Vout(int Sample_cnt);         //Read_Vout:出力電圧測定関数 Sample_cnt:読み取り回数
    
    void Lead_W(float Vout);                //Lead_W:出力電圧を重さに変換する関数 Vout:出力電圧[v]
    
    void Re_Vout_offset(void);              //Re_Vout_offset:オフセットを求める関数。求めた値をEEPROMに格納する。0[kg]調整に使用する。
    
    void write_data(int adr);               //write_data:EEPROMに4バイトごとに書き込む adr:アドレス
    
    void read_data(int adr);                //read_data:EEPROMから4バイトごとに読み込む
    
    union{                                  //ROM:Vout_offset用の共用体
        float all_data;
        uint8_t part_data[EEP_SIZE];
    }ROM;

};

#endif //Weight_scale_M5Stack.h
