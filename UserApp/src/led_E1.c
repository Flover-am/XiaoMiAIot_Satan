//
// Created by 18466 on 2023/6/11.
//
#include "inc/led_E1.h"
#include "inc/i2c.h"

extern i2c_addr_def LED_addr;


void setPWM(uint32_t i2c_periph, uint8_t i2c_addr, uint8_t num, uint16_t on, uint16_t off) {//Set signal PWM
    i2c_byte_write(i2c_periph, i2c_addr, LED0_ON_L + 4 * num, on);
    i2c_byte_write(i2c_periph, i2c_addr, LED0_ON_H + 4 * num, on >> 8);
    i2c_byte_write(i2c_periph, i2c_addr, LED0_OFF_L + 4 * num, off);
    i2c_byte_write(i2c_periph, i2c_addr, LED0_OFF_H + 4 * num, off >> 8);
}

void setPWM_off(uint32_t i2c_periph, uint8_t i2c_addr) {//turn off LED
    i2c_byte_write(i2c_periph, i2c_addr, ALLLED_ON_L, 0);
    i2c_byte_write(i2c_periph, i2c_addr, ALLLED_ON_H, 0);
    i2c_byte_write(i2c_periph, i2c_addr, ALLLED_OFF_L, 0);
    i2c_byte_write(i2c_periph, i2c_addr, ALLLED_OFF_H, 0);
}

void led_E1_i2c_init() {
    int i = 0;
    for (i = 0; i < 4; i++) {
        if (i2c_addr_poll(I2C0, PCA9685_ADDRESS_E1 + i * 2)) {
            LED_addr.periph = I2C0;
            LED_addr.addr = PCA9685_ADDRESS_E1 + i * 2;
            LED_addr.flag = 1;
            break;
        }
    }
    //address not read
    if (LED_addr.flag != 1) {
        //poll I2C1,Verti1
        for (i = 0; i < 4; i++) {
            if (i2c_addr_poll(I2C1, PCA9685_ADDRESS_E1 + i * 2)) {
                LED_addr.periph = I2C1;
                LED_addr.addr = PCA9685_ADDRESS_E1 + i * 2;
                LED_addr.flag = 1;
                break;
            }
        }
    }
    //read successful
    if (LED_addr.flag) {
        //init E1 rgb led
        i2c_byte_write(LED_addr.periph, LED_addr.addr, PCA9685_MODE1, 0x0);
        setPWM_off(LED_addr.periph, LED_addr.addr);
    }
    else {
    }
}
void led_control(uint8_t color) {//Control over target LED
    switch (color) {
        case 0:
            setPWM_off(LED_addr.periph, LED_addr.addr);
            break;
        case 1:
            setPWM(LED_addr.periph, LED_addr.addr, 0, 0x0000, 0x0010);
            setPWM(LED_addr.periph, LED_addr.addr, 1, 0x0199, 0x0C00);
            setPWM(LED_addr.periph, LED_addr.addr, 2, 0x0000, 0x0010);
            break;
        case 2:
            setPWM(LED_addr.periph, LED_addr.addr, 0, 0x0199, 0x0800);
            setPWM(LED_addr.periph, LED_addr.addr, 1, 0x0199, 0x0800);
            setPWM(LED_addr.periph, LED_addr.addr, 2, 0x0000, 0x0010);
            break;
        case 3:
            setPWM(LED_addr.periph, LED_addr.addr, 0, 0x0000, 0x0010);
            setPWM(LED_addr.periph, LED_addr.addr, 1, 0x0000, 0x0010);
            setPWM(LED_addr.periph, LED_addr.addr, 2, 0x0199, 0x0C00);
            break;
        default:
            setPWM_off(LED_addr.periph, LED_addr.addr);
            break;
    }
}

