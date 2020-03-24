#ifndef WEIGHT_SCALE_M5STACK_H_
#define WEIGHT_SCALE_M5STACK_H_

#include<Arduino.h>
#include<EEPROM.h>
#include<M5Stack.h>

//調整不要な定数を以下に示す。
#define EEP_ADRS_OFF 0    //offset用のアドレス:0~3
#define EEP_SIZE 4        //EEPROMで扱うデータのサイズ

//調整可能な定数を以下に示す。
#define Analog_Max_Vout 3.30
/*---Analog_Max_Vout:Analog値がAnalog_Maxの時の出力電圧[v] promicro:4.56[v] esp32:3.30[v]---*/

#define Analog_Max 4095.0
/*---Analog値がMaxの時の値 promicro:1023.0 esp32:4095.0---*/

#define Dv_Dw 0.01780
//ゴトウユキトが手動で調整
/*---Dv_Dw:電圧を重さに変換する定数 promicro:0.02400 esp32:0.01780---*/

class Weight_M5{
public:
    /*
    Signal_Pin:アンプからのアナログ信号を読み取るピン(デフォルトでは35ピンが指定)
    */

    Weight_M5(int Signal_Pin);

    void begin(void);

    void update_val(void);

    void print_Weight(void);

    float Weight_;                          //W:重さ[kg]


private:

    float Vout_;                             //Vout:出力電圧[v]
    
    float Vout_offset_;                      //Vout_offset:体重計起動時のVoutの値(オフセット)  

    void Read_Vout(int Read_cnt);            //Read_Vout:出力電圧測定関数 Read_cnt:読み取り回数
    
    void Lead_W(float Vout_);                //Lead_W:出力電圧を重さに変換する関数 Vout:出力電圧[v]
    
    void Re_Vout_offset(void);               //Re_Vout_offset:オフセットを求める関数。求めた値をEEPROMに格納する。0[kg]調整に使用する。
    
    void write_data(int adr);                //write_data:EEPROMに4バイトごとに書き込む adr:アドレス
    
    void read_data(int adr);                 //read_data:EEPROMから4バイトごとに読み込む
    
    typdef union{
        float all_data;
        uint8_t part_data[EEP_SIZE];
    }ROM;

}

#endif //Weight_scale_M5Stack.h
