//debug room for https://github.com/Legmin/ros-electronic/blob/master/motors/pio/src/main.cpp
#include <Ps3Controller.h>

//analog stick val l & r common
//x:-128 ~ 127 y:127 ~ -128

char mac_adr[] = "aa:bb:cc:dd:ee:ff";
long pretime;
#define Ps3_JOYSTICK_MAX_VALUE 128
enum controllertype {WiredController_asukiaaa,Ps3_x,Ps3_y};

float calcSpeedRate(int16_t joystickValue, int16_t joystickMax,enum controllertype type) {
  float rate,value = joystickValue;
  switch(type){
    case WiredController_asukiaaa:
      rate = (value * 2 - joystickMax) / joystickMax;
      break;
    case Ps3_x:
      if (value < 0.0)
        rate = value / joystickMax;
      else if (value > 0.0)
        rate = value / (joystickMax - 1);
      else
        rate = 0.0;
      break;
    case Ps3_y:
      if (value < 0.0)
        rate = -value / joystickMax;
      else if (value > 0.0)
        rate = -value / (joystickMax - 1);
      else
        rate = 0.0;
      break;
    default: rate = 0.0;
  }
  return rate;
}

void setup(){
    Serial.begin(115200);
    Ps3.begin(mac_adr);
    Serial.println("Ready.");
}

void loop(){
    int16_t lx,ly;
    if(Ps3.isConnected()){
        if(millis() - pretime > 500){
            //check analogstick
            lx = Ps3.data.analog.stick.lx,ly = Ps3.data.analog.stick.ly;
            Serial.print("------------------------\n");
            Serial.print(" lx="); Serial.print(lx, DEC);
            Serial.print(" ly="); Serial.print(ly, DEC);
            Serial.println();
            Serial.print(" rate_lx="); Serial.print(calcSpeedRate(lx,Ps3_JOYSTICK_MAX_VALUE,Ps3_x), 4);
            Serial.print(" rate_ly="); Serial.print(calcSpeedRate(ly,Ps3_JOYSTICK_MAX_VALUE,Ps3_y), 4);
            Serial.println();
            pretime = millis();
        }
    }
}