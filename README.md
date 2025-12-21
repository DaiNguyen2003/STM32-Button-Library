# STM32 Button Library v3.1

Thư viện xử lý nút nhấn cho STM32 với 3 chế độ: **Single Click**, **Double Click**, và **Hold**.

## 📁 File cần copy

| File | Đường dẫn đích |
|------|---------------|
| `button_lib.h` | `Core/Inc/` |
| `button_lib.c` | `Core/Src/` |

## 🚀 Cách sử dụng

### 1. Include header
```c
#include "button_lib.h"
```

### 2. Khai báo và khởi tạo
```c
ButtonHandle_t hButton;

// Trong main(), trước vòng while:
Button_Init(&hButton, GPIOA, GPIO_PIN_1, 1);  // 1 = active LOW (pull-up)
```

### 3. Xử lý trong vòng lặp
```c
while (1) {
    ButtonEvent_t event = Button_GetEvent(&hButton);
    
    switch (event) {
        case BTN_SINGLE_CLICK:
            // Nhấn 1 lần
            break;
        case BTN_DOUBLE_CLICK:
            // Nhấn 2 lần
            break;
        case BTN_HOLD:
            // Giữ >= 500ms
            break;
        default:
            break;
    }
}
```

## 🎮 Quản lý nhiều nút nhấn

```c
#define NUM_BUTTONS 4

ButtonHandle_t hButtons[NUM_BUTTONS];
GPIO_TypeDef* BTN_PORTS[] = {GPIOA, GPIOA, GPIOB, GPIOB};
uint16_t BTN_PINS[] = {GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_0, GPIO_PIN_1};

// Khởi tạo
for (int i = 0; i < NUM_BUTTONS; i++) {
    Button_Init(&hButtons[i], BTN_PORTS[i], BTN_PINS[i], 1);
}

// Xử lý
for (int i = 0; i < NUM_BUTTONS; i++) {
    ButtonEvent_t event = Button_GetEvent(&hButtons[i]);
    // Xử lý event cho nút i...
}
```

## ⚙️ Cấu hình (trong button_lib.h)

| Define | Mặc định | Mô tả |
|--------|----------|-------|
| `BTN_DEBOUNCE_TIME` | 50ms | Thời gian chống rung |
| `BTN_DOUBLE_CLICK_TIME` | 300ms | Timeout giữa 2 click |
| `BTN_HOLD_TIME` | 500ms | Ngưỡng nhấn giữ |

## 📊 API Reference

| Hàm | Mô tả |
|-----|-------|
| `Button_Init()` | Khởi tạo nút nhấn |
| `Button_GetEvent()` | Lấy event (gọi trong while) |
| `Button_GetHoldTime()` | Lấy thời gian đang giữ (ms) |
| `Button_IsPressed()` | Kiểm tra nút đang nhấn |

## 📝 License

MIT License - Free to use
