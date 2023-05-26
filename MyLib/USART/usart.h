#ifndef __USART_H
#define __USART_H

#include "gd32f4xx.h"
#include "stdio.h"


#define BSP_USART_CLOCK         RCU_USART0
#define BSP_USART_TX_CLOCK      RCU_GPIOA
#define BSP_USART_RX_CLOCK      RCU_GPIOA

#define BSP_USART_TX_GPIO_PORT  GPIOA
#define BSP_USART_RX_GPIO_PORT  GPIOA
#define BSP_USART_TX_GPIO_PIN   GPIO_PIN_9
#define BSP_USART_RX_GPIO_PIN   GPIO_PIN_10

#define BSP_USART_AF            GPIO_AF_7

#define BSP_USART_NUM           USART0

// value:波特率
void USART_Init(uint32_t baudval);

//发送数据  value:发送的数据
void USART_Send_Byte(uint8_t Byte);

//发送字符串 value：发送的数据
void USART_Send_String(uint8_t *String);

void ps(char *String);

void pc(char c);

#endif
