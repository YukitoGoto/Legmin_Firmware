#include <Arduino.h>
#include "esp_system.h"

void setup() {
    //baudrate = 9600
    Serial.begin(9600);
}

void loop() {
    // Get MAC address for WiFi station
    uint8_t baseMac[6];
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    char baseMacChr[18] = {0};
    sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);

    Serial.print("\nMAC: ");
    Serial.println(baseMacChr);

    //5秒ごとにMACアドレスを表示
    delay(5000);
}