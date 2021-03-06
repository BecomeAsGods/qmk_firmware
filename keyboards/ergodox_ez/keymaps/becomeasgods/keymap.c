/*
 * algernon's ErgoDox EZ layout, please see the readme.md file!
 */

#include <stdarg.h>
#include QMK_KEYBOARD_H
#include "led.h"
#include "debug.h"
#include "action_layer.h"
#include "action_util.h"
#include "timer.h"
#include "eeconfig.h"
#include "wait.h"
#include "version.h"
#include "print.h"

/* Layers */

enum {
  BASE = 0,
  ARRW,
  GAME,
  NMDIA,
};

/* Macros */

enum {
  NONE = 0,
  // Buttons that do extra stuff
  A_MPN,

  // number/symbol keys
  A_1, // 1
  A_2, // 2
  A_3, // ...
  A_4,
  A_5,
  A_6,
  A_7,
  A_8,
  A_9,
  A_0,

  // Fx
  Fx,
};

/* Fn keys */

enum {
  F_BSE = 0,
  F_SFT,
  F_ALT,
  F_CTRL
};

/* Custom keycodes */

enum {
  CT_CLN = 0,
  CT_TA,
  CT_LBP,
  CT_RBP,
  CT_SR,
};

/* States & timers */

#if KEYLOGGER_ENABLE
# ifdef AUTOLOG_ENABLE
bool log_enable = true;
# else
bool log_enable = false;
# endif
#endif

bool time_travel = false;
bool skip_leds = false;

/* The Keymap */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Keymap 0: Base Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | Next/Prev | 9    | 7  @ | 5  * | 3  ^ | 1  $ | F11  |           |  Fx  | 0  % | 2  ! | 4  # | 6  & | 8    |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |         ~ |   '  |   ,  |   .  |   P  |   Y  |   (  |           |  )   |   F  |   G  |   C  |   R  |  L   | \         |
 * |-----------+------+------+------+------+------|   [  |           |  ]   |------+------+------+------+------+-----------|
 * | Tab/ARROW |   A  |   O  |   E  |   U  |   I  |------|           |------|   D  |   H  |   T  |   N  |  S   | = / Arrow |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * | Play/Pause|   /  |   Q  |   J  |   K  |   X  |      |           |      |   B  |   M  |   W  |   V  |  Z   | Stop/Reset|
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     |       |      |      |      |   :  |                                       |   -  |      |      |      |       |
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | LAlt | GUI  |           | MDIA | Del  |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | Ctrl |           | LEAD |      |      |
 *                                  |Backsp|LShift|------|           |------| Enter| Space|
 *                                  |      |      | ESC  |           | GAME |      |      |
 *                                  `--------------------'           `--------------------'
 */
[BASE] = LAYOUT_ergodox(
// left hand
 M(A_MPN)           ,M(A_9)      ,M(A_7)      ,M(A_5)  ,M(A_3)  ,M(A_1)  ,KC_F11
,KC_GRV             ,KC_QUOT     ,KC_COMM     ,KC_DOT  ,KC_P    ,KC_Y    ,TD(CT_LBP)
,TD(CT_TA)          ,KC_A        ,KC_O        ,KC_E    ,KC_U    ,KC_I
,KC_MPLY            ,KC_SLSH     ,KC_Q        ,KC_J    ,KC_K    ,KC_X    ,KC_NO
,KC_NO              ,KC_NO       ,KC_NO       ,KC_NO   ,TD(CT_CLN)

                                                            ,F(F_ALT),KC_LGUI
                                                                     ,F(F_CTRL)
                                                    ,KC_BSPC,F(F_SFT),KC_ESC

                                                                // right hand
                                                               ,M(Fx)     ,M(A_0)  ,M(A_2)    ,M(A_4)  ,M(A_6)  ,M(A_8)   ,KC_NO
                                                               ,TD(CT_RBP),KC_F    ,KC_G      ,KC_C    ,KC_R    ,KC_L     ,KC_BSLS
                                                                          ,KC_D    ,KC_H      ,KC_T    ,KC_N    ,KC_S     ,KC_EQL
                                                               ,KC_NO     ,KC_B    ,KC_M      ,KC_W    ,KC_V    ,KC_Z     ,TD(CT_SR)
                                                                                   ,KC_MINS   ,KC_NO   ,KC_NO   ,KC_NO    ,KC_NO

                                                               ,OSL(NMDIA),KC_DEL
                                                               ,KC_LEAD
                                                               ,TG(GAME)  ,KC_ENT ,KC_SPC
    ),

