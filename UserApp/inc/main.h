/*!
    \file  main.h
    \brief the header file of main
		\version 2021-08, V0.1
*/
#ifndef MAIN_H
#define MAIN_H
#include "gd32f4xx.h"

//#define PRINT_DEBUG_INFO
#define LED_RED 1
#define LED_YELLOW 2
#define LED_BLUE 3

#define    ETH                0
#define    USB                0
#define    I2S                0

#define    E1_RGB                1
#define    E1_DISPLAY      1
#define    E2                1
#define    E3                1

#define    S1                1
#define    S5                1
#define    S6                1
#define    S7                1

#define    KEYKS0                0x40
#define HT16K33_ADDRESS_S1      0xE8
#define PCA9685_ADDRESS_E1      0xC0
#define HT1633_ADDRESS_E1       0xE0
#define WM8978_ADDR             0x10

#define  LIGHT_ADDRESS_S2          0x46
#define  TEMPER_ADDRESS_S2          0x88
#define  SIX_ADDRESS_S2          0xD0
// 23? 44! 68!

#define DIS_SENSOR_S6      0x58
#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define PCA9685_MODE1 0x0

#define    SYSTEM_ON                0x21

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD
//I2C address struct
typedef struct {
    uint8_t flag;
    uint32_t periph;
    uint8_t addr;
} i2c_addr_def;


#endif /* MAIN_H */
