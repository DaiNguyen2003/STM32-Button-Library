/**
 * @file    button_lib.c
 * @brief   Thư viện xử lý nút nhấn cho STM32
 * @version 3.1
 */

#include "button_lib.h"

static uint8_t ReadPin(ButtonHandle_t* hbtn) {
    uint8_t state = HAL_GPIO_ReadPin(hbtn->GPIOx, hbtn->GPIO_Pin);
    return hbtn->active_low ? !state : state;
}

void Button_Init(ButtonHandle_t* hbtn, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t active_low) {
    hbtn->GPIOx = GPIOx;
    hbtn->GPIO_Pin = GPIO_Pin;
    hbtn->active_low = active_low;
    hbtn->last_raw = 0;
    hbtn->current_raw = 0;
    hbtn->click_count = 0;
    hbtn->press_time = 0;
    hbtn->release_time = 0;
    hbtn->waiting_for_release = 0;
    hbtn->hold_triggered = 0;
    hbtn->hold_time_ms = 0;
    hbtn->sample_changed_at = HAL_GetTick();
}

ButtonEvent_t Button_GetEvent(ButtonHandle_t* hbtn) {
    uint32_t now = HAL_GetTick();
    uint8_t sample = ReadPin(hbtn);
    if (sample != hbtn->last_raw) {
        hbtn->last_raw = sample;
        hbtn->sample_changed_at = now;
    }

    uint8_t second_press = sample && hbtn->click_count &&
        hbtn->sample_changed_at - hbtn->release_time <= BTN_DOUBLE_CLICK_TIME;
    if (!hbtn->current_raw && hbtn->click_count && !second_press &&
        now - hbtn->release_time >= BTN_DOUBLE_CLICK_TIME) {
        hbtn->click_count = 0;
        return BTN_SINGLE_CLICK;
    }

    if (hbtn->current_raw && hbtn->waiting_for_release) {
        uint32_t held_until = sample ? now : hbtn->sample_changed_at;
        hbtn->hold_time_ms = held_until - hbtn->press_time;
        if (hbtn->hold_time_ms >= BTN_HOLD_TIME && !hbtn->hold_triggered) {
            hbtn->hold_triggered = 1;
            hbtn->click_count = 0;
            return BTN_HOLD;
        }
    }

    if (sample != hbtn->current_raw && now - hbtn->sample_changed_at >= BTN_DEBOUNCE_TIME) {
        hbtn->current_raw = sample;
        if (sample) {
            hbtn->press_time = hbtn->sample_changed_at;
            hbtn->waiting_for_release = 1;
            hbtn->hold_triggered = 0;
        } else {
            hbtn->hold_time_ms = 0;
            hbtn->waiting_for_release = 0;
            if (hbtn->hold_triggered) {
                hbtn->hold_triggered = 0;
            } else if (hbtn->click_count) {
                hbtn->click_count = 0;
                return BTN_DOUBLE_CLICK;
            } else {
                hbtn->click_count = 1;
                hbtn->release_time = hbtn->sample_changed_at;
            }
        }
    }

    return BTN_NONE;
}

uint32_t Button_GetHoldTime(ButtonHandle_t* hbtn) {
    return hbtn->hold_time_ms;
}

uint8_t Button_IsPressed(ButtonHandle_t* hbtn) {
    return hbtn->current_raw;
}