/* Keymap 1: Arrow layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      | Home |  Up  |  End |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |------|           |------|      | Left | Down | Rght |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |      |      |
 *                                  | Enter|      |------|           |------| PgUp | PgDn |
 *                                  |      |      |      |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */

[ARRW] = LAYOUT_ergodox(
// left hand
 KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS    ,KC_TRNS    ,KC_TRNS
,KC_TRNS ,KC_TRNS    ,KC_TRNS   ,KC_TRNS    ,KC_TRNS

                                             ,KC_TRNS ,KC_TRNS
                                                      ,KC_TRNS
                                    ,KC_ENT  ,KC_TRNS ,KC_TRNS

                                                                // right hand
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_HOME ,KC_UP   ,KC_END  ,KC_TRNS ,KC_TRNS
                                                                        ,KC_TRNS ,KC_LEFT ,KC_DOWN ,KC_RGHT ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS
                                                                                 ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS ,KC_TRNS

                                                               ,KC_TRNS ,KC_TRNS
                                                               ,KC_TRNS
                                                               ,KC_TRNS ,KC_PGUP ,KC_PGDN
    ),


/* Keymap 2: Gaming Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |  Ő   |      |  Ű   |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |   Á  |  Ó   |  É   |  Ú   |  Í   |------|           |------|      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |  Ö   |      |  Ü   |      |      |           |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |      |      |
 *                                  |      |      |------|           |------|      |      |
 *                                  |      |      |      |           | BASE |      |      |
 *                                  `--------------------'           `--------------------'
 */

[GAME] = LAYOUT_ergodox(
// left hand
 KC_ESC             ,KC_9        ,KC_7        ,KC_5    ,KC_3    ,KC_1    ,KC_F11
,KC_GRV             ,KC_QUOT     ,KC_COMM     ,KC_DOT  ,KC_P    ,KC_Y    ,KC_F1
,KC_TAB             ,KC_A        ,KC_O        ,KC_E    ,KC_U    ,KC_I
,KC_LSFT            ,KC_SLSH     ,KC_Q        ,KC_J    ,KC_K    ,KC_X    ,KC_SCLN
,KC_LBRC            ,KC_F2       ,KC_F3       ,KC_LALT ,KC_LCTL

                                                            ,KC_LALT ,KC_ENT
                                                                     ,KC_LCTL
                                                    ,KC_BSPC,SH_MON  ,SH_TG

                                                                // right hand
                                                               ,KC_F7     ,KC_0    ,KC_2      ,KC_4    ,KC_6    ,KC_8     ,TG(GAME)
                                                               ,KC_F4     ,KC_F    ,KC_G      ,KC_C    ,KC_R    ,KC_L     ,KC_BSLS
                                                                          ,KC_D    ,KC_H      ,KC_T    ,KC_N    ,KC_S     ,KC_EQL
                                                               ,KC_MINS   ,KC_B    ,KC_M      ,KC_W    ,KC_V    ,KC_Z     ,KC_LSFT
                                                                                   ,KC_LCTL   ,KC_LALT ,KC_F6   ,KC_F5    ,KC_RBRC

                                                               ,KC_ENT    ,KC_LALT
                                                               ,KC_LCTL
                                                               ,SH_TG     ,MO(ARRW) ,KC_SPC
    ),

/* Keymap 3: Navigation & Media layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |  F9  |  F7  |  F5  |  F3  |  F1  |ScrLCK|           |      | F10  |  F2  |  F4  |  F6  |  F8  |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |------|           |------|      |      |      |      |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *      |      |      |      |      |      |                                       |      |      |      |      |      |
 *      `----------------------------------'                                       `----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | Mute | VlUp |           | BASE |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | VlDn |           |      |      |      |
 *                                  |      |      |------|           |------|      |      |
 *                                  |      |      |      |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */
