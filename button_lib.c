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
}

ButtonEvent_t Button_GetEvent(ButtonHandle_t* hbtn) {
    hbtn->last_raw = hbtn->current_raw;
    hbtn->current_raw = ReadPin(hbtn);
    
    uint32_t now = HAL_GetTick();
    ButtonEvent_t event = BTN_NONE;
    
    // Nhấn xuống
    if (hbtn->current_raw && !hbtn->last_raw) {
        HAL_Delay(BTN_DEBOUNCE_TIME);
        if (ReadPin(hbtn)) {
            hbtn->press_time = now;
            hbtn->waiting_for_release = 1;
            hbtn->hold_triggered = 0;
        }
    }
    
    // Đang giữ
    if (hbtn->current_raw && hbtn->waiting_for_release) {
        hbtn->hold_time_ms = now - hbtn->press_time;
        if (hbtn->hold_time_ms >= BTN_HOLD_TIME && !hbtn->hold_triggered) {
            hbtn->hold_triggered = 1;
            hbtn->click_count = 0;
            return BTN_HOLD;
        }
    }
    
    // Thả nút
    if (!hbtn->current_raw && hbtn->last_raw) {
        hbtn->hold_time_ms = 0;
        if (hbtn->waiting_for_release) {
            hbtn->waiting_for_release = 0;
            if (hbtn->hold_triggered) {
                hbtn->hold_triggered = 0;
                return BTN_NONE;
            }
            hbtn->click_count++;
            hbtn->release_time = now;
        }
    }
    
    // Xử lý click sau timeout
    if (hbtn->click_count > 0 && !hbtn->current_raw) {
        if ((now - hbtn->release_time) > BTN_DOUBLE_CLICK_TIME) {
            event = (hbtn->click_count >= 2) ? BTN_DOUBLE_CLICK : BTN_SINGLE_CLICK;
            hbtn->click_count = 0;
        }
    }
    
    return event;
}

uint32_t Button_GetHoldTime(ButtonHandle_t* hbtn) {
    return hbtn->hold_time_ms;
}

uint8_t Button_IsPressed(ButtonHandle_t* hbtn) {
    return hbtn->current_raw;
}
