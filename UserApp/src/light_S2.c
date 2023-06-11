#include <stdint.h>
#include "inc/tools.h"
#include "inc/i2c.h"
#include "inc/main.h"
#include "inc/light_S2.h"
#include "../Mylib/delay/delay.h"

/*
 * 测量命令(0x10)
 * 上电命令(0x01)
 * */
extern i2c_addr_def light_sensor_addr;

void light_S2_i2c_init(void) {
    int a[20] = {0};
    int p = 0;
    for (int i = 2; i <= 0x7F * 2; ++i) {
        if (i2c_addr_poll(I2C0, i)) {
            a[p++] = i;
        }
    }
    a[p++] = 0;
    a[p++] = 0;
    for (int i = 2; i <= 0x7F * 2; ++i) {
        if (i2c_addr_poll(I2C1, i )) {
            a[p++] = i ;
        }
    }

    // I2C0
    for (int i = 0; i < 4; ++i) {
        if (i2c_addr_poll(I2C1, LIGHT_ADDRESS_S2 + i * 2)) {
            light_sensor_addr.periph = I2C1;
            light_sensor_addr.addr = LIGHT_ADDRESS_S2 + i * 2;
            light_sensor_addr.flag = 1;
            break;
        }
    }

//    if (light_sensor_addr.flag != 1) {
//        //I2C1
//        for (int i = 0; i < 4; ++i) {
//            if (i2c_addr_poll(I2C1, BH1750FVI_ADDRESS_S2 + i * 2)) {
//                light_sensor_addr.periph = I2C1;
//                light_sensor_addr.addr = BH1750FVI_ADDRESS_S2 + i * 2;
//                light_sensor_addr.flag = 1;
//                break;
//            }
//        }
//
//    }


    if (light_sensor_addr.flag) {
        /* 上电 */
        i2c_cmd_write(light_sensor_addr.periph, light_sensor_addr.addr, 0x01);
    }


}

int get_light(void) {
    /* 测量亮度*/
    i2c_cmd_write(light_sensor_addr.periph, light_sensor_addr.addr, 0x10);
    /* 大致测量时间*/
    delay_ms(150);

    /* 读出数据*/
    uint8_t light_data[2] = {0};
    i2c_read(light_sensor_addr.periph, light_sensor_addr.addr, 1, light_data, 2);

    return (light_data[0] << 8) + light_data[1];
}

