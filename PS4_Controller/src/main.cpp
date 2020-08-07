#include <Arduino.h>
#include <PS4Controller.h>

//#define GET_MAC_ADDR
#ifdef GER_MAC_ADDR
#include "esp_system.h"
void get_mad_addr();
#endif //GET_MAC_ADDR

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

#ifdef GET_MAC_ADDR
void get_mac_addr() {
  uint8_t baseMac[6];
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  char baseMacChr[18] = {0};
  sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);

  Serial.print("\nMAC: ");
  Serial.println(baseMacChr);
}
#endif //GET_MAC_ADDR