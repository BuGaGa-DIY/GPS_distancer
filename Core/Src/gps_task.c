#include "main.h"
#include "gps.h"
#include "cmsis_os2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint8_t gps_msg_char = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  osStatus_t st;
    if(huart->Instance == USART1) // Check which UART triggered the interrupt
    {
      st = osMessageQueuePut(gps_charsHandle, &gps_msg_char, 0, 0U);
      if(st != osOK){

      }
      HAL_UART_Receive_IT(&huart1, &gps_msg_char, 1);
    }
}


/* USER CODE BEGIN Header_StartGPSReceiver */
/**
  * @brief  Function implementing the GPS_Receiver thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartGPSReceiver */
void StartGPSReceiver(void *argument)
{
  /* USER CODE BEGIN 5 */
  static uint8_t gps_msg_str[GPS_MSG_LEN] = {0};
  static uint8_t start = 0, i = 0, element = 0;
  static osStatus_t status;
  GPS_MSG gps_msg;
  /* Infinite loop */
  HAL_UART_Receive_IT(&huart1, &gps_msg_char, 1);
  for(;;)
  {
    status = osMessageQueueGet(gps_charsHandle, &element, NULL, osWaitForever);
    if(osOK == status)
    {
      HAL_GPIO_TogglePin(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);
      if(start)
      {
        if(i == 6){
          if( strnstr((const char *)gps_msg_str, "$GPGGA", 6) == NULL &&
            strnstr((const char *)gps_msg_str, "$GPGLL", 6) == NULL &&
            strnstr((const char *)gps_msg_str, "$GPRMC", 6) == NULL )
          {
            start = 0;
            memset(gps_msg_str, 0, 7);
          }
        }
        if(element == '\n')
        {
          start = 0;
          gps_msg_str[i++] = '\n';
          gps_msg_str[i++] = '\0';
          //debug_printf((const char *)gps_msg_str);
          if(strlen((const char *)gps_msg_str) > 45 &&
              gps_validate(gps_msg_str))
          {
            switch(gps_msg_str[5])
            {
            case 'A':
              gps_msg.type = GGA;
              break;
            case 'L':
              gps_msg.type = GLL;
              break;
            case 'C':
              gps_msg.type = RMC;
              break;
            default :
              debug_printf("Failed to set type: [%c]", gps_msg_str[5]);
              break;
            }
            strncpy(gps_msg.msg, (const char*)gps_msg_str, i);
            memset(gps_msg_str, 0, ++i);
            if(osMessageQueuePut(gps_msgHandle, &gps_msg, 0, 10) != osOK)
            {
              debug_printf("Queue is fool!");
            }
            memset(&gps_msg, 0, sizeof(GPS_MSG));
          }
        } else if(GPS_MSG_LEN == i)
        {
          start = 0;
          memset(gps_msg_str, 0, GPS_MSG_LEN);
        } else {
          gps_msg_str[i++] = element;
        }
      }
      else if(element == '$')
      {
        i = 0;
        gps_msg_str[i++] = element;
        start = 1;
      }
      HAL_GPIO_TogglePin(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);
    }
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Start_Parser */
/**
* @brief Function implementing the Parser_Task thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Parser */
void Start_Parser(void *argument)
{
  /* USER CODE BEGIN Start_Parser */
  GPS_MSG gps_msg;
  GPS_POINT gps_data;
  int status = 0;
  memset(&gps_msg, 0, sizeof(GPS_MSG));
  memset(&gps_data, 0, sizeof(GPS_POINT));
  //static char buff_msg[7] = {0};
  //uint16_t str_size;
  /* Infinite loop */
  for(;;)
  {
    if(osMessageQueueGet(gps_msgHandle, &gps_msg, NULL, osWaitForever) == osOK)
    {
      HAL_GPIO_TogglePin(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);
      //debug_printf(gps_msg.msg);
      switch(gps_msg.type)
      {
      case GGA:
        status = gps_parse_gga(gps_msg.msg, &gps_data);
        break;
      case GLL:
        status = gps_parse_gll(gps_msg.msg, &gps_data);
        break;
      case RMC:
        status = gps_parse_rmc(gps_msg.msg, &gps_data);
        break;
      default:
        debug_printf("Received GPS message without type!");
      }

      if(status)
      {
        gps_data.type = gps_msg.type;
        if(osMessageQueuePut(gps_pointsHandle, &gps_data, 0, 10) != osOK)
        {
          debug_printf("Failed to put GPOS point to queue");
        }
      }
      memset(&gps_data, 0, sizeof(GPS_POINT));
      memset(&gps_msg, 0, sizeof(GPS_MSG));
      HAL_GPIO_TogglePin(DEBUG_PIN_GPIO_Port, DEBUG_PIN_Pin);
    }
  }
  /* USER CODE END Start_Parser */
}