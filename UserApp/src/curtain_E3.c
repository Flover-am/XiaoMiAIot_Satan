//
// Created by 18466 on 2023/6/11.
//
#include "inc/curtain_E3.h"
#include "inc/i2c.h"
#include "inc/display_E1.h"
#include "inc/systick.h"

extern i2c_addr_def curtain_addr;

void curtain_E3_i2c_init() {
    uint8_t i = 0;
    for (i = 0; i < 4; i++) {
        if (i2c_addr_poll(I2C0, CURTAIN_ADDRESS_E3 + i * 2)) {
            curtain_addr.periph = I2C0;
            curtain_addr.addr = CURTAIN_ADDRESS_E3 + i * 2;
            curtain_addr.flag = 1;
            break;
        }
    }
    //address not read
    if (curtain_addr.flag != 1) {
        //poll I2C1,Verti1
        for (i = 0; i < 4; i++) {
            if (i2c_addr_poll(I2C1, CURTAIN_ADDRESS_E3 + i * 2)) {
                curtain_addr.periph = I2C1;
                curtain_addr.addr = CURTAIN_ADDRESS_E3 + i * 2;
                curtain_addr.flag = 1;
                break;
            }
        }
    }

//    if (curtain_addr.flag) {
//        /* 上电 */
//        i2c_cmd_write(curtain_addr.periph, curtain_addr.addr, 0x01);
//    }

}

void open_curtain() {
    i2c_byte_write(curtain_addr.periph, curtain_addr.addr, 0, 0);
    delay_1ms(200);
}

void close_curtain() {
    i2c_byte_write(curtain_addr.periph, curtain_addr.addr, 0, 0);
    delay_1ms(200);
}

void curtain_test(int i) {
    enable_display();
    for (int j = 0; j < 0xFF; ++j) {
        for (int k = 0; k < 0xFF; ++k) {
            i2c_byte_write(curtain_addr.periph, curtain_addr.addr, j, k);
        }
        delay_1ms(100);
        display_int(j);
    }
}