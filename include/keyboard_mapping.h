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

//     /* LAYER
//      * ┌─────┬─────┬─────┬─────┬─────┬─────┐
//      * │ =   │  1  │  2  │  3  │  4  │  5  │
//      * │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┤
//      * │ TAB │  Q  │  W  │  E  │  R  │  T  │
//      * │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┤
//      * │ ESC │  A  │  S  │  D  │  F  │  G  ├─────┐
//      * │     │     │     │     │     │     |LOWER│
//      * ├─────┼─────┼─────┼─────┼─────┼─────┤     │
//      * │SHIFT│  Z  │  X  │  C  │  V  |  B  ├─────┘
//      * │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┘
//      * │CTRL │ SYS │ ALT │Down │Up   |
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
uint8_t const L0_A1[] = {HID_KEY_EQUAL, HID_KEY_NONE};        //  A1
uint8_t const L0_A2[] = {HID_KEY_TAB, HID_KEY_NONE};          //  A2
uint8_t const L0_A3[] = {HID_KEY_ESCAPE, HID_KEY_NONE};       //  A3
uint8_t const L0_A4[] = {HID_KEY_SHIFT_LEFT, HID_KEY_NONE};   //  A4
uint8_t const L0_A5[] = {HID_KEY_CONTROL_LEFT, HID_KEY_NONE}; //  A5
uint8_t const L0_B1[] = {HID_KEY_1, HID_KEY_NONE};            //  B1
uint8_t const L0_B2[] = {HID_KEY_Q, HID_KEY_NONE};            //  B2
uint8_t const L0_B3[] = {HID_KEY_A, HID_KEY_NONE};            //  B3
uint8_t const L0_B4[] = {HID_KEY_Z, HID_KEY_NONE};            //  B4
uint8_t const L0_B5[] = {HID_KEY_GUI_LEFT, HID_KEY_NONE};     //  B5
uint8_t const L0_C1[] = {HID_KEY_2, HID_KEY_NONE};            //  C1
uint8_t const L0_C2[] = {HID_KEY_W, HID_KEY_NONE};            //  C2
uint8_t const L0_C3[] = {HID_KEY_S, HID_KEY_NONE};            //  C3
uint8_t const L0_C4[] = {HID_KEY_X, HID_KEY_NONE};            //  C4
uint8_t const L0_C5[] = {HID_KEY_ALT_LEFT, HID_KEY_NONE};     //  C5
uint8_t const L0_D1[] = {HID_KEY_3, HID_KEY_NONE};            //  D1
uint8_t const L0_D2[] = {HID_KEY_E, HID_KEY_NONE};            //  D2
uint8_t const L0_D3[] = {HID_KEY_D, HID_KEY_NONE};            //  D3
uint8_t const L0_D4[] = {HID_KEY_C, HID_KEY_NONE};            //  D4
uint8_t const L0_D5[] = {HID_KEY_ARROW_DOWN, HID_KEY_NONE};   //  D5
uint8_t const L0_E1[] = {HID_KEY_4, HID_KEY_NONE};            //  E1
uint8_t const L0_E2[] = {HID_KEY_R, HID_KEY_NONE};            //  E2
uint8_t const L0_E3[] = {HID_KEY_F, HID_KEY_NONE};            //  E3
uint8_t const L0_E4[] = {HID_KEY_V, HID_KEY_NONE};            //  E4
uint8_t const L0_E5[] = {HID_KEY_ARROW_UP, HID_KEY_NONE};     //  E5
uint8_t const L0_F1[] = {HID_KEY_5, HID_KEY_NONE};            //  F1
uint8_t const L0_F2[] = {HID_KEY_T, HID_KEY_NONE};            //  F2
uint8_t const L0_F3[] = {HID_KEY_G, HID_KEY_NONE};            //  F3
uint8_t const L0_F4[] = {HID_KEY_B, HID_KEY_NONE};            //  F4
uint8_t const L0_F5[] = {HID_KEY_NONE, HID_KEY_NONE};         //  F5
/* THUMB CLUSTER */
uint8_t const L0_T1[] = {HID_KEY_BACKSPACE, HID_KEY_NONE};    //  T1
uint8_t const L0_T2[] = {HID_KEY_DELETE, HID_KEY_NONE};       //  T2
uint8_t const L0_T3[] = {HID_KEY_HOME, HID_KEY_NONE};         //  T3
uint8_t const L0_T4[] = {HID_KEY_END, HID_KEY_NONE};          //  T4
uint8_t const L0_T5[] = {HID_KEY_ARROW_UP, HID_KEY_NONE};     //  T5
uint8_t const L0_T6[] = {HID_KEY_KEYPAD_ENTER, HID_KEY_NONE}; //  T6

