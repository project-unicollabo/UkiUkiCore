# UkiUkiduino Pony

**Pro Micro 互換機 — AVR64DU32 / USB-C**

UkiUkiduino Pony は、UkiUkiduino シリーズの第二機種で、SparkFun Pro Micro と同じフォームファクタを  
USB ネイティブな AVR `AVR64DU32` で再設計したボードです。  
MCU・クロック・USB・書き込み方式・オンボード LED / ボタンの API は UkiUkiduino と共通で、  
ピン配置とフォームファクタを Pro Micro(ATmega32U4 版)に合わせてあります。  
電源は USB-C のほか、Pro Micro の RAW にあたる **VIN ピン**からも供給できます(5 V レギュレータ搭載)。  
旧称は「UkiUkiduino ProMicro」/「UkiUkimicro」です。  

このページは UkiUkiduino Pony 専用のドキュメントです。コア全体の概要・インストール・共通 API は [README](../../README.md) を参照してください。  
**状態: 製造中(基板 Rev.0.2)** 到着後に実機確認を行います。回路図・基板データは [hardware/UkiUkiduinoPony](../../hardware/UkiUkiduinoPony) にあります。  

---

## 概要

| 項目 | 値 |
|------|----|
| フォームファクタ | Pro Micro 互換(35.0 × 17.8 mm、12 ピン × 2 列、USB-C 側に 2 mm 延長) |
| MCU / クロック / USB | UkiUkiduino と同一(AVR64DU32、内蔵 24 MHz、USB 2.0 FS) |
| 電源 | USB-C(5 V)または **VIN ピン**(ダイオード + 5 V LDO)。I/O は 5 V |
| オンボード LED | フルカラー LED(WS2812B、`LED_BUILTIN` = D17)、電源 LED(白) |
| オンボードボタン | `BTN_BUILTIN` = **D32**(押下 = HIGH) |
| 書き込み | USB ブートローダー / UPDI(4 ピン UPDI ヘッダ: RESET / +5V / GND / UPDI) |

> Pro Micro にある TX / RX LED は搭載していません(該当ピンはフルカラー LED とボタンに使用)。  
> ヘッダの並びは Pro Micro と同じで、RAW の位置が **VIN**、VCC の位置が **+5V** です。


---

## ピンマッピング

Pro Micro(SparkFun / ATmega32U4)と同じ番号付けです。D11〜D13 は存在しません。  
A0〜A3 は D18〜D21 を兼ね、その他のデジタルピンはアナログ A6〜A19 を兼ねます(A4 / A5 はありません)。  

| ピン名 | MCU | 別名 | ADC ch | 主な機能 |
|--------|-----|-----|--------|----------|
| D0 | PA5 | A11 | AIN25 | **RX**(Serial1) / **MOSI**(SPI1) |
| D1 | PA4 | A12 | AIN24 | **TX**(Serial1) / **MISO**(SPI1) |
| D2 | PA2 | A13 | AIN22 | **SDA**(I2C) |
| D3 | PA3 | A14 | AIN23 | **SCL**(I2C) / PWM(TCB1、既定の出口) |
| D4 | PC3 | A6 | AIN31 | PWM(TCB1 → CCL LUT1 経由) / AC0 AINP4 |
| D5 | PD0 | A15 | AIN0 | PWM / CCL(LUT2-IN0) |
| D6 | PD1 | A7 | AIN1 | PWM / CCL(LUT2-IN1) |
| D7 | PA6 | A16 | AIN26 | PWM(TCB1 → CCL LUT0 経由) / XCK(Serial1) / SCK(SPI1) |
| D8 | PA7 | A8 | AIN27 | XDIR(Serial1) / AC0 OUT / EVOUTA / CLKOUT |
| D9 | PD2 | A9 | AIN2 | PWM / CCL(LUT2-IN2) / AC AINP0 / EVOUTD |
| D10 | PD3 | A10 | AIN3 | PWM / CCL(LUT2-OUT) / AC AINN0 |
| D14 | PD5 | A17 | AIN5 | PWM / **MISO**(SPI) |
| D15 | PD6 | A18 | AIN6 | **SCK**(SPI) / TX(Serial2) |
| D16 | PD4 | A19 | AIN4 | PWM / **MOSI**(SPI) |
| A0 | PD7 | D18 | AIN7 | **A0** / SPI ハードウェア SS / RX(Serial2) / VREFA |
| A1 | PF0 | D19 | AIN16 | **A1** / CCL(LUT3-IN0) |
| A2 | PF1 | D20 | AIN17 | **A2** / CCL(LUT3-IN1) |
| A3 | PF2 | D21 | AIN18 | **A3** / CCL(LUT3-IN2) / EVOUTF |
| D17 | PF3 | A20 | AIN19 | **LED_BUILTIN**(フルカラー LED の状態ピン。物理ピンなし・基板上は未接続) |
| D32 | PA1 | — | — | **BTN_BUILTIN**(物理ピンなし) / CCL(LUT0-IN1) |
| — | PA0 | — | — | フルカラー LED のデータ線(Dn 番号なし。D17 への書き込みに連動) |

