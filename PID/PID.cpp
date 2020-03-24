#include "PID.h"
 
PID::PID(float Kp, float Ki, float Kd){
    Kp_ = Kp; Ki_ = Ki; Kd_ = Kd;

    Kflag_ = false;

    //累積の初期化
    iError_ = 0.0;
}
 
void PID::setOutputLimits(float outMin, float outMax){
    if(outMin >= outMax)
        return;

    outMin_  = outMin;
    outMax_  = outMax;
}
 
void PID::setPoint(float sp){
    Point_ = sp;
}

void PID::setProcessValue(float pv)
{
    preInput_ = Input_;
    Input_ = pv;
}

void PID::setGain(float Kp,float Ki,float Kd) {
    Kp_ = Kp; Ki_ = Ki; Kd_ = Kd;
}
 
float PID::update_val(float sp,float pv,float dt){
    setPoint(sp);
    setProcessValue(pv);

    pError_ = Point_ - Input_;                          //比例（現在の偏差）
    dError_ = (pError_ - prepError_) / dt;              //微分 (偏差の差)
    iError_ += ((prepError_ + pError_) * dt) / 2.0;     //積分（偏差を台形近似）
    prepError_ = pError_;                               //更新（現在の偏差を過去の偏差へ）

    if(Kflag_== false){
        Output_ = Kp_ * pError_;
        Kflag_ = true;
    }
    else{
        Output_ = Kp_ * pError_ + Ki_ * iError_ + Kd_ * dError_;
    }

    if(Output_ > outMax_)
        return(outMax_);
    else if(Output_ < outMin_)
        return(outMin_);
    else{
        return(Output_);
    }
}