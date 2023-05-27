#include <stdbool.h>
#include "gd32f4xx.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "inc/main.h"
#include "inc/i2c.h"
#include "inc/systick.h"
#include "inc/tools.h"

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

void USensor_i2c_init(void) {
    uint8_t i = 0;
    for (i = 0; i < 4; i++) {
        if (i2c_addr_poll(I2C0, DIS_SENSOR_S6 + i * 2)) {
            dis_sensor_addr.periph = I2C0;
            dis_sensor_addr.addr = DIS_SENSOR_S6 + i * 2;
            dis_sensor_addr.flag = 1;
            break;
        }
    }
    //address not read
    if (dis_sensor_addr.flag != 1) {
        //poll I2C1,Verti1
        for (i = 0; i < 4; i++) {
            if (i2c_addr_poll(I2C1, DIS_SENSOR_S6 + i * 2)) {
                dis_sensor_addr.periph = I2C1;
                dis_sensor_addr.addr = DIS_SENSOR_S6 + i * 2;
                dis_sensor_addr.flag = 1;
                break;
            }
        }
    }

}

void Display_i2c_init(void) {
    int i = 0;
    for (i = 0; i < 4; i++) {
        if (i2c_addr_poll(I2C0, HT1633_ADDRESS_E1 + i * 2)) {
            display_addr.periph = I2C0;
            display_addr.addr = HT1633_ADDRESS_E1 + i * 2;
            display_addr.flag = 1;
            break;
        }
    }
    //address not read
    if (display_addr.flag != 1) {
        //poll I2C1,Verti1
        for (i = 0; i < 4; i++) {
            if (i2c_addr_poll(I2C1, HT1633_ADDRESS_E1 + i * 2)) {
                display_addr.periph = I2C1;
                display_addr.addr = HT1633_ADDRESS_E1 + i * 2;
                display_addr.flag = 1;
                break;
            }
        }
    }
    if (display_addr.flag) {
        i2c_cmd_write(display_addr.periph, display_addr.addr, 0x21);
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

int get_dis(void) {
    int dis = 0;
    i2c_byte_write(dis_sensor_addr.periph, dis_sensor_addr.addr, 0, 0xAA);
    uint8_t dis_array[2] = {0};
    i2c_read(dis_sensor_addr.periph, dis_sensor_addr.addr, 1, dis_array, 2);
    if (dis_array[0] == 0 && dis_array[1] == 1) {
        dis = -1;
    } else {
        dis = dis_array[0] * 256 + dis_array[1];
    }
    return dis;
}

void enable_display(void) {
    i2c_cmd_write(display_addr.periph, display_addr.addr, 0x81);
}

void disable_display(void) {
    i2c_cmd_write(display_addr.periph, display_addr.addr, 0x80);
}

void display_byte(int which_place, int num) {
    uint8_t mem_1 = 0;
    uint8_t mem_2 = 0;

    switch (which_place) {
        case 1:
            mem_1 = 0x02;
            break;
        case 2:
            mem_1 = 0x04;
            break;
        case 3:
            mem_1 = 0x06;
            break;
        case 4:
            mem_1 = 0x08;
            break;
    }
    mem_2 = mem_1 + 1;
    switch (num) {
        case 0://ABCDEF
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("11111000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000001"));
            break;
        case 1:// BC
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("00110000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000000"));
            break;
        case 2:// ABDEG
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("11011000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000010"));
            break;
        case 3:// ABCD G
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("01111000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000010"));
            break;
        case 4: // BC FG
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("00110000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000011"));
            break;
        case 5:// ACD FG
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("01101000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000011"));
            break;
        case 6: // ACDE FG
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("11101000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000011"));
            break;
        case 7: // ABC
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("00111000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000000"));
            break;
        case 8: // ABCDE FG

            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("11111000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000011"));
            break;
        case 9:// ABCD FG
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("01111000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000011"));
            break;

    }
}

void clear_display_byte(int which_place) {
    uint8_t mem_1 = 0;
    uint8_t mem_2 = 0;
    switch (which_place) {
        case 1:
            mem_1 = 0x02;
            break;
        case 2:
            mem_1 = 0x04;
            break;
        case 3:
            mem_1 = 0x06;
            break;
        case 4:
            mem_1 = 0x08;
            break;
    }
    mem_2 = mem_1 + 1;
    i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("00000000"));
    i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000000"));
}

void display_num(int num) {
    // num = 123
    if (num < 0 || num > 9999) {
        return;
    }
    uint8_t str[4];
    bool flag = true;
    int z_digit = -1;
    // str = {0,1,2,3}
    for (int i = 0; i < 4; ++i) {
        str[i] = get_digit(num, 3 - i);
        if (str[i] == 0 && flag) {
            z_digit++;
        } else {
            flag = false;
        }
    }


    for (int i = 0; i < 4; ++i) {
        if (i <= z_digit) {
            clear_display_byte(i + 1);

        } else {
            display_byte(i + 1, str[i]);
        }

    }
    if (display_addr.periph == 1) {
        printf("1");
    }
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
    USensor_i2c_init();

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

void display_test() {

    i2c_byte_write(display_addr.periph, display_addr.addr, 0x04, binary2int("00000000"));
    i2c_byte_write(display_addr.periph, display_addr.addr, 0x05, binary2int("00000000"));

    i2c_cmd_write(display_addr.periph, display_addr.addr, 0x81);

    delay_1ms(3000);

    delay_1ms(3000);

    //    i2c_cmd_write(display_addr.periph, display_addr.addr, 0x80);
}