> MCU の PF4 / PF5 は基板上で未接続です(Dn 番号なし)。  
> TX / RX LED が無いため `D30` / `D31`・`LED_BUILTIN_TX` / `LED_BUILTIN_RX` は定義されません。  
> `TXLED0` / `TXLED1` / `RXLED0` / `RXLED1` / `TX_RX_LED_INIT` は、Pro Micro 用スケッチがそのまま  
> コンパイルできるよう「何もしないマクロ」として用意してあります。  

> **D3 / D4 / D7 の PWM は択一**です(UkiUkiduino の D3 / D4 / D7 と同じ仕組み)。  
> 最後に `analogWrite()` したピンが出口になり、`tone()` 実行中は 3 本とも停止します。  
> Pro Micro 本来の PWM ピン(D3 / D5 / D6 / D9 / D10)はすべて使え、さらに D4 / D7 / D14 / D16 が加わります。  
>  
> **Pro Micro に AREF 専用ピンはありません。** 外部基準電圧を使う場合は A0(PD7 = VREFA)に入力し、  
> その間 A0 / SPI SS / Serial2 RX は使えません。  
>  
> オンボード LED は D17 への digitalWrite に連動します。D17(PF3)自体はどこにも配線されていない  
> 「状態ピン」で、コアがその出力値を読み取ってフルカラー LED(データ線 PA0)へ送信します。  
>  
> CustomLogic の第 2 ユニット(CustomLogic1 = LUT3)は入力が A1 / A2 / A3 で、出力(D17)はヘッダに  
> 出ていません。結果をピンに出すには `setOutput()` でイベント出力ピン(D8 / D9 / A3)を指定してください。  

---

### シリアルポート

| オブジェクト | 実体 | ピン | 備考 |
|--------------|------|------|------|
| `Serial` | USB CDC | USB-C | シリアルモニタ(仮想 COM) |
| `Serial1` | USART0 | D0(RX) / D1(TX) | Pro Micro 互換ハードウェア UART |
| `Serial2` | USART1 | A0(RX) / D15(TX) | 追加 UART。SPI の SCK / SS と共用(排他) |

> Serial1 は XCK(D7) / XDIR(D8) と併用して **RS-485 の方向制御や SPI ホストモード(SPI1)にも対応**。  
>  
> 通信速度は 2400bps から 1000000bps までが安定領域です。  
> 動作クロックを落とす場合上限は下がります。  

---

### SPI

| 信号 | SPI | SPI1(USART0) |
|------|------|------|
| MOSI | D16 | D0 |
| MISO | D14 | D1 |
| SCK | D15 | D7 |
| SS | A0(D18) | なし |

> **クライアント(受信側)動作:** ハードウェア SS が A0 に出ているため、  
> 付属の **SPISlave ライブラリ**(ESP8266 互換 API)で SPI スレーブとしても動作できます。  
> その間 A0 ピンは SS 入力となり、GPIO / Serial2 / AREF とは排他です。  
>  
> 詳細は [libraries/SPISlave](../libraries/SPISlave/README.md) を参照。  

---

### I2C(Wire)

| 信号 | ピン |
|------|------|
| SDA | D2 |
| SCL | D3 |

> Pro Micro と同じ D2 / D3 に配置されています。**Uno 形 UkiUkiduino(A4 / A5)とは異なります。**  
> Wire と Serial1 / Serial2 / SPI はピンを共有しないので、すべて同時に使えます。  

---

### PWM(`analogWrite()`)

- **D5 / D6 / D9 / D10 / D14 / D16** - TCA0(D14 / D16 は SPI と排他)
- **D3 / D4 / D7** - TCB1 の 8bit PWM 波形を直接または LUT 経由で出力(排他使用)
- Pro Micro の PWM ピン(D3 / D5 / D6 / D9 / D10)はすべて再現されています。

> `tone()` は TCB1 を使うため、実行中は D3 / D4 / D7 の PWM が停止します(TCA0 側は継続)。  

---

### アナログ入力

- 10-bit ADC
- Pro Micro ヘッダの **A0–A3**(= D18–D21)
- 各デジタルピンも ADC チャネルを持ち、A6–A19 として参照可能(A4 / A5 はありません)

---

### クロック出力(CLKOUT)

- メインクロック(CLK_PER)を **D8** へ出力できます(ClockOut ライブラリ)。D8 は AC0 出力・EVOUTA と共用です。

---

## 電源

+5V 系統には 2 つの入力があります。

- **USB-C(5 V)**: VBUS は理想ダイオード IC(CH213K、電流制限約 1.3 A、逆流阻止)を通して +5V になります。
- **VIN ピン**(Pro Micro の RAW の位置): ショットキーダイオード(B5819W、逆接続保護)と 5 V LDO  
  (SPX3819M5-L-5-0、最大 500 mA・入力最大 16 V)を通して +5V になります。
  - 入力は **6〜9 V を推奨**(上限の目安 12 V)。ダイオードと LDO の電圧降下があるため、5 V を得るには約 6 V 以上が必要です。
  - LDO は小型パッケージ(SOT-23-5)のため、VIN が高いほど取り出せる電流が発熱で制限されます。  
    損失はおよそ (VIN − 5.4 V) × 負荷電流 です。データシートの熱抵抗(191 °C/W)から計算した目安は、  
    周囲 40 °C で 7 V: 約 250 mA / 9 V: 約 120 mA / 12 V: 約 60 mA までです(計算値。実機での測定値ではありません)。
