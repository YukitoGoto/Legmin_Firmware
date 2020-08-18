#include <Arduino.h>
#include "esp_system.h"

void setup(void) {
    Serial.begin(9600);
}

void loop() {
    uint8_t btMac[6],wifistaMac[6],wifisapMac[6],ethMac[6];

    Serial.println("-----------------------------");
    esp_read_mac(btMac, ESP_MAC_BT);
    Serial.printf("[Bluetooth] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\n", btMac[0], btMac[1], btMac[2], btMac[3], btMac[4], btMac[5]);
    esp_read_mac(wifistaMac, ESP_MAC_WIFI_STA);
    Serial.printf("[WiFi Station] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\n", wifistaMac[0], wifistaMac[1], wifistaMac[2], wifistaMac[3], wifistaMac[4], wifistaMac[5]);
    esp_read_mac(wifisapMac, ESP_MAC_WIFI_SOFTAP);
    Serial.printf("[WiFi Softap] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\n", wifisapMac[0], wifisapMac[1], wifisapMac[2], wifisapMac[3], wifisapMac[4], wifisapMac[5]);
    esp_read_mac(ethMac, ESP_MAC_ETH);
    Serial.printf("[Ethernet] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\n", ethMac[0], ethMac[1], ethMac[2], ethMac[3], ethMac[4], ethMac[5]);
    //10秒ごとにMACアドレスを表示
    delay(10000);
}
/*
[Bluetooth] Mac Address = 30:AE:A4:08:01:92
[WiFi Station] Mac Address = 30:AE:A4:08:01:90
[WiFi Softap] Mac Address = 30:AE:A4:08:01:91
[Ethernet] Mac Address = 30:AE:A4:08:01:93
*/