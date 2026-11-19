# UkiUkiduino シリーズ ハードウェア設計データ

UkiUkiduino シリーズの基板設計データ(KiCad プロジェクト)です。
ファームウェア(Arduino コア・ブートローダー)はリポジトリ直下の [`megaavr/`](../megaavr/) にあります。

| ディレクトリ | 製品名 | 概要 | Rev. |
|--------------|--------|------|------|
| [`UkiUkiduino/`](UkiUkiduino/) | UkiUkiduino | AVR64DU32 / Arduino Uno R3 互換 / USB-C | 0.6 |
| [`UkiUkiduinoPony/`](UkiUkiduinoPony/) | UkiUkiduino Pony | AVR64DU32 / Pro Micro 互換 / USB-C | 0.2 |

各ディレクトリには次のファイルが入っています。

- `*.kicad_pro` / `*.kicad_sch` / `*.kicad_pcb` - KiCad 10 のプロジェクト・回路図・基板レイアウト
- `*_BOM_JLCPCB.csv` - 部品表(JLCPCB 形式)
- `*ピン構成.csv` - MCU ピンと Arduino ピン番号の対応表
- `libs/` - プロジェクトローカルの部品ライブラリ

## ライセンス

このディレクトリの設計データは **CERN Open Hardware Licence Version 2 - Permissive
(CERN-OHL-P v2)** で公開しています。全文は [LICENSE](LICENSE) を参照してください。

    Copyright Unicollabo 2026.

    This source describes Open Hardware and is licensed under the CERN-OHL-P v2.

    You may redistribute and modify this source and make products using it
    under the terms of the CERN-OHL-P v2 (https://cern.ch/cern-ohl).

    This source is distributed WITHOUT ANY EXPRESS OR IMPLIED WARRANTY,
    INCLUDING OF MERCHANTABILITY, SATISFACTORY QUALITY AND FITNESS FOR A
    PARTICULAR PURPOSE. Please see the CERN-OHL-P v2 for applicable conditions.

リポジトリのその他の部分(Arduino コア・ブートローダーなど)は LGPL 2.1 です。
[../LICENSE.md](../LICENSE.md) を参照してください。

### CERN-OHL-P v2 の対象に含まれないもの

次のものは第三者に由来するため、上記ライセンスの対象ではありません。それぞれの提供元の条件に従います。

- **`libs/lcsc/` 以下の部品データ(シンボル・フットプリントなど)** - LCSC / EasyEDA の部品ライブラリから
  取り込んだものです。再配布・再利用の条件は提供元の利用規約を確認してください。
- **KiCad 標準ライブラリ由来のシンボル・フットプリント** - KiCad Libraries License
  (CC BY-SA 4.0 + 設計データへの適用を除外する例外条項)に従います。
- **キャラクター「浮々ゆにこ」の名称・イラストなどのアートワーク** - このリポジトリには含めていません。
  製品に印刷しているイラストは本ライセンスの対象外です。
