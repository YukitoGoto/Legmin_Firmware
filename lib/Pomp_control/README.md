# 【概要】
ポンプ制御を扱うライブラリです。

# 【説明】
## 呼び出しok
```
Pomp_control(int,int,int)
```
コンストラクタです。以下の引数を持ちますが、省略可能です。

*Pomp_pwmpin_*:ポンプ制御に使うpwmピン

*Flowmeter_pin_*:流量計からのパルスを読み取るピン

*Pressure_pin_*:圧力計からのアナログ信号を読み取るピン


```
begin()
```
初期化処理です。*setup*関数内で必ず実行してください。


```
TargetLpm
```
流量の目標値[l/m]です。0.0~*Max_targetLpm*[l/m]までを代入してください。


```
CurrentLpm
```
現在の流量[l/m]が*Control_interval*ごとに制御（*TargetLpm*を目指す）され、代入されます。


```
CurrentPress
```
現在の圧力[Mpa]です。この値が*Max_pomp_p*を超えると、ポンプが停止します。

## 呼び出し禁止
```
onRising()
control_val()
```

# 【注意】
**このライブラリだけでは動作しません。**

*PID.h*と組み合わせて使う他、*main.cpp*に以下のコードを追加する必要があります。

# 【main.cpp】
```
//動作に最低限必要なコード
#include <Arduino.h>
#include "Pomp_control.h"

Pomp_control ex;  //Pomp_controlクラスをインスタンス化
Ticker ctr;       //Pomp_controlに必要 ctr:任意のインスタンス名

//loop関数より上に実装する。静的記憶域に居る関数。割込み処理で呼び出す。
void onRising(void){
  ex.onRising();
}

void control_val(void){
  ex.control_val();
}

void setup() {
  //割込み処理を行う。
  attachInterrupt(digitalPinToInterrupt(Default_Flowmeter_pin), onRising, FALLING);
  ctr.attach(Control_interval,control_val);
}

void loop() {
}
```

# 【ファイル構造】
実際にこのライブラリを使う場合、ファイル構造を以下のようにします。

```
---lib---
        |--PID
        |--Pomp_control
```