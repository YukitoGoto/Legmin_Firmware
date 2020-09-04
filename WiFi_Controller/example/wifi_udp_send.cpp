#include <WiFi.h>
#include <WiFiUdp.h>

const char ssid[] = "ESP32_wifi";   // SSID
const char pass[] = "daihidaruma";  // password
const int esp32Port = 10000;        // esp32側のポート番号
const int phonePort = 8080;         // スマホ側のポート番号

const IPAddress esp32ip(192, 168, 4, 1);  // IPアドレス(ゲートウェイも兼ねる)
const IPAddress subnet(255, 255, 255, 0); // サブネットマスク
const IPAddress phoneip(192, 168, 4, 2);  // スマホ側のIPアドレス

void udp_send(uint8_t *data,uint8_t arry,IPAddress remoteip,uint16_t remotePort);

WiFiUDP udp;

void setup() {
    Serial.begin(115200);

    WiFi.softAP(ssid, pass);           // SSIDとパスの設定
    delay(100);                        // 追記：このdelayを入れないと失敗する場合がある
    WiFi.softAPConfig(esp32ip, esp32ip, subnet); // IPアドレス、ゲートウェイ、サブネットマスクの設定

    Serial.printf("-----------------\n");
    Serial.print("AP IP address: ");
    IPAddress myIP = WiFi.softAPIP();
    Serial.println(myIP);

    Serial.println("Starting UDP");
    udp.begin(esp32Port);  // UDP通信の開始(引数はポート番号)

    Serial.print("esp32 port: ");
    Serial.println(esp32Port);
    Serial.printf("-----------------\n");
}

void loop(){
    uint8_t txt[][255] = {"Hello,This is ESP32!\n","I'm sending data!\n"};
    udp_send(txt[0],sizeof(txt[0])/sizeof(txt[0][0]),phoneip,phonePort);
    delay(1000);
    udp_send(txt[1],sizeof(txt[1])/sizeof(txt[1][0]),phoneip,phonePort);
    delay(1000);
}

void udp_send(uint8_t *data,uint8_t arry,IPAddress remoteip,uint16_t remotePort){
    udp.beginPacket(remoteip, remotePort);
    for(uint8_t cnt = 0;cnt < arry;cnt++)
        udp.write(data[cnt]);
    udp.endPacket();
}