/**
 * @file    button_lib.h
 * @brief   Thư viện xử lý nút nhấn cho STM32
 * @version 3.1
 * @date    2025-12-21
 */

#ifndef BUTTON_LIB_H
#define BUTTON_LIB_H

#include "stm32f1xx_hal.h"

/* ==================== CẤU HÌNH ==================== */
#define BTN_DEBOUNCE_TIME       50      // Chống rung (ms)
#define BTN_DOUBLE_CLICK_TIME   300     // Thời gian chờ click thứ 2 (ms)
#define BTN_HOLD_TIME           500     // Ngưỡng nhấn giữ (ms)

/* ==================== EVENT ENUM ==================== */
typedef enum {
    BTN_NONE = 0,
    BTN_SINGLE_CLICK,
    BTN_DOUBLE_CLICK,
    BTN_HOLD
} ButtonEvent_t;

/* ==================== STRUCT ==================== */
typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
    uint8_t active_low;
    uint8_t last_raw;
    uint8_t current_raw;
    uint8_t click_count;
    uint32_t press_time;
    uint32_t release_time;
    uint8_t waiting_for_release;
    uint8_t hold_triggered;
    uint32_t hold_time_ms;
    uint32_t sample_changed_at;
} ButtonHandle_t;

/* ==================== FUNCTIONS ==================== */
void Button_Init(ButtonHandle_t* hbtn, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint8_t active_low);
ButtonEvent_t Button_GetEvent(ButtonHandle_t* hbtn);
uint32_t Button_GetHoldTime(ButtonHandle_t* hbtn);
uint8_t Button_IsPressed(ButtonHandle_t* hbtn);

#endif
