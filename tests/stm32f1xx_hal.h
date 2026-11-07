#ifndef TEST_STM32F1XX_HAL_H
#define TEST_STM32F1XX_HAL_H

#include <stdint.h>

typedef struct {
    uint8_t level;
} GPIO_TypeDef;

uint8_t HAL_GPIO_ReadPin(GPIO_TypeDef *port, uint16_t pin);
uint32_t HAL_GetTick(void);

#endif
