#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int RED_BTN_PIN = 28;
const int GREEN_BTN_PIN = 26;
const int RED_LED_PIN = 4;
const int GREEN_LED_PIN = 6;

volatile int red_flag = 0;
volatile int green_flag = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    if (gpio == RED_BTN_PIN) {
      red_flag = 1;
    }
  } else if(events == 0x8) { // rise edge
    if (gpio == GREEN_BTN_PIN) {
      green_flag = 1;
    }
  }
}

int main() {
  stdio_init_all();

  gpio_init(RED_BTN_PIN);
  gpio_set_dir(RED_BTN_PIN, GPIO_IN);
  gpio_pull_up(RED_BTN_PIN);

  gpio_init(GREEN_BTN_PIN);
  gpio_set_dir(GREEN_BTN_PIN, GPIO_IN);
  gpio_pull_up(GREEN_BTN_PIN);

  gpio_init(RED_LED_PIN);
  gpio_set_dir(RED_LED_PIN, GPIO_OUT);

  gpio_init(GREEN_LED_PIN);
  gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);

  // callback led r (first)
  gpio_set_irq_enabled_with_callback(RED_BTN_PIN, GPIO_IRQ_EDGE_FALL, true,
    &btn_callback);

  // callback led g (nao usar _with_callback)
  gpio_set_irq_enabled(GREEN_BTN_PIN, GPIO_IRQ_EDGE_RISE, true);

  while (true) {
    if (green_flag) {
      if (gpio_get(GREEN_LED_PIN)) {
        gpio_put(GREEN_LED_PIN, 0);
      } else {
        gpio_put(GREEN_LED_PIN, 1);
      }
      green_flag = 0;
    } if (red_flag) {
      if (gpio_get(RED_LED_PIN)) {
        gpio_put(RED_LED_PIN, 0);
      } else {
        gpio_put(RED_LED_PIN, 1);
      }
      red_flag = 0;
    }
  }
}
