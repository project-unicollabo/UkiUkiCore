# ChangeLog

UkiUkiCore の変更履歴です。UkiUkiCore は VTuber「浮々ゆにこ」ファングッズ向け Arduino 互換機
UkiUkiduino（Uno R3 互換）/ UkiUkiduino Pony（Pro Micro 互換）のための Arduino コアで、
[WazamonoCore](https://github.com/ws-asahi/WazamonoCore)（DxCore 1.6 系）から派生しています。

---

## 未リリース — UkiUkiduino Pony（旧 UkiUkimicro）への改名と基板 Rev.0.2 対応

Pro Micro 互換機の製品名が **UkiUkiduino Pony** に決まり、基板 Rev.0.2 でオンボード LED・
ボタンまわりのピン割り当てと電源構成が変わったため、variant・ブートローダー・ドキュメントを
合わせて更新しました。ヘッダに出ているピン（D0〜D16、A0）の番号と機能は変わりません。

### 破壊的変更

- **Pro Micro 互換機の名称を「UkiUkimicro」から「UkiUkiduino Pony」に変更**。
  互換エイリアスはありません。ボードメニューの選び直しと、ボード判定マクロを使っている
  スケッチの書き換えが必要です。
  - FQBN `ukiukimicro` → `ukiukiduinopony`
  - `ARDUINO_AVR_UKIUKIMICRO` → `ARDUINO_AVR_UKIUKIDUINO_PONY`
  - `UKIUKIMICRO_PINOUT` → `UKIUKIDUINO_PONY_PINOUT`
  - `variants/UkiUkimicro/` → `variants/UkiUkiduinoPony/`、ドキュメント `extras/UkiUkiduinoPony.md`
  - ブートローダー `usbcdcboot_ukiukimicro.hex` → `usbcdcboot_ukiukiduinopony.hex`
  - USB 製品名 "UkiUkimicro" → "UkiUkiduino Pony"（ブートローダーは "UkiUkiduino Pony Bootloader"）。
    VID:PID は変更なし（0x1209:0x000E / 0x000D、テスト用 PID）。
- **UkiUkiduino Pony のピン割り当てを基板 Rev.0.2 に合わせて変更**（旧基板とは互換がありません）。
  - `A1` / `A2` / `A3`（D19 / D20 / D21）: PF1 / PF2 / PF3 → **PF0 / PF1 / PF2**
  - `BTN_BUILTIN`（D32）: PF0 → **PA1**（UkiUkiduino と同じ MCU ピン）
  - フルカラー LED のデータ線: PF4 → **PA0**（UkiUkiduino と同じ MCU ピン）
  - `LED_BUILTIN`（D17）の状態ピン: PF5 → **PF3**（基板上は未接続。`A20` としても参照可）
  - イベント出力 EVOUTF（PF2）: A2 → **A3**
  - **TX / RX LED を廃止**: `D30` / `D31`、`LED_BUILTIN_TX` / `LED_BUILTIN_RX` は無くなりました。
    `TXLED0` / `TXLED1` / `RXLED0` / `RXLED1` / `TX_RX_LED_INIT` は何もしないマクロとして残ります。
  - 欠番は D11〜D13 と D25〜D31。MCU の PF4 / PF5 は未接続（Dn 番号なし）。
  - CustomLogic1（LUT3）の入力は A1 / A2 / A3、出力（D17）はヘッダに出ていません。

### ボード

- **UkiUkiduino Pony 基板 Rev.0.2**: MCU は VQFN パッケージ（AVR64DU32-I/RXB）。
  Pro Micro の RAW にあたるピンが **VIN** になり、ショットキーダイオード + 5 V LDO
  （SPX3819M5-L-5-0）経由で給電できます。VCC にあたるピンは +5V です。
- ブートローダー `usbcdcboot_ukiukiduinopony.hex` を再ビルド（LED データ線 PA0、新しい製品名）。
  `usbcdcboot_ukiukiduino.hex` は変更ありません。

### アナログ

WazamonoCore の修正・追加を取り込みました（4fa7dcb8 / cd6915fc / 6b56eec6〜90988267）。

- **`init_ADC0()` が `CLKCTRL.MCLKTIMEBASE` を設定するようになりました**（`wiring.c`）。
  AVR DU の ADC は起動・内部基準電圧のセトリング（40 µs）・温度センサの待ち時間をこのレジスタ
  （1 µs あたりの CLK_PER サイクル数）で計時します（DS40002548B 32.3.2 手順 1、32.3.3.5）。
  DA/DB/DD には無いレジスタで、これまでリセット値 0 のままだったため、内部基準電圧
  （`INTERNAL1V024`〜`INTERNAL4V096`）での変換が 0 または 1023 になっていました。
  既定の `analogReference(VDD)` での `analogRead()` には影響ありません。
- **16 MHz / 12 MHz で ADC クロックが規格上限を超えていたのを修正**（`wiring.c`）。
  プリスケーラが 16 MHz → DIV4（4 MHz）、12 MHz → DIV4（3 MHz）になっていました。
  DU の CLK_ADC 上限 2 MHz（DS40002548B Table 35-23）に合わせ、16 MHz → DIV8、12 MHz → DIV6 とし、
  Clock メニューの 24 / 20 / 16 / 12 MHz すべてで 2 MHz になります。

- **`vddRead()` を追加**（`wiring_analog.c`、`Arduino.h`）。ADC の VDD/10 チャネルを内部 2.048 V 基準で
  1 回変換し、電源電圧を 10 mV 単位で返します（5.00 V → 500）。分圧 ±10 %・基準 ±4 % の目安値です。
  ADC の設定は呼び出し前の状態に戻ります。変換中に呼ぶと 0 を返します。
- **`tempCRead()` / `tempFRead()` を追加**。チップ内蔵温度センサを SIGROW の出荷時校正値で換算し、
  0.1 度単位で返します（253 = 25.3 °C）。ダイ温度で、絶対精度は ±10 °C 程度です。
  変換中に呼ぶと `INT16_MIN` を返します。詳細は `megaavr/extras/Ref_Analog.md`。
  WazamonoCore での実測値を掲載しており、UkiUkiduino 実機での確認はこれからです。

### USB CDC

- **ポートが開いていない間の `Serial` への書き込みを捨てるようになりました**（`usb_cdc.c`、
  WazamonoCore 25c3fbe9 の取り込み）。これまで `usbCdcWrite()` / `usbCdcWriteByte()` はポートの
  状態に関係なくデータを受け取り、EP3 IN に積んでいました。今後は「列挙済み・ホストが DTR を
  保持・バスがサスペンドしていない」ときだけ受け付け、それ以外は捨てます。ATmega32U4 コア
  （Leonardo / Pro Micro）の `Serial_::write()` と同じ規則で、`if (Serial)` / `usbCdcReady()` も
  同じ条件で判定します。**DTR を立てずにポートを開くホスト側ソフトでは受信できなくなる**点に
  注意してください（Arduino IDE のシリアルモニタは DTR を立てます）。
- **バスの SUSPEND / RESUME を処理するようになりました**（`usb_core.c`）。DC ジャックや VIN で
  給電している場合、AVR DU の USB マクロはケーブルを抜いても内蔵レギュレータで動き続けるため、
  3 ms のバス無通信（DS40002548B 28.4.10）がケーブル抜去を知る唯一の手がかりです。サスペンド時は
  送信リングを破棄し、EP3 に積んだパケットを取り下げます。RESUME またはバスリセットで再開し、
  DTR はホストのスリープをまたいで保持するので、開いたままのシリアルモニタは PC の復帰後も
  そのまま使えます。診断用に `usbCdcSuspended()` と、variant 用の weak フック
  `usb_cdc_on_bus_suspend()` を追加（UkiUkiduino / Pony は TX/RX LED が無いので未使用）。

### ドキュメント・サンプル

- `megaavr/extras/UkiUkiduinoPony.md` を基板 Rev.0.2 の内容（ピン表・電源・LED / ボタン・主要部品）に更新。
- CustomLogic / EventSystem / AnalogComp のボード別ピン表に UkiUkiduino Pony を追加し、
  サンプル（DualUnits、PinToPin）の注記を新しいピン割り当てに合わせて修正。
- 回路図・基板データは `hardware/UkiUkiduinoPony/` にあります。

---

## v0.0.5 — 2026-09-14 — UkiUkimicro 追加、Ethernet シールド対応、実機検証の反映

UkiUkiduino の実機で GPIO / ADC / PWM / ボタン / LED / Serial1↔Serial2 / SPI（SD カード・
W5100 Ethernet シールド）/ ウォッチドッグの動作を検証し、そこで見つかった問題を修正した
リリースです。Pro Micro 互換機 **UkiUkimicro** の variant とブートローダーが加わりました。

### 破壊的変更

- **`BTN_BUILTIN` を D32 に変更**（UkiUkiduino: 旧 D21 = PA1、UkiUkimicro: PF0）。
  シリーズ共通の番号にするためで、UkiUkiduino は D21〜D31、UkiUkimicro は D22〜D29 が
  欠番になります。UkiUkimicro の RESET / UPDI は index 33 / 34 です。
  スケッチでは `BTN_BUILTIN` マクロを使っていれば変更不要です。
- **Pro Micro 互換機の名称を「UkiUkiduino ProMicro」から「UkiUkimicro」に変更**。
  開発途中の名称で、v0.0.4 には含まれていないため互換エイリアスはありません。
  FQBN `ukiukimicro`、`ARDUINO_AVR_UKIUKIMICRO`、`UKIUKIMICRO_PINOUT`、
  `variants/UkiUkimicro/`、USB 製品名 "UkiUkimicro" / "UkiUkimicro Bootloader"。
- **手動インストールのツールチェーン配置方法を変更**（下記「ツール」参照）。
  `make_platform_local.bat` と `platform.local.txt` は廃止です。

### ボード

- **UkiUkimicro（Pro Micro 互換）を追加**: AVR64DU32、フルカラー LED（XL-5050RGBC-WS2812B、
  PF4、`LED_BUILTIN` = D17 のミラー）、TX/RX LED（D30/D31）、ボタン（D32）。
  Serial1 = D1/D0、Serial2 = D15(TX)/D18=A0(RX)、SPI = D16/D14/D15。
  ブートローダー `usbcdcboot_ukiukimicro.hex`（VID:PID 0x1209:0x000D、テスト用 PID）。
- **Clock メニューに 20 MHz / 12 MHz を追加**（24 / 20 / 16 / 12 MHz、既定 24 MHz、両ボード）。
  オンボード WS2812 ドライバを F_CPU ごとにサイクル計算した nop 数へ一般化し、
  16 MHz 選択時の `#error` を解消。各クロックの実タイミングは実バイナリの逆アセンブルで
  データシートの範囲内であることを確認済み（実機での LED 点灯確認は 24 MHz のみ）。
  USB は CLK_USB（48 MHz）が OSCHF の固定 4 MHz 出力 + PLL48M から作られ主クロックと独立
  （CLK_PER ≥ 12 MHz、DS40002548B 28.3.1.1）のため、全クロックで USB-CDC が動作します。

### コア

- **ウォッチドッグ互換シム `wdt_compat.h` を WazamonoCore から取り込み**。配布中の
  toolchain（15.2.0-wazamono2）の avr-libc 2.3.2 は WDT 修正（#1065/#1068/#1069）を含まず、
  AVR DU では `wdt_enable()` / `wdt_disable()` の書き込みが CCP ウィンドウ内の
  read-modify-write で黙って落ち、`WDTO_2S` が 0.5 秒になっていました。
  `wdt_enable(WDTO_2S)` / `wdt_reset()` / `wdt_disable()` / `MCUSR & _BV(WDRF)` が
  Uno / Pro Micro と同じ書き方で動くことを実機で確認済み。

### ライブラリ

- **Ethernet（W5100 / W5200 / W5500）を同梱**（arduino-libraries/Ethernet 2.0.2 ベース、
  サンプル 13 本を日本語化）。24 MHz の AVR DU では公式版の `SPISettings(14000000)` が
  12 MHz に丸められ、W5100 が応答しません（レジスタ読み値が 1 ビットずれる。Uno R3 は
  16 MHz → 8 MHz なので顕在化しない）。同梱版は `ARDUINO_ARCH_MEGAAVR` で 8 MHz を要求
  （F_CPU 12/16/20/24 MHz で実効 6/8/5/6 MHz）し、チップセレクトを OUTSET/OUTCLR の
  アトミック書き込みに変更。API は公式版と同一。UkiUkiduino に Uno 用シールドを直挿し
  （CS = D10、SD CS = D4）して W5100 認識・固定 IP・TCP 接続まで実機確認済み。

### ツール / インストール

- **`tools/setup_toolchain.sh` / `tools/setup_toolchain.bat` を追加**。
  `docs/package_ukiuki_index.json` の `toolsDependencies` を読み、wazamono-toolchain の
  リリースから avr-gcc / avrdude を取得（SHA-256 検証）して `hardware/UkiUkiCore/tools/`
  に配置します。Arduino IDE 2 / arduino-cli はこの場所を
  `{runtime.tools.<name>-<version>.path}` として登録するため、コンパイル・スケッチ書き込み・
  ブートローダー書き込みの全てでこのツールが使われます。
  従来の `platform.local.txt` はコンパイルにしか効かず、書き込み時には IDE 側の別 avrdude
  （arduino:avr 更新後は 8.0.0-arduino1）が選ばれる問題がありました。
- `platform.txt` のツール参照をバージョン固定（`avr-gcc-15.2.0-wazamono2` /
  `avrdude-8.1-wazamono2`）に変更。ボードマネージャ版・手動版で同じ解決になります。
- `make_platform_local.bat` を削除。
- `tools/make_release.sh`: インデックスに過去バージョンを残す（ダウングレード可能）、
  ボード一覧を boards.txt から生成、toolchain 版のコメントを wazamono2 に更新。

### ドキュメント

- README とボード別ページ（`megaavr/extras/UkiUkiduino.md` / `UkiUkimicro.md`）の 3 分割に
  再構成。通信（Serial / SPI / I2C）の節を追加。
- サンプルスケッチの日本語化: ClockOut / SPISlave（第 13 弾）、UkiUkimicro のピン注記
  （第 14 弾）、Ethernet 13 本（第 15 弾）。

### 既知の制約（実機検証で確認、仕様として据え置き）

- 24 MHz では USART の最低ボーレートが約 1465 bps で、`Serial1.begin(300)` は約 1659 bps に
  化けます（`begin()` が 4·F_CPU/baud を uint16 に切り詰めるため）。300 / 1200 bps は非対応です。
  影響が小さいためコア側の修正は行いません。
- 2 Mbps は生成・受信できますが割り込み処理が追いつかず、連続受信の実用上限は 1 Mbps 程度です。

---

## v0.0.4 — 2026-09-02

- toolchain を `15.2.0-wazamono2` / `avrdude 8.1-wazamono2` に更新し、ボードマネージャ
  配布を整備（`tools/make_release.sh`、`docs/package_ukiuki_index.json`）。
- UkiUkiduino の variant を WazamonoCore Tsurugi 最新版ベースに再構成
  （ピン再割当、D21 ボタン、Serial2、CCL 経由の TCB1 PWM）。
- オンボード LED（WS2812D-F5-12mA-C1）のドライバとブートローダーの DFU 表示を
  データシートのタイミングに適合。電源 LED を白（KT-0603W）に変更。
- WazamonoCore 上流（2026-07-06〜09-02）をマージ。Wazamono 専用ドキュメントを削除し、
  ボード詳細を README に統合。

## v0.0.3 — 2026-07-19

- 初期公開版。UkiUkiduino（AVR64DU32、Uno R3 互換）の variant、USB-CDC ブートローダー、
  ボードマネージャ用インデックス。