- USB と VIN を同時に接続しても、CH213K が USB 側への逆流を防ぐため問題ありません。
- **+5V ピン**(Pro Micro の VCC の位置)は +5V 系統そのもので、MCU の VDD に直結しています。  
  ここへ外部 5 V を直接供給することもできますが、**5.5 V を超える電圧を加えないでください**(MCU の絶対最大定格)。
- 3.3 V 出力はありません(USB 用の 3.3 V は MCU 内蔵レギュレータが VUSB ピンに生成します)。

---

## LED とスイッチ

| 部品 | 接続 | 用途 |
|------|------|------|
| **LED_BUILTIN** | D17(Active-HIGH) | ユーザー用フルカラー LED(WS2812B) |
| 電源 LED | +5V | 通電表示(白) |
| **BTN_BUILTIN** | **D32**(Push-HIGH) | ユーザー用ボタン |
| リセット | RESET | ボタン(ダブルタップでブートローダ) |

> `setBLEDColor()` による色・明るさ指定、`digitalWrite(LED_BUILTIN, HIGH/LOW)` による点灯・消灯、  
> ブートローダ DFU 時の黄色ブレス点灯は UkiUkiduino と同一です。  
> UkiUkiduino ライブラリのサンプル(BLEDColorCycle 等)はそのまま動作します。  
>  
> フルカラー LED のデータ線(PA0)とボタン(PA1 = D32)は Uno 形の UkiUkiduino と同じ MCU ピンです。  


---

## 書き込み

- USB ブートローダ(USB-CDC / STK500v1)。RESET ダブルタップでブートローダに入り、フルカラー LED が黄色にブレス点灯します。
- ブートローダ書き換えは基板端の 4 ピン UPDI ヘッダ(RESET / +5V / GND / UPDI)から UPDI プログラマで行います。
- ヘッダの RST ピンとリセットボタンは 330 Ω を介して MCU の RESET に接続されています(UPDI ヘッダの RESET は MCU に直結)。

---

## ソフトウェア互換性(Pro Micro)

- ピン番号・`LED_BUILTIN` / `BTN_BUILTIN`・`Serial` / `Serial1` の意味は Pro Micro と揃えてあるので、  
  多くのスケッチは無改変で動きます。  
- `A6`〜`A10` は Pro Micro と同じ物理ピン(D4 / D6 / D8 / D9 / D10)を指します。  
- ボード判定は `#if defined(ARDUINO_AVR_UKIUKIDUINO_PONY)` を使ってください。  
- TX / RX LED はありません。`TXLED1` などのマクロは何もしません(コンパイルは通ります)。`LED_BUILTIN_TX` / `LED_BUILTIN_RX` を  
  直接使うスケッチは、その部分を削除するか `LED_BUILTIN` に置き換えてください。  
- RAW ピンは VIN(5 V LDO の入力)です。Pro Micro と同様に 5 V より高い電圧を入力できます(「電源」を参照)。  
- 32U4 特有のレジスタ操作・`USBCON` の直接操作は動作しません。  


---

## 主要部品

| 部品 | 型番 | 備考 |
|------|------|------|
| MCU | AVR64DU32(T)-I/RXB | VQFN-32(5 × 5 mm) |
| USB コネクタ | Hirose CX90M-16P | ミッドマウント USB-C 16P |
| USB 保護 | USBLC6-2SC6 | ESD 保護 |
| USB 電源スイッチ | WCH CH213K | 理想ダイオード / 電流制限 |
| VIN 入力ダイオード | B5819W | ショットキー(逆接続保護) |
| 5 V レギュレータ | SPX3819M5-L-5-0 | LDO、SOT-23-5、最大 500 mA |
| フルカラー LED | XINGLIGHT XL-5050RGBC-WS2812B | 5050 SMD、GRB 順、データ線 PA0(330 Ω 直列) |
| 電源 LED | KT-0603W(白) | 5 V から 5.1 kΩ |
| ボタン | TS-1088-AR02016 ×2 | RESET / BTN_BUILTIN(5.1 kΩ プルダウン) |
| 基板 | 35.0 × 17.78 mm、1.2 mm 厚 | 市販 USB-C 版 Pro Micro と同寸 |

---

## 公式ドキュメント

- AVR64DU32 製品ページ: <https://www.microchip.com/en-us/product/AVR64DU32>
- データシート: DS40002548(AVR64DU28/32)
- LED データシート: XINGLIGHT XL-5050RGBC-WS2812B(LCSC C2843785)
- USB コネクタ: Hirose CX90M-16P Design Guide(推奨板厚 0.8 mm、本機は 1.2 mm で実装実績あり)
