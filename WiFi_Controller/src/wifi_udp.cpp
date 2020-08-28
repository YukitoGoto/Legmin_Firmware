#include <WiFi.h>
#include <WiFiUdp.h>

const char ssid[] = "ESP32_wifi";   // SSID
const char pass[] = "daihidaruma";  // password
const int localPort = 10000;        // ポート番号

const IPAddress ip(192, 168, 4, 1);       // IPアドレス(ゲートウェイも兼ねる)
const IPAddress subnet(255, 255, 255, 0); // サブネットマスク

WiFiUDP udp;

void setup() {
    Serial.begin(115200);

    WiFi.softAP(ssid, pass);           // SSIDとパスの設定
    delay(100);                        // 追記：このdelayを入れないと失敗する場合がある
    WiFi.softAPConfig(ip, ip, subnet); // IPアドレス、ゲートウェイ、サブネットマスクの設定

    Serial.printf("-----------------\n");
    Serial.print("AP IP address: ");
    IPAddress myIP = WiFi.softAPIP();
    Serial.println(myIP);

    Serial.println("Starting UDP");
    udp.begin(localPort);  // UDP通信の開始(引数はポート番号)

    Serial.print("Local port: ");
    Serial.println(localPort);
    Serial.printf("-----------------\n");
}

void loop() {
    if(udp.parsePacket()){
        switch(udp.read()){
            case 48:
                Serial.printf("▲\n");
                break;
            case 49:
                Serial.printf("◀\n");
                break;
            case 50:
                Serial.printf("▶\n");
                break;
            case 51:
                Serial.printf("▼\n");
                break;
            case 52:
                Serial.printf("B\n");
                break;
            case 53:
                Serial.printf("A\n");
                break;
            case 54:
                Serial.printf("-----------------\n");  //stop command is 6 (ASCII 0d54)
                break;
            default: break;
        }
    }
}