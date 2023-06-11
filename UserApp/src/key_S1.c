#include "inc/i2c.h"
#include "inc/key_S1.h"

//
// Created by 18466 on 2023/6/11.
//
extern i2c_addr_def key_addr;

int key_trans(int key_value[]) {//Translation from Array to Key number
    if (key_value[0] == 0x01) {
        return 1;
    } else if (key_value[0] == 0x02) {
        return 2;
    } else if (key_value[0] == 0x04) {
        return 3;
    } else if (key_value[0] == 0x08) {
        return 4;
    }
    return 0;
}

void key_S1_i2c_init() {
    int i = 0;
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
        i2c_cmd_write(key_addr.periph, key_addr.addr, SYSTEM_ON);
    } else {

    }
}

uint8_t get_key() {
    int key_value[6] = {0};
    i2c_read(key_addr.periph, key_addr.addr, KEYKS0, key_value, 6);//get key status
    int key = key_trans(key_value);
    return key;
}