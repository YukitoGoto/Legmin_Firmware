//spi通信で信号を確認！
#include <Arduino.h>
#include <SPI.h>

long pretime,pretime2;
int send_data;
uint8_t receive_data[8];

void setup() {
  SPI.begin(SCK,MISO,MOSI,SS);  //VSPI - SCK,MISO,MOSI,SS = 18,19,23,5
  SPI.beginTransaction(SPISettings(300000,MSBFIRST,SPI_MODE1));
  Serial.begin(9600);
  pretime2 = pretime = millis();
}

void loop() {
  if(millis() - pretime2 > 150){
    for(int i = 0;i < 8;i++)
      receive_data[i] = SPI.transfer(send_data);
    pretime2 = millis();
  }
  if(millis() - pretime > 1000){
    Serial.printf("\nreceive_data:");
    for(int i = 0;i < 8;i++)
      Serial.printf(" %x",receive_data[i]);
    pretime = millis();
  }
}