#include <Ps3Controller.h>

//analog stick val l & r common
//x:-128 ~ 127 y:127 ~ -128

char mac_adr[] = "aa:bb:cc:dd:ee:ff";
long pretime;

void setup(){
    Serial.begin(115200);
    Ps3.begin(mac_adr);
    Serial.println("Ready.");
}

void loop(){
    if(Ps3.isConnected()){
        if(millis() - pretime > 500){
            //check analogstick
            Serial.print("------------------------\n");
            Serial.print(" lx="); Serial.print(Ps3.data.analog.stick.lx, DEC);
            Serial.print(" ly="); Serial.print(Ps3.data.analog.stick.ly, DEC);
            Serial.print(" rx="); Serial.print(Ps3.data.analog.stick.rx, DEC);
            Serial.print(" ry="); Serial.print(Ps3.data.analog.stick.ry, DEC);
            Serial.println();
            pretime = millis();
        }
    }
}