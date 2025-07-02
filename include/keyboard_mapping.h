#ifndef KEYBOARD_MAPPING_H
#define KEYBOARD_MAPPING_H

#include "tusb.h"
// #include "hid.h"

#include "pico/platform.h"
#include <stdint.h>
#include "keyboard_config.h"

// const uint16_t __in_flash("keymaps_mapping") keymaps_mapping[NUM_KEYS] = {
    
// };


// A1, B1, C1, D1, E1, F1,        
// A2, B2, C2, D2, E2, F2,        
// A3, B3, C3, D3, E3, F3,        
// A4, B4, C4, D4, E4, F4, F5,    
// A5, B5, C5, D5, E5,            
//                           T6  T5, 
//                               T3,
//                       T1, T2, T4, 


#define LEFT 0
#define RIGHT 1

#ifndef KEYBOARD_SIDE
#define KEYBOARD_SIDE 0
#endif

#if KEYBOARD_SIDE == LEFT
const uint16_t __in_flash("keymaps") keymaps_layers[][NUM_KEYS] = {
    [0] = {
        HID_KEY_GRAVE,        //  A1
        HID_KEY_TAB,          //  A2
        HID_KEY_CAPS_LOCK,    //  A3
        HID_KEY_SHIFT_LEFT,   //  A4
        HID_KEY_CONTROL_LEFT, //  A5
        HID_KEY_1,            //  B1
        HID_KEY_Q,            //  B2
        HID_KEY_A,            //  B3
        HID_KEY_Z,            //  B4
        HID_KEY_GUI_LEFT,     //  B5
        HID_KEY_2,            //  C1
        HID_KEY_W,            //  C2
        HID_KEY_S,            //  C3
        HID_KEY_X,            //  C4
        HID_KEY_ALT_LEFT,     //  C5
        HID_KEY_3,            //  D1
        HID_KEY_E,            //  D2
        HID_KEY_D,            //  D3
        HID_KEY_C,            //  D4
        HID_KEY_SPACE,        //  D5
        HID_KEY_4,            //  E1
        HID_KEY_R,            //  E2
        HID_KEY_F,            //  E3
        HID_KEY_V,            //  E4
        HID_KEY_NONE,         //  E5
        HID_KEY_5,            //  F1
        HID_KEY_T,            //  F2
        HID_KEY_G,            //  F3
        HID_KEY_B,            //  F4
        HID_KEY_NONE,         //  F5
        HID_KEY_SPACE,        // T1
        HID_KEY_ESCAPE,       // T2
        HID_KEY_ARROW_DOWN,   // T3
        HID_KEY_DELETE,       // T4
        HID_KEY_ARROW_UP,     // T5
        HID_KEY_KEYPAD_ENTER  // T6
    }};
#else
//       F1, E1, D1, C1, B1, A1, 
//       F2, E2, D2, C2, B2, A2, 
//       F3, E3, D3, C3, B3, A3, 
//   F5, F4, E4, D4, C4, B4, A4, 
//           E5, D5, C5, B5, A5, 
//  T5, T6
//  T3,
//  T4, T2, T1

const uint16_t __in_flash("keymaps") keymaps_layers[][NUM_KEYS] = {
    [0] = {
        HID_KEY_BACKSPACE,     //  A1
        HID_KEY_BACKSLASH,     //  A2
        HID_KEY_ENTER,         //  A3
        HID_KEY_SHIFT_RIGHT,   //  A4
        HID_KEY_CONTROL_RIGHT, //  A5
        HID_KEY_0,             //  B1
        HID_KEY_P,             //  B2
        HID_KEY_SEMICOLON,     //  B3
        HID_KEY_SLASH,         //  B4
        HID_KEY_GUI_RIGHT,     //  B5
        HID_KEY_9,             //  C1
        HID_KEY_O,             //  C2
        HID_KEY_L,             //  C3
        HID_KEY_PERIOD,        //  C4
        HID_KEY_ALT_LEFT,      //  C5
        HID_KEY_8,             //  D1
        HID_KEY_I,             //  D2
        HID_KEY_K,             //  D3
        HID_KEY_COMMA,         //  D4
        HID_KEY_SPACE,         //  D5
        HID_KEY_7,             //  E1
        HID_KEY_U,             //  E2
        HID_KEY_J,             //  E3
        HID_KEY_M,             //  E4
        HID_KEY_NONE,          //  E5
        HID_KEY_6,             //  F1
        HID_KEY_Y,             //  F2
        HID_KEY_H,             //  F3
        HID_KEY_N,             //  F4
        HID_KEY_NONE,          //  F5
        HID_KEY_SPACE,         // T1
        HID_KEY_SPACE,         // T2
        HID_KEY_ARROW_DOWN,    // T3
        HID_KEY_DELETE,        // T4
        HID_KEY_ARROW_UP,      // T5
        HID_KEY_KEYPAD_ENTER   // T6
    }};

#endif

// const uint16_t __in_flash("keymaps") keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
//     [0] = {
//         HID_KEY_6, HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_0, HID_KEY_MINUS,              // HID_KEY_BACKSPACE,
//         HID_KEY_Y, HID_KEY_U, HID_KEY_I, HID_KEY_O, HID_KEY_P, HID_KEY_BRACKET_LEFT,       // HID_KEY_BRACKET_RIGHT,
//         HID_KEY_H, HID_KEY_J, HID_KEY_K, HID_KEY_L, HID_KEY_SEMICOLON, HID_KEY_APOSTROPHE, // HID_KEY_ENTER,
//         HID_KEY_B, HID_KEY_N, HID_KEY_M, HID_KEY_COMMA, HID_KEY_PERIOD, HID_KEY_SLASH,     // HID_KEY_SHIFT_RIGHT,
//         HID_KEY_SPACE, HID_KEY_SPACE, HID_KEY_NONE, HID_KEY_ALT_RIGHT, HID_KEY_NONE, HID_KEY_CONTROL_RIGHT}};

