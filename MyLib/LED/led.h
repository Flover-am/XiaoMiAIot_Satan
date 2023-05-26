#ifndef __LED_H
#define __LED_H
#include "gd32f4xx.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"

#define LED_CLOCK       RCU_GPIOE
#define LED_Port        GPIOE
#define LED_Pin         GPIO_PIN_3

#define LED_Mode        GPIO_MODE_OUTPUT
#define LED_Pull        GPIO_PUPD_NONE
#define LED_Out_Type    GPIO_OTYPE_PP
#define LED_Pin_Speed   GPIO_OSPEED_50MHZ

#define LED_ON          gpio_bit_set(LED_Port, LED_Pin)
#define LED_OFF         gpio_bit_reset(LED_Port, LED_Pin)

void LED_Init(void);


#endif
