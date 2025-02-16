#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
volatile int btn_flag = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    btn_flag = 1;
  } else if (events == 0x8) { // rise edge
    btn_flag = 0;
  }
}

int main() {
  stdio_init_all();

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  int capture_flag = 0;
  int last_btn_flag = 0;

  while (true) {
    if (btn_flag != last_btn_flag) {
      capture_flag = 1;
      last_btn_flag = btn_flag;
    }

    if (capture_flag) {
      if (btn_flag) {
        printf("fall\n");
      } else {
        printf("rise\n");
      }
      capture_flag = 0;
    }
  }
}