//     /* LAYER
//      * ┌─────┬─────┬─────┬─────┬─────┬─────┐
//      * │ F1  │ F2  │ F3  │ F4  │ F5  │ F6  │
//      * │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┤
//      * │ TAB │  Q  │PAUSE│ VOL │ VOL │ VOL │
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
uint8_t const L1_A1[] = {HID_KEY_F1, HID_KEY_NONE};           //  A1
uint8_t const L1_A2[] = {HID_KEY_TAB, HID_KEY_NONE};          //  A2
uint8_t const L1_A3[] = {HID_KEY_ESCAPE, HID_KEY_NONE};       //  A3
uint8_t const L1_A4[] = {HID_KEY_SHIFT_LEFT, HID_KEY_NONE};   //  A4
uint8_t const L1_A5[] = {HID_KEY_CONTROL_LEFT, HID_KEY_NONE}; //  A5
uint8_t const L1_B1[] = {HID_KEY_F2, HID_KEY_NONE};           //  B1
uint8_t const L1_B2[] = {HID_KEY_Q, HID_KEY_NONE};            //  B2
uint8_t const L1_B3[] = {HID_KEY_A, HID_KEY_NONE};            //  B3
uint8_t const L1_B4[] = {HID_KEY_Z, HID_KEY_NONE};            //  B4
uint8_t const L1_B5[] = {HID_KEY_GUI_LEFT, HID_KEY_NONE};     //  B5
uint8_t const L1_C1[] = {HID_KEY_F3, HID_KEY_NONE};           //  C1
uint8_t const L1_C2[] = {HID_KEY_PAUSE, HID_KEY_NONE};        //  C2
uint8_t const L1_C3[] = {HID_KEY_S, HID_KEY_NONE};            //  C3
uint8_t const L1_C4[] = {HID_KEY_X, HID_KEY_NONE};            //  C4
uint8_t const L1_C5[] = {HID_KEY_ALT_LEFT, HID_KEY_NONE};     //  C5
uint8_t const L1_D1[] = {HID_KEY_F4, HID_KEY_NONE};           //  D1
uint8_t const L1_D2[] = {HID_KEY_MUTE, HID_KEY_NONE};         //  D2
uint8_t const L1_D3[] = {HID_KEY_D, HID_KEY_NONE};            //  D3
uint8_t const L1_D4[] = {HID_KEY_C, HID_KEY_NONE};            //  D4
uint8_t const L1_D5[] = {HID_KEY_ARROW_DOWN, HID_KEY_NONE};   //  D5
uint8_t const L1_E1[] = {HID_KEY_F5, HID_KEY_NONE};           //  E1
uint8_t const L1_E2[] = {HID_KEY_VOLUME_DOWN, HID_KEY_NONE};  //  E2
uint8_t const L1_E3[] = {HID_KEY_F, HID_KEY_NONE};            //  E3
uint8_t const L1_E4[] = {HID_KEY_V, HID_KEY_NONE};            //  E4
uint8_t const L1_E5[] = {HID_KEY_ARROW_UP, HID_KEY_NONE};     //  D5
uint8_t const L1_F1[] = {HID_KEY_F6, HID_KEY_NONE};           //  F1
uint8_t const L1_F2[] = {HID_KEY_VOLUME_UP, HID_KEY_NONE};    //  F2
uint8_t const L1_F3[] = {HID_KEY_G, HID_KEY_NONE};            //  F3
uint8_t const L1_F4[] = {HID_KEY_B, HID_KEY_NONE};            //  F4
uint8_t const L1_F5[] = {HID_KEY_NONE, HID_KEY_NONE};         //  F5
/* THUMB CLUSTER */
uint8_t const L1_T1[] = {HID_KEY_BACKSPACE, HID_KEY_NONE};    //  T1
uint8_t const L1_T2[] = {HID_KEY_DELETE, HID_KEY_NONE};       //  T2
uint8_t const L1_T3[] = {HID_KEY_HOME, HID_KEY_NONE};         //  T3
uint8_t const L1_T4[] = {HID_KEY_END, HID_KEY_NONE};          //  T4
uint8_t const L1_T5[] = {HID_KEY_ARROW_UP, HID_KEY_NONE};     //  T5
uint8_t const L1_T6[] = {HID_KEY_KEYPAD_ENTER, HID_KEY_NONE}; //  T6

