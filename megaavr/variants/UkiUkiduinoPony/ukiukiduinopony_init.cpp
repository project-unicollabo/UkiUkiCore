/* ukiukiduinopony_init.cpp - board-specific init for the UkiUkiduino Pony
 * ---------------------------------------------------------------------------
 * Part of UkiUkiCore (a product-specific fork of WazamonoCore / SpenceKonde's
 * DxCore). (C) Workshop Asahi 2026. DxCore is (C) Spence Konde, LGPL 2.1
 * (see LICENSE.md).
 *
 * Purpose
 *   Prepare PA0 as the data pin for the on-board XL-5050RGBC-WS2812B
 *   full-color LED and make sure the LED starts dark (a WS2812's power-on
 *   pixel state is undefined, and the bootloader may have left its
 *   breathing color lit).
 *   The LED follows digitalWrite()/digitalWriteFast() calls made to
 *   LED_BUILTIN = D17 (PF3, an MCU pin that is not connected to anything on
 *   the board and is used as the LED's state pin): the core's mirror hook
 *   reads the resulting PF3 OUT bit and sends the matching WS2812 frame on
 *   PA0 (see LED_BUILTIN_MIRROR in pins_arduino.h and
 *   ukiukiduinopony_led.cpp). This keeps the classic "LED_BUILTIN = LED"
 *   experience - the stock Blink sketch blinks the LED (yellow by default)
 *   unmodified - while digitalRead(LED_BUILTIN) still returns the LED state
 *   (the data line itself must idle LOW for the WS2812 latch, so it cannot
 *   carry the state).
 *
 *   The board has no TX/RX activity LEDs, so the USB-CDC activity hooks
 *   (usb_cdc_on_rx_activity / usb_cdc_on_tx_activity / usb_cdc_on_led_tick)
 *   are left at the core's weak no-op defaults.
 *
 * Resource use
 *   None. No EVSYS channel and no CCL LUT is consumed.
 *
 * Note: BTN_BUILTIN (D32 = PA1) needs no init here - it has an external
 *   5.1 kOhm pull-down on the board (pressed = HIGH) and is read as a plain
 *   input.
 */

#include <Arduino.h>

#if defined(UKIUKIDUINO_PONY_PINOUT)

/* ---- Force-link marker ----------------------------------------------------
 * Arduino archives variant-folder objects into core.a, and the core's main.cpp
 * supplies a *weak* initVariant(). An archived strong symbol does NOT override
 * a weak one already provided by a linked object, so without help this entire
 * translation unit is silently dropped at link time. boards.txt passes
 *     -Wl,-u,ukiukiduinopony_variant_keep
 * which forces the linker to pull this member. */
extern "C" { __attribute__((used)) char ukiukiduinopony_variant_keep = 0; }

/* initVariant() is a weak no-op in the core (cores/dxcore/main.cpp); this strong
 * definition overrides it and runs once, immediately after init(), before setup(). */
void initVariant(void) {
  /* WS2812B data pin PA0: output, idle LOW. The pin stays an output
   * permanently; pinMode(LED_BUILTIN, ...) only affects PF3, so the LED simply
   * holds its last mirrored frame if D17 is made an input. */
  VPORTA.OUT &= ~(1 << 0);
  VPORTA.DIR |= (1 << 0);
  /* Blank the LED: PF3 OUT is 0 after reset, so the mirror hook sends the
   * (0,0,0) frame. This clears both the WS2812's undefined power-on state
   * and anything the bootloader's breathing animation left behind. */
  __led_builtin_mirror_hook();
}

#endif /* UKIUKIDUINO_PONY_PINOUT */
