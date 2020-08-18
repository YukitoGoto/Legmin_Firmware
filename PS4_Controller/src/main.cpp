#include <Arduino.h>
#include <PS4Controller.h>

//your esp32 bluetooth mac addr
char esp32_btmac_addr[] = "30:ae:a4:08:01:90";

void setup() {
    Serial.begin(9600);
    PS4.begin(esp32_btmac_addr);
    Serial.println("Initialization ready!");
}

void loop() {
    if(PS4.isConnected()){
        Serial.println("Controller connected!");
    }else{
        Serial.println("Searching Controller...");
    }
    delay(1000);
}