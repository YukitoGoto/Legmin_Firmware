# PS4_Controller
PS4コントローラ[DUALSHOCK™4](https://pur.store.sony.jp/ps4/lineup/ps4_controller/)をESP32で使うための情報をまとめます。[PlatformIo](https://platformio.org/)を使用します。

# ライブラリのインストール
## ZIPファイルのダウンロード
[PS4-esp32（GitHub）](https://github.com/aed3/PS4-esp32)にアクセスします。リポジトリをZIPファイル(`PS4-esp32-master.zip`)でダウンロードします。その後、展開してください。
```
Code(緑色のボタン) --> Download ZIP --> すべて展開
```
## ライブラリの置き場所
### 方法1
PlatformIoから任意の名前でプロジェクトを作成します。プロジェクト内に`lib`というフォルダが作成されます。この中に先ほどの展開したファイルを入れます。
```
---ProjectName(任意名)
    :
    |---include
    |---lib
    |    |---PS4-esp32-master
    |
    |---src
    :
```
これでライブラリが読み込まれるようになります。
### 方法2
PlatformIoから任意の名前でプロジェクトを作成します。プロジェクト内に`platformio.ini`という設定ファイルが作成されます。この中で`lib_extra_dirs = ライブラリまでのpath`を指定します。
```
;platformio.ini
;僕の場合の設定です。

[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
lib_extra_dirs = ../lib
```
この場合、先ほどの展開したファイルは次の場所にあります。
```
    :
    |---ProjectName(任意名)
    |       :
    |       |---include
    |       |---lib
    |       |---src
    |       :
    |       |---platformio.ini
    |---lib
    :    |---PS4-esp32-master
```
これでライブラリが読み込まれるようになります。

`platformio.ini`に関する詳細な設定方法は[こちら](https://docs.platformio.org/en/latest/projectconf/section_env_library.html)をご覧ください。
# MACアドレスの登録
## 原理
esp32とDUALSHOCK™4がペアリングするには、**esp32のMACアドレスをDUALSHOCK™4に登録する**必要があります。
## esp32のMACアドレスを取得
`src/main.cpp`を次のように書き換え実行します。
```
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
```
シリアルモニタにMACアドレスが表示されます。**得られたMACアドレスはメモして置いてください。**
## DUALSHOCK™4に登録
[SixaxisPairTool](https://dancingpixelstudios.com/sixaxis-controller/sixaxispairtool/)にアクセスします。インストーラー（SixaxisPairToolSetup-0.x.x.exe）をダウンロード、起動して、適当に次へ次へ...とやっていきます。
```
いづれかのOS（Windows/Linux/MAC OSX）のDownload linkを選択 --> インストーラーを起動 --> 次へ次へ
```
SixaxisPairToolを起動します。DUALSHOCK™4をPCに接続します。ドライバーの自動インストール後に、次のような画面になります。
先ほど得られたMACアドレスを登録しましょう！

![ ](https://github.com/YukitoGoto/Legmin_Firmware/blob/master/PS4_Controller/picture/macaddr.png)

`Current Master`:現在登録されているMACアドレスです。

`Change master`:新たに登録するMACアドレスを正確に記入して、`Update`を押すと登録されます。
```
SixaxisPairToolを起動 --> DUALSHOCK™4をPCに接続（usb）--> ドライバインストール（自動） --> MACアドレス登録
```
# ペアリングの実行
`src/main.cpp`を次のように書き換え実行します。esp32のmacアドレスの部分は皆さんのものに書き換えてください。
```
#include <Arduino.h>
#include <PS4Controller.h>

//your esp32's mac addr
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
```
`ペアリング`:DUALSHOCK™4の`ps4ボタン`を押す。DUALSHOCK™4が白色に点滅後に点灯すればペアリング完了！

`ペアリング切断`:マイコンの`リセットボタン`を押す。しばらくすると、DUALSHOCK™4が消灯する。

**ここまでお疲れ様でした！！**
# 各種関数
**工事中**
# 資料
[参考サイト（DUALSHOCK™4）](https://techtutorialsx.com/2020/02/15/esp32-connecting-a-ps4-controller/)

[PS4-esp32（GitHub）](https://github.com/aed3/PS4-esp32)

[参考サイト（esp32-MACアドレス）](https://mobile.k05.biz/e/2018/12/esp32-mac-addr.html)