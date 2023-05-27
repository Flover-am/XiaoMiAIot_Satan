//
// Created by 18466 on 2023/5/27.
//
#include "inc/distanceS6.h"
#include "stdint.h"
#include "inc/main.h"
#include "inc/i2c.h"

extern i2c_addr_def dis_sensor_addr;

void distanceS6_addr_init(void) {
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