[NMDIA] = LAYOUT_ergodox(
// left hand
 KC_NO      ,KC_F9       ,KC_F7      ,KC_F5   ,KC_F3   ,KC_F1   ,LGUI(KC_L)
,KC_NO      ,KC_NO       ,KC_NO      ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
,KC_NO      ,KC_NO       ,KC_NO      ,KC_NO   ,KC_NO   ,KC_NO
,KC_NO      ,KC_NO       ,KC_NO      ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO
,KC_NO      ,KC_NO       ,KC_NO      ,KC_NO   ,KC_NO
                                                        ,KC_MUTE ,KC_VOLU
                                                                 ,KC_VOLD
                                                 ,KC_NO ,KC_NO   ,KC_TRNS

                                                                     // right hand
                                                                     ,KC_TRNS   ,KC_F10  ,KC_F2   ,KC_F4   ,KC_F6   ,KC_F8    ,KC_NO
                                                                     ,KC_NO     ,KC_NO   ,KC_NO   ,KC_UP   ,KC_NO   ,KC_NO    ,KC_NO
                                                                                ,KC_NO   ,KC_LEFT ,KC_DOWN ,KC_RGHT ,KC_NO    ,KC_NO
                                                                     ,KC_NO     ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO    ,KC_NO
                                                                                         ,KC_NO   ,KC_NO   ,KC_NO   ,KC_NO    ,KC_NO

                                                                     ,KC_TRNS   ,KC_NO
                                                                     ,KC_NO
                                                                     ,KC_NO     ,KC_NO   ,KC_NO
    ),

};

const uint16_t PROGMEM fn_actions[] = {
   [F_BSE]  = ACTION_LAYER_CLEAR(ON_PRESS)
  ,[F_SFT]  = ACTION_MODS_ONESHOT (MOD_LSFT)
  ,[F_ALT]  = ACTION_MODS_ONESHOT (MOD_LALT)
  ,[F_CTRL] = ACTION_MODS_ONESHOT (MOD_LCTL)
};

static void ang_handle_num_row(uint8_t id, keyrecord_t *record) {
  uint8_t idx = id - A_1;
  uint8_t kc;
  static bool shifted[10];

  if (get_mods() & MOD_BIT(KC_LSFT) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
    if (record->event.pressed)
      shifted[idx] = true;
  }

  if (!shifted[idx]) {
    kc = idx + KC_1;
  } else {
    switch (id) {
    case A_8:
    case A_9:
      shifted[idx] = false;
      return;

    case A_7:
      kc = KC_2;
      break;
    case A_5:
      kc = KC_8;
      break;
    case A_3:
      kc = KC_4;
      break;
    case A_1:
      kc = KC_6;
      break;

    case A_0:
      kc = KC_5;
      break;
    case A_2:
      kc = KC_1;
      break;
    case A_4:
      kc = KC_3;
      break;
    case A_6:
      kc = KC_7;
      break;
    }
  }

  if (record->event.pressed) {
    register_code (kc);
  } else {
    unregister_code (kc);
    shifted[idx] = false;
  }
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
      switch(id) {
      case A_MPN:
        if (record->event.pressed) {
          if (get_mods() & MOD_BIT(KC_LSFT) ||
              ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
            int oneshot = ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out());

            if (oneshot)
              clear_oneshot_mods ();
            unregister_code (KC_LSFT);

            register_code (KC_MPRV);
            unregister_code (KC_MPRV);

            if (!oneshot)
              register_code (KC_LSFT);
          } else {
            return MACRO (T(MNXT), END);
          }
        }
        break;

                /* Fx */
      case Fx:
        if (record->event.pressed) {
          set_oneshot_mods (MOD_LALT);
          layer_on (NMDIA);
          set_oneshot_layer (NMDIA, ONESHOT_START);
        } else {
          clear_oneshot_layer_state (ONESHOT_PRESSED);
        }
        break;

        // number row and symbols
      case A_1 ... A_0:
        ang_handle_num_row(id, record);
        break;
      }

      return MACRO_NONE;
};

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {

  set_unicode_input_mode(UC_LNX);

  ergodox_led_all_on();
  for (int i = LED_BRIGHTNESS_HI; i > LED_BRIGHTNESS_LO; i--) {
    ergodox_led_all_set (i);
    wait_ms (5);
  }
  wait_ms(1000);
  for (int i = LED_BRIGHTNESS_LO; i > 0; i--) {
    ergodox_led_all_set (i);
    wait_ms (10);
  }
  ergodox_led_all_off();

  if (!eeconfig_is_enabled())
    eeconfig_init();
};

