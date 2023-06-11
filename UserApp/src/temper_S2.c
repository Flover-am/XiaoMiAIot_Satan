//
// Created by 18466 on 2023/6/11.
//
#include "inc/temper_S2.h"
#include "inc/i2c.h"
#include "delay.h"
#include "inc/main.h"

extern i2c_addr_def temper_sensor_addr;

void temper_S2_i2c_init(void) {
    uint8_t i = 0;
    for (i = 0; i < 4; i++) {
        if (i2c_addr_poll(I2C0, TEMPER_ADDRESS_S2 + i * 2)) {
            temper_sensor_addr.periph = I2C0;
            temper_sensor_addr.addr = TEMPER_ADDRESS_S2 + i * 2;
            temper_sensor_addr.flag = 1;
            break;
        }
    }
    //address not read
    if (temper_sensor_addr.flag != 1) {
        //poll I2C1,Verti1
        for (i = 0; i < 4; i++) {
            if (i2c_addr_poll(I2C1, TEMPER_ADDRESS_S2 + i * 2)) {
                temper_sensor_addr.periph = I2C1;
                temper_sensor_addr.addr = TEMPER_ADDRESS_S2 + i * 2;
                temper_sensor_addr.flag = 1;
                break;
            }
        }
    }
}

double get_temper(void) {
    uint8_t res[20] = {0x0D};
    i2c_read_cmd(temper_sensor_addr.periph, temper_sensor_addr.addr, 0x2C/*MSB*/, 0x0D/*LSB*/, res, 6, 150);
    uint32_t temp_get = (res[0] << 8) + res[1];
    double temp = -45 + 175.0 * temp_get / ((1 << 16) - 1);
    return temp;
}

double get_hum(void) {
    uint8_t res[20] = {0x0D};
    i2c_read_cmd(temper_sensor_addr.periph, temper_sensor_addr.addr, 0x2C/*MSB*/, 0x0D/*LSB*/, res, 6, 150);
    uint32_t hum_get = (res[3] << 8) + res[4];
    double hum = 100.0 * hum_get / ((1 << 16) - 1);
    return hum;
}