//     /* LAYER
//      * ┌─────┬─────┬─────┬─────┬─────┬─────┐
//      * │ F1  │ F2  │ F3  │ F4  │ F5  │ F6  │
//      * │     │     │     │     │     │     │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┤
//      * │ TAB │git  │git  │git  │git  │git  │
//      * │     │push │pull │commi│add  │stat |
//      * ├─────┼─────┼─────┼─────┼─────┼─────┤
//      * │ ESC │git  │  S  │git  │  F  │ls   ├─────┐
//      * │     │log  │     │diff │     │-lrt | F5  │
//      * ├─────┼─────┼─────┼─────┼─────┼─────┤     │
//      * │SHIFT│vim  │make │clear│vim  |pytho├─────┘
//      * │     │     │-j4  │     │     │     │
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
uint8_t const L2_A1[] = {HID_KEY_F1, HID_KEY_NONE};                                                                                                               //  A1
uint8_t const L2_A2[] = {HID_KEY_TAB, HID_KEY_NONE};                                                                                                              //  A2
uint8_t const L2_A3[] = {HID_KEY_ESCAPE, HID_KEY_NONE};                                                                                                           //  A3
uint8_t const L2_A4[] = {HID_KEY_SHIFT_LEFT, HID_KEY_NONE};                                                                                                       //  A4
uint8_t const L2_A5[] = {HID_KEY_CONTROL_LEFT, HID_KEY_NONE};                                                                                                     //  A5
uint8_t const L2_B1[] = {HID_KEY_F2, HID_KEY_NONE};                                                                                                               //  B1
uint8_t const L2_B2[] = {HID_KEY_G, HID_KEY_I, HID_KEY_T, HID_KEY_SPACE, HID_KEY_P, HID_KEY_U, HID_KEY_S, HID_KEY_H, HID_KEY_NONE};                               //  B2
uint8_t const L2_B3[] = {HID_KEY_G, HID_KEY_I, HID_KEY_T, HID_KEY_SPACE, HID_KEY_L, HID_KEY_O, HID_KEY_G, HID_KEY_NONE};                                          //  B3
uint8_t const L2_B4[] = {HID_KEY_V, HID_KEY_I, HID_KEY_M, HID_KEY_NONE};                                                                                          //  B4
uint8_t const L2_B5[] = {HID_KEY_GUI_LEFT, HID_KEY_NONE};                                                                                                         //  B5
uint8_t const L2_C1[] = {HID_KEY_F3, HID_KEY_NONE};                                                                                                               //  C1
uint8_t const L2_C2[] = {HID_KEY_G, HID_KEY_I, HID_KEY_T, HID_KEY_SPACE, HID_KEY_P, HID_KEY_U, HID_KEY_L, HID_KEY_L, HID_KEY_NONE};                               //  B2
uint8_t const L2_C3[] = {HID_KEY_S, HID_KEY_NONE};                                                                                                                //  C3
uint8_t const L2_C4[] = {HID_KEY_M, HID_KEY_A, HID_KEY_K, HID_KEY_E, HID_KEY_SPACE, HID_KEY_MINUS, HID_KEY_J, HID_KEY_4, HID_KEY_NONE};                           //  C4
uint8_t const L2_C5[] = {HID_KEY_ALT_LEFT, HID_KEY_NONE};                                                                                                         //  C5
uint8_t const L2_D1[] = {HID_KEY_F4, HID_KEY_NONE};                                                                                                               //  D1
uint8_t const L2_D2[] = {HID_KEY_G, HID_KEY_I, HID_KEY_T, HID_KEY_SPACE, HID_KEY_C, HID_KEY_O, HID_KEY_M, HID_KEY_M, HID_KEY_I, HID_KEY_T, HID_KEY_NONE};         //  D2
uint8_t const L2_D3[] = {HID_KEY_G, HID_KEY_I, HID_KEY_T, HID_KEY_SPACE, HID_KEY_D, HID_KEY_I, HID_KEY_F, HID_KEY_F, HID_KEY_NONE};                               //  D3
uint8_t const L2_D4[] = {HID_KEY_C, HID_KEY_L, HID_KEY_E, HID_KEY_A, HID_KEY_R, HID_KEY_NONE};                                                                    //  D4
uint8_t const L2_D5[] = {HID_KEY_ARROW_DOWN, HID_KEY_NONE};                                                                                                       //  D5
uint8_t const L2_E1[] = {HID_KEY_F5, HID_KEY_NONE};                                                                                                               //  E1
uint8_t const L2_E2[] = {HID_KEY_G, HID_KEY_I, HID_KEY_T, HID_KEY_SPACE, HID_KEY_A, HID_KEY_D, HID_KEY_D, HID_KEY_SPACE, HID_KEY_MINUS, HID_KEY_U, HID_KEY_NONE}; //  E2
uint8_t const L2_E3[] = {HID_KEY_F, HID_KEY_NONE};                                                                                                                //  E3
uint8_t const L2_E4[] = {HID_KEY_V, HID_KEY_I, HID_KEY_M, HID_KEY_NONE};                                                                                          //  E4
uint8_t const L2_E5[] = {HID_KEY_ARROW_UP, HID_KEY_NONE};                                                                                                         //  D5
uint8_t const L2_F1[] = {HID_KEY_F6, HID_KEY_NONE};                                                                                                               //  F1
uint8_t const L2_F2[] = {HID_KEY_G, HID_KEY_I, HID_KEY_T, HID_KEY_SPACE, HID_KEY_S, HID_KEY_T, HID_KEY_A, HID_KEY_T, HID_KEY_U, HID_KEY_S, HID_KEY_NONE};         //  F2
uint8_t const L2_F3[] = {HID_KEY_L, HID_KEY_S, HID_KEY_SPACE, HID_KEY_MINUS, HID_KEY_L, HID_KEY_R, HID_KEY_T, HID_KEY_NONE};                                      //  F3
uint8_t const L2_F4[] = {HID_KEY_P, HID_KEY_Y, HID_KEY_T, HID_KEY_H, HID_KEY_O, HID_KEY_N, HID_KEY_NONE};                                                         //  F4
uint8_t const L2_F5[] = {HID_KEY_NONE, HID_KEY_NONE};                                                                                                             //  F5
/* THUMB CLUSTER */
uint8_t const L2_T1[] = {HID_KEY_BACKSPACE, HID_KEY_NONE};    //  T1
uint8_t const L2_T2[] = {HID_KEY_DELETE, HID_KEY_NONE};       //  T2
uint8_t const L2_T3[] = {HID_KEY_HOME, HID_KEY_NONE};         //  T3
uint8_t const L2_T4[] = {HID_KEY_END, HID_KEY_NONE};          //  T4
uint8_t const L2_T5[] = {HID_KEY_ARROW_UP, HID_KEY_NONE};     //  T5
uint8_t const L2_T6[] = {HID_KEY_KEYPAD_ENTER, HID_KEY_NONE}; //  T6