LEADER_EXTERNS();

static void ang_tap (uint16_t code, ...) {
  uint16_t kc = code;
  va_list ap;

  va_start(ap, code);

  do {
    register_code16(kc);
    unregister_code16(kc);
    wait_ms(50);
    kc = va_arg(ap, int);
  } while (kc != 0);
  va_end(ap);
}

#define TAP_ONCE(code)  \
  register_code (code); \
  unregister_code (code)

typedef struct {
  bool layer_toggle;
  bool sticky;
} td_ta_state_t;

static void ang_tap_dance_ta_finished (qk_tap_dance_state_t *state, void *user_data) {
  td_ta_state_t *td_ta = (td_ta_state_t *) user_data;

  if (td_ta->sticky) {
    td_ta->sticky = false;
    td_ta->layer_toggle = false;
    layer_off (ARRW);
    return;
  }

  if (state->count == 1 && !state->pressed) {
    register_code (KC_TAB);
    td_ta->sticky = false;
    td_ta->layer_toggle = false;
  } else {
    td_ta->layer_toggle = true;
    layer_on (ARRW);
    td_ta->sticky = (state->count == 2);
  }
}

static void ang_tap_dance_ta_reset (qk_tap_dance_state_t *state, void *user_data) {
  td_ta_state_t *td_ta = (td_ta_state_t *) user_data;

  if (!td_ta->layer_toggle)
    unregister_code (KC_TAB);
  if (!td_ta->sticky)
    layer_off (ARRW);
}

static void
_td_sr_each (qk_tap_dance_state_t *state, void *user_data) {
  skip_leds = true;

  switch (state->count) {
  case 1:
    ergodox_right_led_3_on ();
    break;
  case 2:
    ergodox_right_led_2_on ();
    break;
  case 3:
    ergodox_right_led_1_on ();
    break;
  case 4:
    ergodox_right_led_3_off ();
    wait_ms (50);
    ergodox_right_led_2_off ();
    wait_ms (50);
    ergodox_right_led_1_off ();
    break;
  }
}

static void
_td_sr_finished (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    register_code (KC_MSTP);
  }
  if (state->count >= 4) {
    uprintf("CMD:reflash\n");
    wait_ms (1000);
    reset_keyboard ();
    reset_tap_dance (state);
  }
}

static void
_td_sr_reset (qk_tap_dance_state_t *state, void *user_data) {
  ergodox_right_led_1_off ();
  wait_ms (50);
  ergodox_right_led_2_off ();
  wait_ms (50);
  ergodox_right_led_3_off ();
  wait_ms(1000);
  skip_leds = false;

  if (state->count == 1) {
    unregister_code (KC_MSTP);
  }
}

static void
_td_brackets_finished (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    if (state->keycode == TD(CT_LBP))
      register_code16 (KC_LBRC);
    else
      register_code16 (KC_RBRC);
  } else if (state->count == 2) {
    if (state->keycode == TD(CT_LBP))
      register_code16 (KC_LPRN);
    else
      register_code16 (KC_RPRN);
  } else if (state->count == 3) {
    unicode_input_start();

    if (state->keycode == TD(CT_LBP))
      register_hex (0x300c);
    else
      register_hex (0x300d);

    unicode_input_finish();
  }
}

