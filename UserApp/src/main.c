#include "gd32f4xx.h"
#include "delay.h"
#include "led.h"
#include "inc/main.h"
#include "inc/i2c.h"
#include "inc/systick.h"
#include "inc/tools.h"
#include "inc/displayE1.h"
#include "inc/distanceS6.h"

i2c_addr_def key_addr;//Key address struct
i2c_addr_def LED_addr;//LED address struct
i2c_addr_def dis_sensor_addr;
i2c_addr_def display_addr;
uint8_t keyvalue[6];//Storage of key status

void display_test();

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

void i2c_addr_init(void) {
    uint8_t i;
    uint8_t c = 1;
// RGB 三色灯
#if    E1_RGB
    //E1 RGB LED I2C address init
    //poll I2C0,Horizontal
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
#ifdef PRINT_DEBUG_INFO
        printf("E1-RGB-LED is initialization success!\n");
        if(LED_addr.periph == I2C0)
            printf("%s","I2C0");
        else
            printf("%s","I2C1");
        printf(":");
        printf("%X",LED_addr.addr);
        printf("\n");
#endif
        //init E1 rgb led
        i2c_byte_write(LED_addr.periph, LED_addr.addr, PCA9685_MODE1, 0x0);
        setPWM_off(LED_addr.periph, LED_addr.addr);
    }
        //read failure
    else {
#ifdef PRINT_DEBUG_INFO
        printf("E1-RGB-LED is initialization failed	!\n");
#endif
    }
#endif
// 键盘子板
#if    S1
    //S1 KEY I2C address init
    for (i = 0; i < 4; i++) {
        if (i2c_addr_poll(I2C0, HT16K33_ADDRESS_S1 + i * 2)) {
            key_addr.periph = I2C0;
            key_addr.addr = HT16K33_ADDRESS_S1 + i * 2;
            key_addr.flag = 1;
            break;
        }
    }
    //address not read
    if (key_addr.flag != 1) {
        //poll I2C1,Verti1
        for (i = 0; i < 4; i++) {
            if (i2c_addr_poll(I2C1, HT16K33_ADDRESS_S1 + i * 2)) {
                key_addr.periph = I2C1;
                key_addr.addr = HT16K33_ADDRESS_S1 + i * 2;
                key_addr.flag = 1;
                break;
            }
        }
    }
    //read successful
    if (key_addr.flag) {
#ifdef PRINT_DEBUG_INFO
        printf("S1-KEY is initialization success!\n");
        if(key_addr.periph == I2C0)
            printf("%s","I2C0");
        else
            printf("%s","I2C1");
        printf(":");
        printf("%X",key_addr.addr);
        printf("\n");
#endif
        i2c_cmd_write(key_addr.periph, key_addr.addr, SYSTEM_ON);
    }
        //read failure
    else {
#ifdef PRINT_DEBUG_INFO
        printf("S1-KEY is initialization failed	!\n");
#endif
    }
#endif
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

void key_control(int key) {//Control over
    switch (key) {
        case 1:
            led_control(LED_RED);
            break;
        case 2:
            led_control(LED_BLUE);
            break;
        case 3:
            led_control(LED_YELLOW);
            break;
        default:
            return;
    }
}

int key_trans(void) {//Translation from Array to Key number
    if (keyvalue[0] & 0x01) {
        return 1;
    } else if (keyvalue[2] & 0x01) {
        return 2;
    } else if (keyvalue[4] & 0x01) {
        return 3;
    }
    return 0;
}



int main(void) {
    /* configure I2C GPIO */
    i2c0_gpio_config();
    /* configure I2C */
    i2c0_config();
    /* configure I2C GPIO */
    i2c1_gpio_config();
    /* configure I2C */
    i2c1_config();
    i2c_addr_init();//initializing i2c address of key and LED
    distanceS6_addr_init();

    systick_config();//system tick configuration
    int b = 0;
    uint8_t a[3] = {48, 49, 50};
    int cc[3] = {1, 2, 2};
    uint8_t i2c_which = dis_sensor_addr.periph;
    Display_i2c_init();
    enable_display();
    while (1) {
        int dis_evg = 0;
        int dis_all = 0;
        for (int i = 0; i < 300; ++i) {
            dis_all += get_dis();
            delay_1ms(1);
        }
        dis_evg = dis_all / 300;
        display_num(dis_evg / 10);
        enable_display();
        delay_1ms(10);
//        display_test();
    }
}
