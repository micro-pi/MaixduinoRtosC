#include "main.h"

uint32_t lcd_gram[LCD_X_MAX * LCD_Y_MAX / 2] __attribute__((aligned(128)));

int main() {
  BaseType_t xReturn;

  char msg1[] = "canaan kendryte";
  char msg2[] = "hello AI world";

  initMillis();

  printf("lcd test\n");
  lcd_init();
  memset(lcd_gram, 0xc0, LCD_X_MAX * LCD_Y_MAX * 2);
  lcd_clear(GREEN);
  lcd_draw_picture(100, 0, 120, 240, lcd_gram);
  lcd_draw_string(100, 50, msg1, BLUE);
  lcd_draw_string(100, 90, msg2, NAVY);

  xReturn = xTaskCreateAtProcessor(CORE_0, &vPowerBlinkTaskCore0, "vPowerBlinkTaskCore0", 256, NULL, 2, NULL);
  if (xReturn != pdPASS) {
    printf("Task %s run problem\r\n", "vPowerBlinkTaskCore0");
  }

  xReturn = xTaskCreateAtProcessor(CORE_1, &vITG3200TaskCore1, "vITG3200TaskCore1", 512, NULL, 2, NULL);
  if (xReturn != pdPASS) {
    printf("Task %s run problem\r\n", "vITG3200TaskCore1");
  }

  xReturn = xTaskCreateAtProcessor(CORE_1, &vI2cTaskCore1, "vI2cTaskCore1", 512, NULL, 2, NULL);
  if (xReturn != pdPASS) {
    printf("Task %s run problem\r\n", "vI2cTaskCore1");
  }

  for (;;) {
  }
}