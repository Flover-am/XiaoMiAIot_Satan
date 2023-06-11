//
// Created by 18466 on 2023/6/11.
//
#include <stdbool.h>
#include "inc/main.h"
#include "inc/i2c.h"
#include "inc/tools.h"
#include "inc/display_E1.h"

extern i2c_addr_def display_addr;

void display_E1_i2c_init(void) {
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

void enable_display(void) {
    i2c_cmd_write(display_addr.periph, display_addr.addr, 0x81);
}

void disable_display(void) {
    display_byte(1, 11);
    display_byte(2, 11);
    display_byte(3, 11);
    display_byte(4, 11);
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

        case -1:// BC
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("00110000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000100"));
            break;
        case -2:// ABDEG
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("11011000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000110"));
            break;
        case -3:// ABCD G
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("01111000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000110"));
            break;
        case -4: // BC FG
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("00110000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000111"));
            break;
        case -5:// ACD FG
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("01101000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000111"));
            break;
        case -6: // ACDE FG
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("11101000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000111"));
            break;
        case -7: // ABC
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("00111000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000100"));
            break;
        case -8: // ABCDE FG

            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("11111000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000111"));
            break;
        case -9:// ABCD FG
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("01111000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000111"));
            break;
        case 10:
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("11111000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000011"));
            break;
        case 11:
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_1, binary2int("00000000"));
            i2c_byte_write(display_addr.periph, display_addr.addr, mem_2, binary2int("00000000"));
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

void display_int(int num) {
    enable_display();
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

}

void displya_double(double num) {

    int integer = (int) num;//111.2345---0.234523.45-23
    int point_p = 0;
    if (num >= 10000 || num < 0) {
        return;
    } else if (num < 10000 && num >= 1000) {
        integer = integer;

    } else if (num < 1000 && num >= 100) {
        // 123
        integer = (int) (num * 10);
        display_byte(1, get_digit(integer, 3));
        display_byte(2, get_digit(integer, 2));
        display_byte(3, -1 * get_digit(integer, 1));
        display_byte(4, get_digit(integer, 0));
    } else if (num < 100 && num >= 10) {
        //12
        integer = (int) (num * 100);
        display_byte(1, get_digit(integer, 3));
        display_byte(2, -1 * get_digit(integer, 2));
        display_byte(3, get_digit(integer, 1));
        display_byte(4, get_digit(integer, 0));
    } else if (num < 10 && num >= 0) {
        integer = (int) (num * 1000);
        display_byte(1, -1 * get_digit(integer, 3));
        display_byte(2, get_digit(integer, 2));
        display_byte(3, get_digit(integer, 1));
        display_byte(4, get_digit(integer, 0));
    }


//    display_int(integer);
}

