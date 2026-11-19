/* CustomLogic / DualUnits(2ユニット同時使用)
 *
 * UkiUkiduinoには第1ユニットと独立に動く第2ユニット
 * 「CustomLogic1」があります - ハードウェアゲートを同時に2つ
 * 使えるということです。
 *
 * ピン(UkiUkiduino):
 *                 IN0   IN1   OUT
 *   CustomLogic   D5    D6    D10
 *   CustomLogic1  A0    A1    A3
 *
 * UkiUkiduino Ponyの場合: CustomLogic は同じ(IN0=D5 / IN1=D6 / OUT=D10)。
 *   CustomLogic1 は IN0=A1 / IN1=A2 / IN2=A3 で、OUT(D17 = LED_BUILTINの
 *   状態ピン)はヘッダに出ていません(オンボードLEDも連動しません)。
 *   結果をピンで見るには、setup()の最後に
 *     CustomLogic1.setOutput(D8);   // イベント出力ピン(D8 / D9 / A3 のいずれか)
 *   を追加してください。CustomLogic1.read() での読み出しはそのまま使えます。
 *
 * UkiUkiduino向けに日本語化
 */
#include <CustomLogic.h>

void setup() {
  CustomLogic.begin(AND);     // ユニット0: IN0とIN1が両方HIGHの間だけOUTがHIGH
  CustomLogic1.begin(OR);     // ユニット1: IN0かIN1がHIGHの間OUTがHIGH
}

void loop() {
  // 2つのゲートが同時にハードウェアで動作中 - することはない
}
