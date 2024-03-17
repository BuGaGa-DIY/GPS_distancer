#include "main.h"
#include "gps.h"
#include "st7735.h"
#include "lcd_gfx.h"
#include <stdio.h>

/* USER CODE BEGIN Header_Start_lcd */
/**
* @brief Function implementing the LCD_Updater thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_lcd */
void Start_lcd(void *argument)
{
  /* USER CODE BEGIN Start_lcd */
  int la_h = 0, la_l = 0, lo_h = 0, lo_l = 0, no_data = 0;
  GPS_POINT gps_data;
  char text_buf[32] = {0};
  ST7735_Init(0);
  fillScreen(BLACK);
  ST7735_SetRotation(2);
  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(gps_pointsHandle, &gps_data, NULL, 10000) == osOK)
    {
      HAL_GPIO_TogglePin(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);
      la_h = (int)gps_data.latitude;
      la_l = (int)((gps_data.latitude - la_h) * 100000);

      lo_h = (int)gps_data.longitude;
      lo_l = (int)((gps_data.longitude - lo_h) * 100000);

      debug_printf("%d.%05d%c %d.%05d%c ", la_h, la_l, gps_data.latSide, lo_h, lo_l, gps_data.lonSide);
      snprintf(text_buf, 32, "%d.%05d %c", la_h, la_l, gps_data.latSide);
      if(osSemaphoreAcquire(lcd_semHandle, 100) == osOK)
      {
        ST7735_WriteString(10, 50, text_buf, Font_11x18, CYAN, BLACK);
        snprintf(text_buf, 32, "%d.%05d %c", lo_h, lo_l, gps_data.lonSide);
        ST7735_WriteString(10, 70, text_buf, Font_11x18, CYAN, BLACK);
        if(gps_data.type == GGA)
        {
          snprintf(text_buf, 32, "S:%d", gps_data.satilits);
          ST7735_WriteString(0, 0, text_buf, Font_11x18, gps_data.satilits > 5 ? GREEN : RED, BLACK);
        }
        snprintf(text_buf, 32, "UTC:%s", gps_data.lastMeasure);
        ST7735_WriteString(20, 149, text_buf, Font_7x10, YELLOW, BLACK);
        osSemaphoreRelease(lcd_semHandle);
      }
      no_data = 0;
      HAL_GPIO_TogglePin(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);
    } else if(!no_data)
    {
      no_data = 1;
      fillScreen(BLACK);
      snprintf(text_buf, 32, "NO SIGNAL!");
      ST7735_WriteString(5, 70, text_buf, Font_11x18, RED, BLACK);
    }
  }
  /* USER CODE END Start_lcd */
}

/* USER CODE BEGIN Header_Start_lcd_idel */
/**
* @brief Function implementing the LCD_Idel thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_lcd_idel */
void Start_lcd_idel(void *argument)
{
  /* USER CODE BEGIN Start_lcd_idel */
  /* Infinite loop */
  for(;;)
  {
    for(int i = 2; i < 11; ++i)
    {
      if(osSemaphoreAcquire(lcd_semHandle, 100) == osOK){
        drawCircle(118, 10, i, MAGENTA);
        osSemaphoreRelease(lcd_semHandle);
      }
      osDelay(70);
    }
    for(int i = 10; i > 0; --i)
    {
      if(osSemaphoreAcquire(lcd_semHandle, 100) == osOK){
        drawCircle(118, 10, i, BLACK);
        osSemaphoreRelease(lcd_semHandle);
      }
      osDelay(100);
    }
  }
  /* USER CODE END Start_lcd_idel */
}