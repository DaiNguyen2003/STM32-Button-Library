#include "button_lib.h"

#include <assert.h>

static uint32_t tick;

uint8_t HAL_GPIO_ReadPin(GPIO_TypeDef *port, uint16_t pin)
{
    (void)pin;
    return port->level;
}

uint32_t HAL_GetTick(void)
{
    return tick;
}

static void Expect(ButtonHandle_t *button, uint32_t time, uint8_t level, ButtonEvent_t event)
{
    tick = time;
    button->GPIOx->level = level;
    assert(Button_GetEvent(button) == event);
}

int main(void)
{
    GPIO_TypeDef first = {.level = 1};
    GPIO_TypeDef second = {.level = 1};
    ButtonHandle_t a, b;
    Button_Init(&a, &first, 1, 1);
    Button_Init(&b, &second, 1, 1);

    /* An early bounce and overlapping presses must not block either button. */
    Expect(&a, 10, 0, BTN_NONE);
    Expect(&a, 20, 1, BTN_NONE);
    Expect(&a, 30, 0, BTN_NONE);
    Expect(&b, 40, 0, BTN_NONE);
    Expect(&a, 80, 0, BTN_NONE);
    Expect(&b, 90, 0, BTN_NONE);
    assert(Button_IsPressed(&a) && Button_IsPressed(&b));
    Expect(&a, 100, 1, BTN_NONE);
    Expect(&a, 150, 1, BTN_NONE);
    Expect(&a, 399, 1, BTN_NONE);
    Expect(&a, 400, 1, BTN_SINGLE_CLICK);
    Expect(&b, 540, 0, BTN_HOLD);
    assert(Button_GetHoldTime(&b) == 500);
    Expect(&b, 550, 0, BTN_NONE);
    Expect(&b, 560, 1, BTN_NONE);
    Expect(&b, 610, 1, BTN_NONE);
    assert(Button_GetHoldTime(&b) == 0);

    Expect(&a, 700, 0, BTN_NONE);
    Expect(&a, 750, 0, BTN_NONE);
    Expect(&a, 780, 1, BTN_NONE);
    Expect(&a, 830, 1, BTN_NONE);
    Expect(&a, 950, 0, BTN_NONE);
    Expect(&a, 1000, 0, BTN_NONE);
    Expect(&a, 1020, 1, BTN_NONE);
    Expect(&a, 1070, 1, BTN_DOUBLE_CLICK);
    Expect(&a, 1400, 1, BTN_NONE);

    /* Elapsed time must work when the 32-bit tick wraps. */
    Expect(&a, UINT32_MAX - 20U, 0, BTN_NONE);
    Expect(&a, 30U, 0, BTN_NONE);
    assert(Button_IsPressed(&a));
    Expect(&a, 480U, 0, BTN_HOLD);
    return 0;
}
