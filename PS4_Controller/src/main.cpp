#include <Arduino.h>
#include <PS4Controller.h>

//your esp32 bluetooth mac addr
char esp32_btmac_addr[] = "30:ae:a4:08:01:90";

//count
uint32_t cnt;

//time
uint64_t pre_time;

void setup() {
    Serial.begin(9600);
    PS4.begin(esp32_btmac_addr);
    cnt = 0;
    pre_time = millis();
    Serial.printf("\nInitialization ready!\n");
}

void loop() {
    if(PS4.isConnected()){
        //Serial.println("Controller connected!");
        cnt++;
    }else{
        //Serial.println("Searching Controller...");
    }
    if(millis() - pre_time > 3000){
        pre_time = millis();
        Serial.printf("Connected count:%d\n",cnt);
        cnt = 0;
    }
}