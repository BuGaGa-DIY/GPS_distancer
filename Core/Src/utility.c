#include "main.h"
#include "cmsis_os2.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

void debug_printf(const char* format, ...)
{
  static char buff[DEBUG_MSG_LEN] = {0};
  static int size = 0;
  if(osSemaphoreAcquire(printer_semHandle, 100) == osOK)
  {
    va_list args;
    va_start(args, format);
    size = vsnprintf((char*)buff, DEBUG_MSG_LEN, format, args);
    va_end(args);
    if(size < DEBUG_MSG_LEN - 2)
    {
      //size--;
      buff[size++] = '\r';
      buff[size++] = '\n';
      buff[size++] = '\0';
    }
    else
    {
      buff[size++] = '\0';
    }
    osMessageQueuePut(printer_qHandle, buff, 0, 100);
    osSemaphoreRelease(printer_semHandle);
  }
}

/* USER CODE BEGIN Header_Start_printer */
/**
* @brief Function implementing the Print_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_printer */
void Start_printer(void *argument)
{
  /* USER CODE BEGIN Start_printer */
  size_t size = 0;
  char data[DEBUG_MSG_LEN];
  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(printer_qHandle, data, NULL, osWaitForever) == osOK)
    {
      size = strlen((const char *)data);
      HAL_UART_Transmit(&huart2, (uint8_t *)data, size, 500);
    }
    else
      osDelay(1);
  }
  /* USER CODE END Start_printer */
}