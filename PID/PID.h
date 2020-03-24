#ifndef PID_H
#define PID_H

class PID{
 
public:

    /*
    Kp 比例ゲイン
    Ki 積分ゲイン
    Kd 微分ゲイン
    dt 制御周期
    sp 目標値
    pv 現在値（現在の入力値）
    */

    PID(float Kp, float Ki, float Kd);                  //コンストラクタでゲイン設定

    void setOutputLimits(float outMin, float outMax);   //出力値の範囲設定
    
    void setGain(float Kp, float Ki, float Kd);         //PIDゲインの再設定
 
    float update_val(float sp,float pv,float dt);       //PIDの計算 戻り値はoutMinからoutMaxの範囲

private:
    bool  Kflag_;           //update初回実行時には偏差が取れないので、Kd,Ki項を無効にする。        

    float Kp_, Ki_, Kd_;    //PIDゲイン
 
    float Point_;           //目標値
 
    float Input_;           //現在の入力値
    float preInput_;        //前回の入力値
    float Output_;          //計算した出力値(outMin_~outMax_)

    float outMin_, outMax_; //出力の最小値,最大値
 
    float pError_;          //現在の入力値と目標値との偏差
    float prepError_;       //前回の入力値と目標値との偏差
    
    float iError_;          //入力値の偏差

    float dError_;          //入力値の偏差

    void setPoint(float sp); //目標の設定

    void setProcessValue(float pv); //現在値の設定
};
 
#endif /* PID_H */