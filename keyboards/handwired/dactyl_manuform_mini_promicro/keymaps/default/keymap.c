#include "dactyl_manuformone_mini_promicro.h"

#define GUI_TAB_TIMER_WAIT_MS 500

#define BSPC_LAYER_2 LT(2, KC_BSPC)
#define ENT_LAYER_1 LT(1, KC_ENT)

enum custom_keycodes {          // Make sure have the awesome keycode ready
  GUI_TAB = SAFE_RANGE,
  MACOS_COPY,
  MACOS_PASTE,
  MACOS_CHANGE_WINDOW,
  TMUX_NORMAL_MODE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	KEYMAP(
		KC_QUOT, KC_COMM, KC_DOT, KC_P, KC_Y, KC_F, KC_G, KC_C, KC_R, KC_L,
		KC_A, KC_O, KC_E, KC_U, KC_I, KC_D, KC_H, KC_T, KC_N, KC_S,
		KC_SLSH, KC_Q, KC_J, KC_K, KC_X, KC_B, KC_M, KC_W, KC_V, KC_Z,
		KC_SPC, MT(MOD_LALT, KC_DOWN), MT(MOD_LGUI, KC_UP), MT(MOD_LCTL, KC_ESC), MT(MOD_LSFT, KC_TAB), MT(MOD_RSFT, KC_ESC), ENT_LAYER_1, MT(MOD_RGUI, KC_LEFT), MT(MOD_RALT, KC_RGHT), BSPC_LAYER_2), 

  KEYMAP(
		KC_LBRC, KC_7, KC_8, KC_9, KC_RBRC, TMUX_NORMAL_MODE, KC_TRNS, MACOS_COPY, KC_TRNS, KC_TRNS,
		KC_SCLN, KC_4, KC_5, KC_6, KC_EQL, KC_TRNS, GUI_TAB, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_GRV, KC_1, KC_2, KC_3, KC_BSLS, KC_TRNS, MACOS_CHANGE_WINDOW, KC_TRNS, MACOS_PASTE, KC_TRNS,
		KC_0, KC_TRNS, KC_TRNS, KC_MINS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS),

	KEYMAP(
		KC_LCBR, KC_AMPR, KC_ASTR, KC_LPRN, KC_RCBR, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_COLN, KC_DOLLAR, KC_PERC, KC_CIRC, KC_PLUS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_PIPE, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_RPRN, KC_TRNS, KC_TRNS, KC_UNDS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS), 

};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case BSPC_LAYER_2:
        return 110;
      case ENT_LAYER_1:
        return 110;
      default:
        return TAPPING_TERM;
    }
}

bool is_gui_tab_active = false;
uint16_t gui_tab_timer = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case GUI_TAB:
      if (record->event.pressed) {
        if (!is_gui_tab_active) {
          is_gui_tab_active = true;
          register_code(KC_LGUI);
        }
        gui_tab_timer = timer_read();
        register_code(KC_TAB);
      } else {
        unregister_code(KC_TAB);
      }
      break;
    case MACOS_COPY:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI("c"));
      }
      break;
    case MACOS_PASTE:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI("v"));
      }
      break;
    case MACOS_CHANGE_WINDOW:
      if (record->event.pressed) {
        SEND_STRING(SS_LGUI("`"));
      }
      break;
    case TMUX_NORMAL_MODE:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTRL("a")"[");
      }
      break;
  }
  return true;
}

void matrix_scan_user(void) {
  if (is_gui_tab_active) {
    if (timer_elapsed(gui_tab_timer) > GUI_TAB_TIMER_WAIT_MS) {
      unregister_code(KC_LGUI);
      is_gui_tab_active = false;
    }
  }
}
