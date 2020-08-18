//esp32のBluetooth通信には、Bluetoothのmacアドレスを使う必要がある！
#include <Arduino.h>
#include "esp_system.h"

void setup(void) {
    Serial.begin(9600);
}

void loop() {
    uint8_t btMac[6],baseMac[6];

    Serial.println("-----------------------------");
    esp_read_mac(btMac, ESP_MAC_BT);
    Serial.printf("[Bluetooth] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\n", btMac[0], btMac[1], btMac[2], btMac[3], btMac[4], btMac[5]);
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    Serial.printf("[WiFi] Mac Address = %02X:%02X:%02X:%02X:%02X:%02X\n", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);

    //10秒ごとにMACアドレスを表示
    delay(10000);
}