#else
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

//     /* LAYER
//      *               ┌─────┬─────┬─────┬─────┬─────┬─────┐
//      *               │ 6   │ 7   │ 8   │ 9   │ 0   │ -   │
//      *               │     │     │     │     │     │     │
//      *               ├─────┼─────┼─────┼─────┼─────┼─────┤
//      *               │ y   │ u   │ i   │ o   │ p   │ #   │
//      *               │     │     │     │     │     │     │
//      *               ├─────┼─────┼─────┼─────┼─────┼─────┤
//      *         ┌─────┤ h   │ j   │ k   │ l   │ ;   │ENTER|
//      *         │UPPER│     │     │     │     │     │     |
//      *         │     ├─────┼─────┼─────┼─────┼─────┼─────┤
//      *         └─────┤ n   │ m   │ ,   │ .   │ /   │SHIFT|
//      *               │     │     │     │     │     │     |
//      *               └─────┼─────┼─────┼─────┼─────┼─────┤
//      *                     │Left │Right│ ALT │ GUI │CTRL │
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
uint8_t const L0_A1[] = {HID_KEY_MINUS, HID_KEY_NONE};         //  A1
uint8_t const L0_A2[] = {HID_KEY_BACKSLASH, HID_KEY_NONE};     //  A2
uint8_t const L0_A3[] = {HID_KEY_APOSTROPHE, HID_KEY_NONE};    //  A3
uint8_t const L0_A4[] = {HID_KEY_SHIFT_RIGHT, HID_KEY_NONE};   //  A4
uint8_t const L0_A5[] = {HID_KEY_CONTROL_RIGHT, HID_KEY_NONE}; //  A5
uint8_t const L0_B1[] = {HID_KEY_0, HID_KEY_NONE};             //  B1
uint8_t const L0_B2[] = {HID_KEY_P, HID_KEY_NONE};             //  B2
uint8_t const L0_B3[] = {HID_KEY_SEMICOLON, HID_KEY_NONE};     //  B3
uint8_t const L0_B4[] = {HID_KEY_SLASH, HID_KEY_NONE};         //  B4
uint8_t const L0_B5[] = {HID_KEY_GUI_RIGHT, HID_KEY_NONE};     //  B5
uint8_t const L0_C1[] = {HID_KEY_9, HID_KEY_NONE};             //  C1
uint8_t const L0_C2[] = {HID_KEY_O, HID_KEY_NONE};             //  C2
uint8_t const L0_C3[] = {HID_KEY_L, HID_KEY_NONE};             //  C3
uint8_t const L0_C4[] = {HID_KEY_PERIOD, HID_KEY_NONE};        //  C4
uint8_t const L0_C5[] = {HID_KEY_ALT_LEFT, HID_KEY_NONE};      //  C5
uint8_t const L0_D1[] = {HID_KEY_8, HID_KEY_NONE};             //  D1
uint8_t const L0_D2[] = {HID_KEY_I, HID_KEY_NONE};             //  D2
uint8_t const L0_D3[] = {HID_KEY_K, HID_KEY_NONE};             //  D3
uint8_t const L0_D4[] = {HID_KEY_COMMA, HID_KEY_NONE};         //  D4
uint8_t const L0_D5[] = {HID_KEY_ARROW_RIGHT, HID_KEY_NONE};   //  D5
uint8_t const L0_E1[] = {HID_KEY_7, HID_KEY_NONE};             //  E1
uint8_t const L0_E2[] = {HID_KEY_U, HID_KEY_NONE};             //  E2
uint8_t const L0_E3[] = {HID_KEY_J, HID_KEY_NONE};             //  E3
uint8_t const L0_E4[] = {HID_KEY_M, HID_KEY_NONE};             //  E4
uint8_t const L0_E5[] = {HID_KEY_ARROW_LEFT, HID_KEY_NONE};    //  E5
uint8_t const L0_F1[] = {HID_KEY_6, HID_KEY_NONE};             //  F1
uint8_t const L0_F2[] = {HID_KEY_Y, HID_KEY_NONE};             //  F2
uint8_t const L0_F3[] = {HID_KEY_H, HID_KEY_NONE};             //  F3
uint8_t const L0_F4[] = {HID_KEY_N, HID_KEY_NONE};             //  F4
uint8_t const L0_F5[] = {HID_KEY_NONE, HID_KEY_NONE};          //  F5
/* THUMB CLUSTER */
uint8_t const L0_T1[] = {HID_KEY_SPACE, HID_KEY_NONE};        //  T1
uint8_t const L0_T2[] = {HID_KEY_ENTER, HID_KEY_NONE};        //  T2
uint8_t const L0_T3[] = {HID_KEY_PAGE_UP, HID_KEY_NONE};      //  T3
uint8_t const L0_T4[] = {HID_KEY_PAGE_DOWN, HID_KEY_NONE};    //  T4
uint8_t const L0_T5[] = {HID_KEY_ARROW_UP, HID_KEY_NONE};     //  T5
uint8_t const L0_T6[] = {HID_KEY_KEYPAD_ENTER, HID_KEY_NONE}; //  T6

