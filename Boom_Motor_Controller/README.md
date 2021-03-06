# Boom_Moter_Controller
esp32上でモーターを制御します。リミットスイッチを取り付けて、始端終端自動停止を行います。
# Circuit
## チャタリング対策
スイッチのチャタリング防止として、各**Digital信号入力ピン**は以下の様に接続されます。

![Chattering_prevention](https://github.com/YukitoGoto/Legmin_Firmware/blob/master/Boom_Motor_Controller/circuits/Chattering_prevention.png)

### Vcc
電源電圧です。esp32では3.3[v]です。
### Vout
Digital信号入力ピンにつなぎます。ピンアサインを示します。
|SW|esp32のピン|
|:---:|:---:|
|始端リミットスイッチ(LIMIT_START_PIN)|8|
|終端リミットスイッチ(LIMIT_FINISH_PIN)|9|
### SW
リミットスイッチや、モード選択（自動or手動）スイッチ等がここに接続されます。
### 真理値表
|SW|Vout[v]|※T(on/off)[msec]|
|:---:|:---:|:---:|
|ON|0(LOW)|0.1|
|OFF|Vcc(HIGH)|1.1|

※目安として以下の公式を示す。
```
T(on/off):スイッチの入力が反映されるまでの遅れ時間
Ton = R2 × C1
Toff = (R1 + R2) × C1
```