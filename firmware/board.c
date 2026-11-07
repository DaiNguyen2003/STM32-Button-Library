#include "main.h"

/* The linker omits the toolchain CRT; startup still runs libc init arrays. */
void _init(void) {}

void HAL_MspInit(void)
{
    __HAL_RCC_AFIO_CLK_ENABLE();
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}