//     /* LAYER
//      *               ┌─────┬─────┬─────┬─────┬─────┬─────┐
//      *               │ F7  │ F8  │ F9  │ F10 │ F11 │ F12 │
//      *               │     │     │     │     │     │     │
//      *               ├─────┼─────┼─────┼─────┼─────┼─────┤
//      *               │NUM_/│NUM_7│NUM_8│NUM_9│NUM_-│ \   │
//      *               │     │     │     │     │     │     │
//      *               ├─────┼─────┼─────┼─────┼─────┼─────┤
//      *         ┌─────┤ h   │NUM_4│NUM_5│NUM_6│NUM_+│ '   |
//      *         │UPPER│     │     │     │     │     │     |
//      *         │     ├─────┼─────┼─────┼─────┼─────┼─────┤
//      *         └─────┤ n   │NUM_1│NUM_2│NUM_3│NUM_=│SHIFT|
//      *               │     │     │     │     │     │     |
//      *               └─────┼─────┼─────┼─────┼─────┼─────┤
//      *                     │NUM_0│Right│ .   │NUM_=│CTRL │
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

// Layer 1
uint8_t const L1_A1[] = {HID_KEY_F12, HID_KEY_NONE};               //  A1
uint8_t const L1_A2[] = {HID_KEY_BACKSLASH, HID_KEY_NONE};         //  A2
uint8_t const L1_A3[] = {HID_KEY_APOSTROPHE, HID_KEY_NONE};        //  A3
uint8_t const L1_A4[] = {HID_KEY_SHIFT_RIGHT, HID_KEY_NONE};       //  A4
uint8_t const L1_A5[] = {HID_KEY_CONTROL_RIGHT, HID_KEY_NONE};     //  A5
uint8_t const L1_B1[] = {HID_KEY_F11, HID_KEY_NONE};               //  B1
uint8_t const L1_B2[] = {HID_KEY_KEYPAD_SUBTRACT, HID_KEY_NONE};   //  B2
uint8_t const L1_B3[] = {HID_KEY_KEYPAD_ADD, HID_KEY_NONE};        //  B3
uint8_t const L1_B4[] = {HID_KEY_KEYPAD_EQUAL, HID_KEY_NONE};      //  B4
uint8_t const L1_B5[] = {HID_KEY_KEYPAD_EQUAL_SIGN, HID_KEY_NONE}; //  B5
uint8_t const L1_C1[] = {HID_KEY_F10, HID_KEY_NONE};               //  C1
uint8_t const L1_C2[] = {HID_KEY_KEYPAD_9, HID_KEY_NONE};          //  C2
uint8_t const L1_C3[] = {HID_KEY_KEYPAD_6, HID_KEY_NONE};          //  C3
uint8_t const L1_C4[] = {HID_KEY_KEYPAD_3, HID_KEY_NONE};          //  C4
uint8_t const L1_C5[] = {HID_KEY_KEYPAD_DECIMAL, HID_KEY_NONE};    //  C5
uint8_t const L1_D1[] = {HID_KEY_F9, HID_KEY_NONE};                //  D1
uint8_t const L1_D2[] = {HID_KEY_KEYPAD_8, HID_KEY_NONE};          //  D2
uint8_t const L1_D3[] = {HID_KEY_KEYPAD_5, HID_KEY_NONE};          //  D3
uint8_t const L1_D4[] = {HID_KEY_KEYPAD_2, HID_KEY_NONE};          //  D4
uint8_t const L1_D5[] = {HID_KEY_ARROW_RIGHT, HID_KEY_NONE};       //  D5
uint8_t const L1_E1[] = {HID_KEY_F8, HID_KEY_NONE};                //  E1
uint8_t const L1_E2[] = {HID_KEY_KEYPAD_7, HID_KEY_NONE};          //  E2
uint8_t const L1_E3[] = {HID_KEY_KEYPAD_4, HID_KEY_NONE};          //  E3
uint8_t const L1_E4[] = {HID_KEY_KEYPAD_1, HID_KEY_NONE};          //  E4
uint8_t const L1_E5[] = {HID_KEY_KEYPAD_0, HID_KEY_NONE};          //  E5
uint8_t const L1_F1[] = {HID_KEY_F7, HID_KEY_NONE};                //  F1
uint8_t const L1_F2[] = {HID_KEY_KEYPAD_DIVIDE, HID_KEY_NONE};     //  F2
uint8_t const L1_F3[] = {HID_KEY_ARROW_LEFT, HID_KEY_NONE};        //  F3
uint8_t const L1_F4[] = {HID_KEY_N, HID_KEY_NONE};                 //  F4
uint8_t const L1_F5[] = {HID_KEY_NONE, HID_KEY_NONE};              //  F5
/* THUMB CLUSTER */
uint8_t const L1_T1[] = {HID_KEY_SPACE, HID_KEY_NONE};        //  T1
uint8_t const L1_T2[] = {HID_KEY_ENTER, HID_KEY_NONE};       //  T2
uint8_t const L1_T3[] = {HID_KEY_ARROW_DOWN, HID_KEY_NONE};   //  T3
uint8_t const L1_T4[] = {HID_KEY_DELETE, HID_KEY_NONE};       //  T4
uint8_t const L1_T5[] = {HID_KEY_ARROW_UP, HID_KEY_NONE};     //  T5
uint8_t const L1_T6[] = {HID_KEY_KEYPAD_ENTER, HID_KEY_NONE}; //  T6

