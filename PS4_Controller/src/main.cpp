#include <Arduino.h>
#include <PS4Controller.h>

char esp32_mac_addr[] = "30:ae:a4:08:01:90";

void setup() {
    Serial.begin(9600);
    PS4.begin(esp32_mac_addr);
    Serial.println("Initialization ready!");
}

void loop() {
    if(PS4.isConnected()){
        Serial.println("Controller connected!");
    }else{
        Serial.println("Searching Controller...");
    }
    delay(3000);
}