static void
_td_brackets_reset (qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    if (state->keycode == TD(CT_LBP))
      unregister_code16 (KC_LBRC);
    else
      unregister_code16 (KC_RBRC);
  } else if (state->count == 2) {
    if (state->keycode == TD(CT_LBP))
      unregister_code16 (KC_LPRN);
    else
      unregister_code16 (KC_RPRN);
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
   [CT_CLN] = ACTION_TAP_DANCE_DOUBLE (KC_COLN, KC_SCLN)
  ,[CT_TA]  = {
     .fn = { NULL, ang_tap_dance_ta_finished, ang_tap_dance_ta_reset },
     .user_data = (void *)&((td_ta_state_t) { false, false })
   }
  ,[CT_LBP] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, _td_brackets_finished, _td_brackets_reset)
  ,[CT_RBP] = ACTION_TAP_DANCE_FN_ADVANCED (NULL, _td_brackets_finished, _td_brackets_reset)
  ,[CT_SR]  = ACTION_TAP_DANCE_FN_ADVANCED (_td_sr_each, _td_sr_finished, _td_sr_reset)
};

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  uint8_t layer = biton32(layer_state);
  //bool is_arrow = false;

  if (!skip_leds) {
    if (layer == GAME) {
      ergodox_right_led_2_on();
      ergodox_right_led_3_on();
      //TODO
      // if (swap_hands) {
      // ergodox_right_led_1_on ();
      // }
    } else if (layer == ARRW) {
      ergodox_right_led_1_on ();
      ergodox_right_led_3_on ();
    } else if (layer == NMDIA) {
      ergodox_right_led_1_on();
      ergodox_right_led_2_on();
    }
  }

  // if (layer_state & (1UL << ARRW)) {
  //   if (!skip_leds) {
  //     ergodox_right_led_1_on ();
  //     ergodox_right_led_3_on ();
  //   }
  //   is_arrow = true;
  // }

  if (!skip_leds) {
    if (get_mods() & MOD_BIT(KC_LSFT) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && !has_oneshot_mods_timed_out())) {
      ergodox_right_led_1_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_1_on ();
    } else {
      ergodox_right_led_1_set (LED_BRIGHTNESS_LO);
      if (layer != ARRW && layer != NMDIA)
        ergodox_right_led_1_off ();
    }

    if (get_mods() & MOD_BIT(KC_LALT) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LALT)) && !has_oneshot_mods_timed_out())) {
      ergodox_right_led_2_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_2_on ();
    } else {
      ergodox_right_led_2_set (LED_BRIGHTNESS_LO);
      if (layer != GAME && layer != NMDIA)
        ergodox_right_led_2_off ();
    }

    if (get_mods() & MOD_BIT(KC_LCTRL) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LCTRL)) && !has_oneshot_mods_timed_out())) {
      ergodox_right_led_3_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_3_on ();
    } else {
      ergodox_right_led_3_set (LED_BRIGHTNESS_LO);
      if (layer != GAME && layer != ARRW)
        ergodox_right_led_3_off ();
    }
  }

  LEADER_DICTIONARY() {
    leading = false;
    leader_end ();

    SEQ_ONE_KEY (KC_C) {
      ang_tap (LSFT(KC_C), KC_S, KC_I, KC_L, KC_L, KC_RALT, KC_QUOT, KC_A, KC_M, KC_A, KC_S,
               KC_S, KC_Z, KC_O, KC_N, KC_Y, KC_K, KC_RALT, KC_QUOT, KC_A, KC_M, 0);
    }

    SEQ_ONE_KEY (KC_K) {
      ang_tap (KC_SPC, LSFT(KC_7), KC_SPC, 0);
      register_code(KC_LCTL);
      register_code(KC_LSFT);
      register_code(KC_U);
      unregister_code(KC_U);
      unregister_code(KC_LSFT);
      unregister_code(KC_LCTL);
      ang_tap (KC_1, KC_F, KC_4, KC_7, KC_6, 0);
      register_code (KC_ENT);
      unregister_code (KC_ENT);
      ang_tap (KC_END, 0);
      register_code(KC_LCTL);
      register_code(KC_LSFT);
      register_code(KC_U);
      unregister_code(KC_U);
      unregister_code(KC_LSFT);
      unregister_code(KC_LCTL);
      ang_tap (KC_1, KC_F, KC_4, KC_7, KC_6, 0);
      register_code (KC_SPC);
      unregister_code (KC_SPC);
    }

    SEQ_ONE_KEY (KC_G) {
      ang_tap (LSFT(KC_G), KC_E, KC_J, KC_G, KC_RALT, KC_EQL, KC_O,
               KC_RALT, KC_EQL, KC_O,
               KC_RALT, KC_EQL, KC_O, 0);
    }

#if KEYLOGGER_ENABLE
    SEQ_ONE_KEY (KC_D) {
      ergodox_led_all_on();
      wait_ms(100);
      ergodox_led_all_off();
      log_enable = !log_enable;
    }
#endif

    SEQ_ONE_KEY (KC_R) {
      ang_tap(KC_RALT, 0);
    }

    SEQ_ONE_KEY (KC_T) {
      time_travel = !time_travel;
    }

    SEQ_ONE_KEY (KC_U) {
      qk_ucis_start();
    }

    SEQ_ONE_KEY (KC_V) {
      SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ (" QMK_VERSION "/" LAYOUT_ergodox_VERSION ")");
    }

    SEQ_ONE_KEY (KC_L) {
      /* λ */
      unicode_input_start();
      register_hex(0x03bb);
      unicode_input_finish();
    }

    SEQ_ONE_KEY (KC_Y) {
      ang_tap (KC_BSLS, KC_O, KC_SLSH, 0);
    }

    SEQ_ONE_KEY (KC_S) {
      unicode_input_start(); register_hex(0xaf); unicode_input_finish();
      TAP_ONCE (KC_BSLS);
      register_code (KC_RSFT); TAP_ONCE (KC_MINS); TAP_ONCE (KC_9); unregister_code (KC_RSFT);
      unicode_input_start (); register_hex(0x30c4); unicode_input_finish();
      register_code (KC_RSFT); TAP_ONCE (KC_0); TAP_ONCE (KC_MINS); unregister_code (KC_RSFT);
      TAP_ONCE (KC_SLSH);
      unicode_input_start (); register_hex(0xaf); unicode_input_finish();
    }

    SEQ_TWO_KEYS (KC_W, KC_M) {
      uprintf("CMD:wm\n");
    }
  }
}

