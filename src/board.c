#include "board.h"
#include "keyboard_config.h"

#include "hardware/gpio.h"
#include <stdio.h>

#define GPIO_INPUT_SETUP(BUTTON)   \
    gpio_init(BUTTON);             \
    gpio_set_dir(BUTTON, GPIO_IN); \
    gpio_pull_up(BUTTON);

#define GPIO_INPUT_PULLDOWN(BUTTON) \
    gpio_init(BUTTON);              \
    gpio_set_dir(BUTTON, GPIO_IN);  \
    gpio_pull_down(BUTTON);

atomic_bool encoder_a = 0;
atomic_bool encoder_b = 0;
bool encoder_button = 0;

atomic_uint count_clockwise = 0;
atomic_uint count_anti_clockwise = 0;

bool encoder_has_action()
{
    return count_clockwise || count_anti_clockwise || encoder_button;
}

static void isr_handler(uint buf, uint32_t events)
{
    (void)events;
    switch (buf)
    {
    case ENCODER_A:
        if (events == GPIO_IRQ_EDGE_FALL)
        {
            if (encoder_b)
            {
                count_clockwise++;
                encoder_b = 0;
            }
            else
            {
                encoder_a = 1;
            }
        }
        else
        {
            encoder_a = 0;
        }
        break;
    case ENCODER_B:
        if (events == GPIO_IRQ_EDGE_FALL)
        {
            if (encoder_a)
            {
                count_anti_clockwise++;
                encoder_a = 0;
            }
            else
            {
                encoder_b = 1;
            }
        }
        else
        {
            encoder_b = 0;
        }
        break;
    case ENCODER_BUTTON:
        encoder_button = true;
        break;
    default:
        break;
    }
}

void setup_board(void)
{
    GPIO_INPUT_SETUP(A1)
    GPIO_INPUT_SETUP(A2)
    GPIO_INPUT_SETUP(A3)
    GPIO_INPUT_SETUP(A4)
    GPIO_INPUT_SETUP(A5)

    GPIO_INPUT_SETUP(B1)
    GPIO_INPUT_SETUP(B2)
    GPIO_INPUT_SETUP(B3)
    GPIO_INPUT_SETUP(B4)
    GPIO_INPUT_SETUP(B5)

    GPIO_INPUT_SETUP(C1)
    GPIO_INPUT_SETUP(C2)
    GPIO_INPUT_SETUP(C3)
    GPIO_INPUT_SETUP(C4)
    GPIO_INPUT_SETUP(C5)

    GPIO_INPUT_SETUP(D1)
    GPIO_INPUT_SETUP(D2)
    GPIO_INPUT_SETUP(D3)
    GPIO_INPUT_SETUP(D4)
    GPIO_INPUT_SETUP(D5)

    GPIO_INPUT_SETUP(E1)
    GPIO_INPUT_SETUP(E2)
    GPIO_INPUT_SETUP(E3)
    GPIO_INPUT_SETUP(E4)
    GPIO_INPUT_SETUP(E5)

    GPIO_INPUT_SETUP(F1)
    GPIO_INPUT_SETUP(F2)
    GPIO_INPUT_SETUP(F3)
    GPIO_INPUT_SETUP(F4)
    GPIO_INPUT_SETUP(F5)

    // THUMB CLUSTER
    GPIO_INPUT_SETUP(T1)
    GPIO_INPUT_SETUP(T2)
    GPIO_INPUT_SETUP(T3)
    GPIO_INPUT_SETUP(T4)
    GPIO_INPUT_SETUP(T5)
    GPIO_INPUT_SETUP(T6)
}

void setup_encoder()
{
    GPIO_INPUT_PULLDOWN(ENCODER_BUTTON)
    GPIO_INPUT_PULLDOWN(ENCODER_A)
    GPIO_INPUT_PULLDOWN(ENCODER_B)

    gpio_set_irq_enabled_with_callback(ENCODER_BUTTON, GPIO_IRQ_EDGE_FALL, true, &isr_handler);
    gpio_set_irq_enabled(ENCODER_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(ENCODER_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
}