#endif
// const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

//     /* LAYER
//      * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
//      * │ ESC │  1  │  2  │  3  │  4  │  5  │  6  │  7  │  8  │  9  │  0  │  -  │ BS  │
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
//      * │ TAB │  Q  │  W  │  E  │  R  │  T  │  Y  │  U  │  I  │  O  │  P  │  [  │  ]  │
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
//      * │CAPS │  A  │  S  │  D  │  F  │  G  │  H  │  J  │  K  │  L  │  ;  │  '  │ENTER│
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
//      * │SHIFT│  \  │  Z  │  X  │  C  │  V  │  B  │  N  │  M  │  ,  │  .  │  /  │SHIFT│
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┴─────┼─────┼─────┼─────┼─────┼─────┼─────┤
//      * │CTRL │     │ SYS │ ALT │LOWER│   SPACE   │SPACE│RAISE│ ALT │ SYS │     │CTRL │
//      * │     │     │     │     │     │           │     │     │     │     │     │     │
//      * └─────┴─────┴─────┴─────┴─────┴───────────┴─────┴─────┴─────┴─────┴─────┴─────┘
//      */

//     [0] = LAYOUT(
//         //******** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** **********
//         KC_ESC,    KC_1,      KC_2,      KC_3,      KC_4,      KC_5,      KC_6,      KC_7,      KC_8,      KC_9,      KC_0,      KC_MINS,   KC_BSPC,   KC_INS,    KC_HOME,   KC_PGUP,
//         KC_TAB,    KC_Q,      KC_W,      KC_E,      KC_R,      KC_T,      KC_Y,      KC_U,      KC_I,      KC_O,      KC_P,      KC_LBRC,   KC_RBRC,   KC_DEL,    KC_END,    KC_PGDN,
//         KC_CAPS,   KC_A,      KC_S,      KC_D,      KC_F,      KC_G,      KC_H,      KC_J,      KC_K,      KC_L,      KC_SCLN,   KC_QUOT,   KC_ENT,
//         KC_LSFT,   KC_BSLS,   KC_Z,      KC_X,      KC_C,      KC_V,      KC_B,      KC_N,      KC_M,      KC_COMM,   KC_DOT,    KC_SLSH,   KC_RSFT,              KC_UP,
//         KC_LCTL,   KC_NO,     KC_LGUI,   KC_LALT,   MO(1),            KC_SPC,        KC_SPC,    MO(2),     KC_RALT,   KC_RGUI,   KC_NO,     KC_RCTL,   KC_LEFT,   KC_DOWN,   KC_RGHT
//     ),

//     /* FUNCTION LAYER
//      * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┐
//      * │PAUSE│ F1  │ F2  │ F3  │ F4  │ F5  │ F6  │ F7  │ F8  │ F9  │ F10 │ F11 │ F12 │   │ PRT │     │     │
//      * │PLAY │     │     │     │     │     │     │     │     │     │     │     │     │   │ SCR │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┤
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │   │     │     │     │
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │   │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤   └─────┴─────┴─────┘
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤         ┌─────┐
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │         │     │
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │         │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┴─────┼─────┼─────┼─────┼─────┼─────┼─────┤   ┌─────┼─────┼─────┐
//      * │     │     │     │     │     │           │     │     │     │     │     │     │   │     │     │     │
//      * │     │     │     │     │     │           │     │     │     │     │     │     │   │     │     │     │
//      * └─────┴─────┴─────┴─────┴─────┴───────────┴─────┴─────┴─────┴─────┴─────┴─────┘   └─────┴─────┴─────┘
//      */

//     [1] = LAYOUT(
//         //******** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** **********
//         KC_MPLY,   KC_F1,     KC_F2,     KC_F3,     KC_F4,     KC_F5,     KC_F6,     KC_F7,     KC_F8,     KC_F9,     KC_F10,    KC_F11,    KC_F12,    KC_PSCR,   KC_NO,     KC_NO,
//         KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
//         KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
//         KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,                KC_NO,
//         KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_TRNS,          KC_NO,         KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO
//     ),

//     /* SYMBOLS LAYER
//      * ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐   ┌─────┬─────┬─────┐
//      * │     │     │     │     │     │     │     │     │     │  `  │  =  │  +  │     │   │     │     │     │
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │   │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤   ├─────┼─────┼─────┤
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │   │     │     │     │
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │   │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤   └─────┴─────┴─────┘
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤         ┌─────┐
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │         │     │
//      * │     │     │     │     │     │     │     │     │     │     │     │     │     │         │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┴─────┼─────┼─────┼─────┼─────┼─────┼─────┤   ┌─────┼─────┼─────┐
//      * │     │     │     │     │     │           │     │     │     │     │     │     │   │     │     │     │
//      * │     │     │     │     │     │           │     │     │     │     │     │     │   │     │     │     │
//      * └─────┴─────┴─────┴─────┴─────┴───────────┴─────┴─────┴─────┴─────┴─────┴─────┘   └─────┴─────┴─────┘
//      */

//     [2] = LAYOUT(
//         //******** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** ********** **********
//         KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_GRV,    KC_EQL,    KC_PPLS,   KC_NO,     KC_NO,     KC_NO,     KC_NO,
//         KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
//         KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,
//         KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,                KC_NO,
//         KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,            KC_NO,         KC_NO,     KC_TRNS,   KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO,     KC_NO
//     )
// };
