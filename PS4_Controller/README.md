# PS4_Controller
PS4コントローラ[DUALSHOCK™4](https://pur.store.sony.jp/ps4/lineup/ps4_controller/)をESP32で使うための情報をまとめます。`PlatformIo`を使用します。

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
# MACアドレスの書き換え

# ペアリングの実効

# 資料
[参考サイト](https://techtutorialsx.com/2020/02/15/esp32-connecting-a-ps4-controller/)

[PS4-esp32（GitHub）](https://github.com/aed3/PS4-esp32)