static uint16_t last4[4];

const qk_ucis_symbol_t ucis_symbol_table[] = UCIS_TABLE
(
 UCIS_SYM("poop", 0x1f4a9),
 UCIS_SYM("rofl", 0x1f923),
 UCIS_SYM("kiss", 0x1f619),
 UCIS_SYM("snowman", 0x2603),
 UCIS_SYM("coffee", 0x2615),
 UCIS_SYM("heart", 0x2764),
 UCIS_SYM("bolt", 0x26a1),
 UCIS_SYM("pi", 0x03c0),
 UCIS_SYM("mouse", 0x1f401),
 UCIS_SYM("micro", 0x00b5),
 UCIS_SYM("tm", 0x2122),
 UCIS_SYM("child", 0x1f476),
 UCIS_SYM("family", 0x1F46A),
 UCIS_SYM("joy", 0x1F602)
);

bool process_record_user (uint16_t keycode, keyrecord_t *record) {
#if KEYLOGGER_ENABLE
  if (log_enable) {
    uint8_t layer = biton32(layer_state);

    if (layer == BASE)
      uprintf ("KL: col=%02d, row=%02d, pressed=%d, layer=%s\n", record->event.key.col,
               record->event.key.row, record->event.pressed, "Dvorak");
  }
#endif

  if (keycode == KC_ESC && record->event.pressed) {

    if ((get_oneshot_mods ()) && !has_oneshot_mods_timed_out ()) {
      clear_oneshot_mods ();
    }
  }

  if (time_travel && !record->event.pressed) {
    uint8_t p;

    // shift cache one to the left
    for (p = 0; p < 3; p++) {
      last4[p] = last4[p + 1];
    }
    last4[3] = keycode;

    if (last4[0] == KC_D && last4[1] == KC_A && last4[2] == KC_T && last4[3] == KC_E) {
      ang_tap (KC_E, KC_SPC, KC_MINS, KC_D, KC_SPC, KC_QUOT, 0);
      register_code (KC_RSFT);
      register_code (KC_EQL);
      unregister_code (KC_EQL);
      unregister_code (KC_RSFT);

      ang_tap (KC_4, KC_SPC, KC_D, KC_A, KC_Y, KC_S, KC_QUOT, 0);

      return false;
    }
  }

  return true;
}

void qk_ucis_symbol_fallback (void) {
  for (uint8_t i = 0; i < qk_ucis_state.count - 1; i++) {
    uint8_t code;

    if ((qk_ucis_state.codes[i] >= M(A_1)) && (qk_ucis_state.codes[i] <= M(A_0)))
      code = qk_ucis_state.codes[i] - M(A_1) + KC_1;
    else
      code = qk_ucis_state.codes[i];
    register_code(code);
    unregister_code(code);
    wait_ms (10);
  }
}