// Layer 2
uint8_t const L2_A1[] = {HID_KEY_F12, HID_KEY_NONE};           //  A1
uint8_t const L2_A2[] = {HID_KEY_BACKSLASH, HID_KEY_NONE};     //  A2
uint8_t const L2_A3[] = {HID_KEY_APOSTROPHE, HID_KEY_NONE};    //  A3
uint8_t const L2_A4[] = {HID_KEY_SHIFT_RIGHT, HID_KEY_NONE};   //  A4
uint8_t const L2_A5[] = {HID_KEY_CONTROL_RIGHT, HID_KEY_NONE}; //  A5
uint8_t const L2_B1[] = {HID_KEY_F11, HID_KEY_NONE};           //  B1
uint8_t const L2_B2[] = {HID_KEY_EQUAL, HID_KEY_NONE};         //  B2
uint8_t const L2_B3[] = {HID_KEY_SEMICOLON, HID_KEY_NONE};     //  B3
uint8_t const L2_B4[] = {HID_KEY_SLASH, HID_KEY_NONE};         //  B4
uint8_t const L2_B5[] = {HID_KEY_GUI_RIGHT, HID_KEY_NONE};     //  B5
uint8_t const L2_C1[] = {HID_KEY_F10, HID_KEY_NONE};           //  C1
uint8_t const L2_C2[] = {HID_KEY_MINUS, HID_KEY_NONE};         //  C2
uint8_t const L2_C3[] = {HID_KEY_ARROW_RIGHT, HID_KEY_NONE};   //  C3
uint8_t const L2_C4[] = {HID_KEY_PERIOD, HID_KEY_NONE};        //  C4
uint8_t const L2_C5[] = {HID_KEY_ALT_LEFT, HID_KEY_NONE};      //  C5
uint8_t const L2_D1[] = {HID_KEY_F9, HID_KEY_NONE};            //  D1
uint8_t const L2_D2[] = {HID_KEY_I, HID_KEY_NONE};             //  D2
uint8_t const L2_D3[] = {HID_KEY_ARROW_UP, HID_KEY_NONE};      //  D3
uint8_t const L2_D4[] = {HID_KEY_COMMA, HID_KEY_NONE};         //  D4
uint8_t const L2_D5[] = {HID_KEY_ARROW_RIGHT, HID_KEY_NONE};   //  D5
uint8_t const L2_E1[] = {HID_KEY_F8, HID_KEY_NONE};            //  E1
uint8_t const L2_E2[] = {HID_KEY_U, HID_KEY_NONE};             //  E2
uint8_t const L2_E3[] = {HID_KEY_ARROW_DOWN, HID_KEY_NONE};    //  E3
uint8_t const L2_E4[] = {HID_KEY_M, HID_KEY_A, HID_KEY_K, HID_KEY_E, HID_KEY_SPACE, HID_KEY_MINUS, HID_KEY_J, HID_KEY_4, HID_KEY_NONE};             //  E4
uint8_t const L2_E5[] = {HID_KEY_ARROW_LEFT, HID_KEY_NONE};    //  E5
uint8_t const L2_F1[] = {HID_KEY_F7, HID_KEY_NONE};            //  F1
uint8_t const L2_F2[] = {HID_KEY_Y, HID_KEY_NONE};             //  F2
uint8_t const L2_F3[] = {HID_KEY_ARROW_LEFT, HID_KEY_NONE};    //  F3
uint8_t const L2_F4[] = {HID_KEY_N, HID_KEY_NONE};             //  F4
uint8_t const L2_F5[] = {HID_KEY_NONE, HID_KEY_NONE};          //  F5
/* THUMB CLUSTER */
uint8_t const L2_T1[] = {HID_KEY_SPACE, HID_KEY_NONE};        //  T1
uint8_t const L2_T2[] = {HID_KEY_ENTER, HID_KEY_NONE};       //  T2
uint8_t const L2_T3[] = {HID_KEY_ARROW_DOWN, HID_KEY_NONE};   //  T3
uint8_t const L2_T4[] = {HID_KEY_DELETE, HID_KEY_NONE};       //  T4
uint8_t const L2_T5[] = {HID_KEY_ARROW_UP, HID_KEY_NONE};     //  T5
uint8_t const L2_T6[] = {HID_KEY_KEYPAD_ENTER, HID_KEY_NONE}; //  T6

