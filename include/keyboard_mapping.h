#ifndef KEYBOARD_MAPPING_H
#define KEYBOARD_MAPPING_H

#include "tusb.h"
// #include "hid.h"

#include "pico/platform.h"
#include <stdint.h>
#include "keyboard_config.h"

// A1, B1, C1, D1, E1, F1,
// A2, B2, C2, D2, E2, F2,
// A3, B3, C3, D3, E3, F3,
// A4, B4, C4, D4, E4, F4, F5,
// A5, B5, C5, D5, E5,
//                           T6  T5,
//                               T3,
//                       T1, T2, T4,

//     /* LAYER
//      * ┌─────┬─────┬─────┬─────┬─────┬─────┐
//      * │ A1  │ B1  │ C1  │ D1  │ E1  │ F1  │
//      * │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┤
//      * │ A2  │ B2  │ C2  │ D2  │ E2  │ F2  │
//      * │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┤
//      * │ A3  │ B3  │ C3  │ D3  │ E3  │ F3  ├─────┐
//      * │     │     │     │     │     │     | F5  │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┤     │
//      * │ A4  │ B4  │ C4  │ D4  │ E4  │ F4  ├─────┘
//      * │     │     │     │     │     │     |
//      * ├─────┼─────┼─────┼─────┼─────┼─────┘
//      * │ A5  │ B5  │ C5  │ D5  │ E5  │
//      * │     │     │     │     │     │       ┌─────┬─────┐
//      * └─────┴─────┴─────┴─────┴─────┘       │ T6  │ T5  │
//      *                                       │     │     │
//      *                                 ┌─────┼─────┼─────┤
//      *                                 │     │     │ T3  │
//      *                                 │     │     │     │
//      *                                 | T1  | T2  ├─────┤
//      *                                 │     │     │ T4  │
//      *                                 │     │     │     │
//      *                                 └─────┴─────┴─────┘
//      */
#if KEYBOARD_SIDE == LEFT
const uint16_t __in_flash("keymaps") keymaps_layers[][NUM_KEYS] = {
    [0] = {
        //     /* LAYER
        //      * ┌─────┬─────┬─────┬─────┬─────┬─────┐
        //      * │ =   │  1  │  2  │  3  │  4  │  5  │
        //      * │     │     │     │     │     │     │
        //      * ├─────┼─────┼─────┼─────┼─────┼─────┤
        //      * │ TAB │  Q  │  W  │  E  │  R  │  T  │
        //      * │     │     │     │     │     │     │
        //      * ├─────┼─────┼─────┼─────┼─────┼─────┤
        //      * │ ESC │  A  │  S  │  D  │  F  │  G  ├─────┐
        //      * │     │     │     │     │     │     | F5  │
        //      * ├─────┼─────┼─────┼─────┼─────┼─────┤     │
        //      * │SHIFT│  Z  │  X  │  C  │  V  |  B  ├─────┘
        //      * │     │     │     │     │     │     │
        //      * ├─────┼─────┼─────┼─────┼─────┼─────┘
        //      * │CTRL │ SYS │ ALT │LOWER│SPACE│
        //      * │     │     │     │     │     │       ┌─────┬─────┐
        //      * └─────┴─────┴─────┴─────┴─────┘       │ T6  │ T5  │
        //      *                                       │     │     │
        //      *                                 ┌─────┼─────┼─────┤
        //      *                                 │     │     │HOME │
        //      *                                 │     │     │     │
        //      *                                 |BSPC | DEL ├─────┤
        //      *                                 │     │     │ END │
        //      *                                 │     │     │     │
        //      *                                 └─────┴─────┴─────┘
        //      */
        //      */
        HID_KEY_EQUAL,        //  A1
        HID_KEY_TAB,          //  A2
        HID_KEY_ESCAPE,       //  A3
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
        HID_KEY_NONE,         //  D5
        HID_KEY_4,            //  E1
        HID_KEY_R,            //  E2
        HID_KEY_F,            //  E3
        HID_KEY_V,            //  E4
        HID_KEY_SPACE,        //  E5
        HID_KEY_5,            //  F1
        HID_KEY_T,            //  F2
        HID_KEY_G,            //  F3
        HID_KEY_B,            //  F4
        HID_KEY_NONE,         //  F5
        /* THUMB CLUSTER */
        HID_KEY_BACKSPACE,   //  T1
        HID_KEY_DELETE,      //  T2
        HID_KEY_HOME,        //  T3
        HID_KEY_END,         //  T4
        HID_KEY_ARROW_UP,    //  T5
        HID_KEY_KEYPAD_ENTER //  T6
    },
    [1] = {
        //     /* LAYER
        //      * ┌─────┬─────┬─────┬─────┬─────┬─────┐
        //      * │ F1  │ F2  │ F3  │ F4  │ F5  │ F6  │
        //      * │     │     │     │     │     │     │
        //      * ├─────┼─────┼─────┼─────┼─────┼─────┤
        //      * │ TAB │  Q  │  W  │ VOL │ VOL │ VOL │
        //      * │     │     │     │ MUTE│ DOWN│ UP  │
        //      * ├─────┼─────┼─────┼─────┼─────┼─────┤
        //      * │CAPS │  A  │  S  │  D  │  F  │  G  ├─────┐
        //      * │     │     │     │     │     │     | F5  │
        //      * ├─────┼─────┼─────┼─────┼─────┼─────┤     │
        //      * │SHIFT│  Z  │  X  │  C  │  V  |  B  ├─────┘
        //      * │     │     │     │     │     │     │
        //      * ├─────┼─────┼─────┼─────┼─────┼─────┘
        //      * │CTRL │ SYS │ ALT │LOWER│SPACE│
        //      * │     │     │     │     │     │       ┌─────┬─────┐
        //      * └─────┴─────┴─────┴─────┴─────┘       │ T6  │ T5  │
        //      *                                       │     │     │
        //      *                                 ┌─────┼─────┼─────┤
        //      *                                 │     │     │ T3  │
        //      *                                 │     │     │     │
        //      *                                 |BSPC | DEL ├─────┤
        //      *                                 │     │     │ T4  │
        //      *                                 │     │     │     │
        //      *                                 └─────┴─────┴─────┘
        //      */
        //      */
        HID_KEY_F1,           //  A1
        HID_KEY_TAB,          //  A2
        HID_KEY_ESCAPE,       //  A3
        HID_KEY_SHIFT_LEFT,   //  A4
        HID_KEY_CONTROL_LEFT, //  A5
        HID_KEY_F2,           //  B1
        HID_KEY_Q,            //  B2
        HID_KEY_A,            //  B3
        HID_KEY_Z,            //  B4
        HID_KEY_GUI_LEFT,     //  B5
        HID_KEY_F3,           //  C1
        HID_KEY_W,            //  C2
        HID_KEY_S,            //  C3
        HID_KEY_X,            //  C4
        HID_KEY_ALT_LEFT,     //  C5
        HID_KEY_F4,           //  D1
        HID_KEY_MUTE,         //  D2
        HID_KEY_D,            //  D3
        HID_KEY_C,            //  D4
        HID_KEY_NONE,         //  D5
        HID_KEY_F5,           //  E1
        HID_KEY_VOLUME_DOWN,  //  E2
        HID_KEY_F,            //  E3
        HID_KEY_V,            //  E4
        HID_KEY_SPACE,        //  E5
        HID_KEY_F6,           //  F1
        HID_KEY_VOLUME_UP,    //  F2
        HID_KEY_G,            //  F3
        HID_KEY_B,            //  F4
        HID_KEY_NONE,         //  F5
        /* THUMB CLUSTER */
        HID_KEY_BACKSPACE,   //  T1
        HID_KEY_DELETE,      //  T2
        HID_KEY_HOME,        //  T3
        HID_KEY_END,         //  T4
        HID_KEY_ARROW_UP,    //  T5
        HID_KEY_KEYPAD_ENTER //  T6
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

//     /* LAYER
//      *               ┌─────┬─────┬─────┬─────┬─────┬─────┐
//      *               │ F1  │ E1  │ D1  │ C1  │ B1  │ A1  │
//      *               │     │     │     │     │     │     │
//      *               ├─────┼─────┼─────┼─────┼─────┼─────┤
//      *               │ F2  │ E2  │ D2  │ C2  │ B2  │ A2  │
//      *               │     │     │     │     │     │     │
//      *               ├─────┼─────┼─────┼─────┼─────┼─────┤
//      *         ┌─────┤ F3  │ E3  │ D3  │ C3  │ B3  │ A3  |
//      *         │ F5  │     │     │     │     │     │     |
//      *         │     ├─────┼─────┼─────┼─────┼─────┼─────┤
//      *         └─────┤ F4  │ E4  │ D4  │ C4  │ B4  │ A4  |
//      *               │     │     │     │     │     │     |
//      *               └─────┼─────┼─────┼─────┼─────┼─────┤
//      *                     │ E5  │ D5  │ C5  │ B5  │ A5  │
//      * ┌─────┬─────┐       │     │     │     │     │     │
//      * │ T5  │ T6  │       └─────┴─────┴─────┴─────┴─────┘
//      * │     │     │       
//      ^ ├─────┼─────┼─────┐ 
//      * │ T3  │     │     │ 
//      * │     │     │     │ 
//      * ├─────┤ T2  ┤ T1  | 
//      * │ T4  │     │     │ 
//      * │     │     │     │ 
//      * └─────┴─────┴─────┘ 
//      *  
//      */ 
const uint16_t __in_flash("keymaps") keymaps_layers[][NUM_KEYS] = {
    [0] = {
//     /* LAYER
//      *               ┌─────┬─────┬─────┬─────┬─────┬─────┐
//      *               │ 6   │ 7   │ 8   │ 9   │ 0   │ -   │
//      *               │     │     │     │     │     │     │
//      *               ├─────┼─────┼─────┼─────┼─────┼─────┤
//      *               │ y   │ u   │ i   │ o   │ p   │ #   │
//      *               │     │     │     │     │     │     │
//      *               ├─────┼─────┼─────┼─────┼─────┼─────┤
//      *         ┌─────┤ h   │ j   │ k   │ l   │ ;   │ENTER|
//      *         │     │     │     │     │     │     │     |
//      *         │     ├─────┼─────┼─────┼─────┼─────┼─────┤
//      *         └─────┤ n   │ m   │ ,   │ .   │ /   │SHIFT|
//      *               │     │     │     │     │     │     |
//      *               └─────┼─────┼─────┼─────┼─────┼─────┤
//      *                     │SPACE│UPPER│ ALT │ GUI │CTRL │
//      * ┌─────┬─────┐       │     │     │     │     │     │
//      * │ T5  │ T6  │       └─────┴─────┴─────┴─────┴─────┘
//      * │     │     │       
//      ^ ├─────┼─────┼─────┐ 
//      * │ T3  │     │     │ 
//      * │     │     │     │ 
//      * ├─────┤ENTER|SPACE| 
//      * │ T4  │     │     │ 
//      * │     │     │     │ 
//      * └─────┴─────┴─────┘ 
//      *  
//      */ 
        HID_KEY_EQUAL,         //  A1
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
        HID_KEY_NONE,          //  D5
        HID_KEY_7,             //  E1
        HID_KEY_U,             //  E2
        HID_KEY_J,             //  E3
        HID_KEY_M,             //  E4
        HID_KEY_SPACE,         //  E5
        HID_KEY_6,             //  F1
        HID_KEY_Y,             //  F2
        HID_KEY_H,             //  F3
        HID_KEY_N,             //  F4
        HID_KEY_NONE,          //  F5
        /* THUMB CLUSTER */
        HID_KEY_SPACE,       //  T1
        HID_KEY_ENTER,       //  T2
        HID_KEY_PAGE_UP,     //  T3
        HID_KEY_PAGE_DOWN,   //  T4
        HID_KEY_ARROW_UP,    //  T5
        HID_KEY_KEYPAD_ENTER //  T6
    },
    [1] = {
        HID_KEY_F12,           //  A1
        HID_KEY_BACKSLASH,     //  A2
        HID_KEY_ENTER,         //  A3
        HID_KEY_SHIFT_RIGHT,   //  A4
        HID_KEY_CONTROL_RIGHT, //  A5
        HID_KEY_F11,           //  B1
        HID_KEY_EQUAL,         //  B2
        HID_KEY_SEMICOLON,     //  B3
        HID_KEY_SLASH,         //  B4
        HID_KEY_GUI_RIGHT,     //  B5
        HID_KEY_F10,           //  C1
        HID_KEY_MINUS,         //  C2
        HID_KEY_ARROW_RIGHT,   //  C3
        HID_KEY_PERIOD,        //  C4
        HID_KEY_ALT_LEFT,      //  C5
        HID_KEY_F9,            //  D1
        HID_KEY_I,             //  D2
        HID_KEY_ARROW_UP,      //  D3
        HID_KEY_COMMA,         //  D4
        HID_KEY_NONE,          //  D5
        HID_KEY_F8,            //  E1
        HID_KEY_U,             //  E2
        HID_KEY_ARROW_DOWN,    //  E3
        HID_KEY_M,             //  E4
        HID_KEY_SPACE,         //  E5
        HID_KEY_F7,            //  F1
        HID_KEY_Y,             //  F2
        HID_KEY_ARROW_LEFT,    //  F3
        HID_KEY_N,             //  F4
        HID_KEY_NONE,          //  F5
        /* THUMB CLUSTER */
        HID_KEY_SPACE,       //  T1
        HID_KEY_DELETE,      //  T2
        HID_KEY_ARROW_DOWN,  //  T3
        HID_KEY_DELETE,      //  T4
        HID_KEY_ARROW_UP,    //  T5
        HID_KEY_KEYPAD_ENTER //  T6
    }};

#endif

#endif
