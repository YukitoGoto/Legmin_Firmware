//esp32をWiFiに接続。2.4[GHz]帯にのみに対応している。
#include <WiFi.h>

// 接続先のSSIDとパスワード
const char ssid[] = "Buffalo-G-E468";
const char passwd[] = "5vnh66an8fbi6";

void connectWiFi(void);

void setup() {
    Serial.begin(115200);

    // Connect WiFi
    connectWiFi();
}
void loop() {
    // Reconnect
    if ( WiFi.status() == WL_DISCONNECTED ) {
        connectWiFi();
    }
}

/**
 * Connect WiFi
 */
void connectWiFi(void)
{
    WiFi.begin(ssid, passwd);
    Serial.printf("\nWiFi connecting...");
    while(WiFi.status() != WL_CONNECTED) {
        Serial.printf(".");
        delay(100);
    }

    Serial.printf(" connected! \n");
    Serial.println(String("IP:") + WiFi.localIP());
}