#endif

const uint8_t *
    __in_flash("keymaps") keymaps_layers[][NUM_KEYS] = {
        [0] = {
            L0_A1, L0_A2, L0_A3, L0_A4, L0_A5,
            L0_B1, L0_B2, L0_B3, L0_B4, L0_B5,
            L0_C1, L0_C2, L0_C3, L0_C4, L0_C5,
            L0_D1, L0_D2, L0_D3, L0_D4, L0_D5,
            L0_E1, L0_E2, L0_E3, L0_E4, L0_E5,
            L0_F1, L0_F2, L0_F3, L0_F4, L0_F5,
            L0_T1, L0_T2, L0_T3,
            L0_T4, L0_T5, L0_T6},
        [1] = {L1_A1, L1_A2, L1_A3, L1_A4, L1_A5, L1_B1, L1_B2, L1_B3, L1_B4, L1_B5, L1_C1, L1_C2, L1_C3, L1_C4, L1_C5, L1_D1, L1_D2, L1_D3, L1_D4, L1_D5, L1_E1, L1_E2, L1_E3, L1_E4, L1_E5, L1_F1, L1_F2, L1_F3, L1_F4, L1_F5, L1_T1, L1_T2, L1_T3, L1_T4, L1_T5, L1_T6},
        [2] = {L2_A1, L2_A2, L2_A3, L2_A4, L2_A5, L2_B1, L2_B2, L2_B3, L2_B4, L2_B5, L2_C1, L2_C2, L2_C3, L2_C4, L2_C5, L2_D1, L2_D2, L2_D3, L2_D4, L2_D5, L2_E1, L2_E2, L2_E3, L2_E4, L2_E5, L2_F1, L2_F2, L2_F3, L2_F4, L2_F5, L2_T1, L2_T2, L2_T3, L2_T4, L2_T5, L2_T6},
};

#endif
