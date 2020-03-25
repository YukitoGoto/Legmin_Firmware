#ifndef POMP_CONTROL_H
#define POMP_CONTROL_H

#include <Arduino.h>
#include <Ticker.h>
#include "PID.h"

//ポンプ制御に用いる定数
#define Max_pomp_p 0.20    //ポンプのシャットオフ圧力[MPa] 実際の値は0.344[MPa]（50[PSI]）であるが、余裕を持って設定する。
#define Max_targetlmp 9.5  //流量の最大値[l/m]

//PID制御に用いる定数(各ゲイン:Ki~Kd 出力する操作量の幅:DT_MIN~DT_MAX) ゲイン調整時に水が空気を含まないようにする。
#define Kp 0.02
#define Ki 0.1
#define Kd 0.005
#define DT_MAX 1.0
#define DT_MIN 0.0

//ポンプ制御に用いるPWMピン及び定数
#define Pomp_pwmch 0            //pwmチャンネル
#define Pomp_pwmHz 5000         //pwm周波数
#define Default_Pomp_pwmpin 15  //pwmpin (0,2,4,12,13,14,15,25,26,27,32,33)のいずれかに設定する。
#define Pomp_pwmrenge 12        //pwmの分解能[bit] 8[bit]:0~255でDT比を設定する。
#define Pomp_pwmmax 4095        //pwmの最大値 8[bit]:255 12[bit]:4095

//流量計読み取りに使うピン及び定数
#define Default_Flowmeter_pin 5         //流量計からのパルスを読み取るピン
#define DIV_NUMBER_FOR_LPM_FROM_HZ 4.4  //currentHz[Hz]からcurrentLmp[l/m]に変換する為の定数

//圧力計読み取りに使うピン及び定数
#define Default_Pressure_pin 36         //圧力計からのアナログ信号を読み取るピン
#define Analog_Max_Vout 3.30            //Analog_Max_Vout:Analog値がAnalog_Maxの時の出力電圧[v] promicro:4.56[v] esp32:3.30[v]
#define Analog_Max 4095.0               //Analog値がMaxの時の値 promicro:1023.0 esp32:4095.0
#define Sample_cnt 1000                 //アナログ信号のサンプリング回数
#define Dv_Dp 2.50                      //ゴトウユキトが手動で調整 Dv_Dp:電圧を圧力に変換する定数 esp32:2.50
#define Vout_offset 0.516               //Vout_offset:圧力計起動時のVoutの値(オフセット)

//【重要】CurrentLmp CurrentPress 更新頻度
#define Control_interval 0.8             //Control_interval:タイマー割込み頻度[s] ポンプの制御周期[s]

class Pomp_control{

public:
    Pomp_control(int Flowmeter_Signal_Pin_ = Default_Flowmeter_pin,int Pressure_Signal_Pin_ = Default_Pressure_pin,int Pomp_Pwm_Pin_ = Default_Pomp_pwmpin);
    void begin(void);
    float TargetLmp;                    //TargetLmp:流量の目標値[l/m] 0.0~Max_targetlmp[l/m]までを代入する。
    float CurrentLmp;
    float CurrentPress;
    int Flowmeter_Signal_Pin;           //アクセス禁止です。
    void onRising(void);                //呼び出し禁止です。
    void control_val(void);             //呼び出し禁止です。

private:
    PID *moter;      //PIDmoterクラス
    bool shutoff;
    int Pressure_Signal_Pin;
    int Pomp_Pwm_Pin;
    unsigned long pulseCounter;
    float CurrentHz;
    float DT;                           //DT:ポンプ内のモータの操作量 0.0~1.0が代入される。
    float Vout;                         //Vout:Pressure_Signal_Pinの出力電圧[v]
    void update_power(void);            //update_power:ポンプ内のモータのパワーをDTで更新する。
    void getCurrentLpm(void);
    void Lead_P(void);                  //Lead_P:出力電圧を測定して出力電圧を圧力に変換する関数
};

#endif /*POMP_CONTROL_H*/