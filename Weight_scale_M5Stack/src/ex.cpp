//サンプルコード
#include <Arduino.h>
#include "Weight_scale_M5Stack.h"

//#include <ros.h>
//#include <std_msgs/Float32.h>

Weight_M5 Pesticide;

//ros::NodeHandle nh;
//std_msgs::Float32 weight_msg;
//ros::Publisher weight_pub("weight_pub",&weight_msg);

void setup() {
  //初期化
  Pesticide.begin();

  //ROS関係
  //nh.getHardware()->setBaud(115200);
  //nh.initNode();
  //nh.advertise(weight_pub);
}

void loop() {
  Pesticide.update_val(); //Pesticide.Weight_が更新される。
  Pesticide.update_lcd(); //Pesticide.Weight_がLcdに表示される。

  //weight_msg.data = Pesticide.Weight_;
  //weight_pub.publish(&weight_msg);
  //nh.spinOnce();
}