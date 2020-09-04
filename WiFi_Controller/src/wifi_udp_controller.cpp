#include <WiFi.h>
#include <WiFiUdp.h>

const char ssid[] = "ESP32_wifi";   // SSID
const char pass[] = "daihidaruma";  // password
const int esp32Port = 10000;        // esp32側のポート番号
const int phonePort = 8080;         // スマホ側のポート番号

const IPAddress esp32ip(192, 168, 4, 1);  // IPアドレス(ゲートウェイも兼ねる)
const IPAddress subnet(255, 255, 255, 0); // サブネットマスク
const IPAddress phoneip(192, 168, 4, 2);  // スマホ側のIPアドレス

WiFiUDP udp;

void wifi_controll(void);
bool ros_status;   //true:on false:off
uint32_t cnt_x;    //x長押し判定

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

    ros_status = false;
    Serial.printf("ROS is OFF\n");
    Serial.printf("-----------------\n");
}

void loop() {
    wifi_controll();
}

void wifi_controll(void){
    if(ros_status){
        if(udp.parsePacket()){
            switch(udp.read()){
                case 54:    //'6'
                    Serial.printf("X\n");   //btn12
                    cnt_x++;
                    if(cnt_x > 15U){
                        //ROSで操作終了
                        ros_status = false;
                        cnt_x = 0U;
                        Serial.printf("ROS is OFF\n");
                        Serial.printf("-----------------\n");
                        delay(2000);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    while(!ros_status){
        if(udp.parsePacket()){
            switch(udp.read()){
                case 48:    //'0'(ASCII 0d48)
                    Serial.printf("▲\n");   //btn2
                    //前進
                    break;
                case 49:    //'1'
                    Serial.printf("◀\n");   //bnt4
                    //左旋回
                    break;
                case 50:    //'2'
                    Serial.printf("▶\n");   //bnt6
                    //右旋回
                    break;
                case 51:    //'3'
                    Serial.printf("▼\n");   //bnt8
                    //後進
                    break;
                case 52:    //'4'
                    Serial.printf("A\n");   //btn17
                    break;
                case 53:    //'5'
                    Serial.printf("B\n");   //bnt20
                    break;
                case 54:    //'6'
                    Serial.printf("X\n");   //btn12
                    //ROSで操作開始
                    ros_status = true;
                    Serial.printf("ROS is ON\n");
                    Serial.printf("-----------------\n");
                    break;
                case 55:    //'7'
                    Serial.printf("Y\n");   //bnt15
                    break;
                case 115:
                    Serial.printf("-----------------\n");  //stop command is 's' (ASCII 0d115)
                    break;
                default: break;
                    //ブレーキ
            }
            if(ros_status)
                return;
        }
        else{
        //ブレーキ
        }
    }
    return;
}