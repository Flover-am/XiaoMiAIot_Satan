
#include <stdbool.h>
#include "inc/i2c.h"
#include "inc/systick.h"
#include "inc/tools.h"
#include "inc/temper_S2.h"
#include "inc/display_E1.h"
#include "inc/key_S1.h"
#include "inc/led_E1.h"
#include "inc/light_S2.h"
#include "inc/curtain_E3.h"

i2c_addr_def key_addr;//Key address struct
i2c_addr_def LED_addr;//LED address struct
i2c_addr_def dis_sensor_addr;
i2c_addr_def display_addr;
i2c_addr_def temper_sensor_addr;
i2c_addr_def light_sensor_addr;
i2c_addr_def curtain_addr;

/**
 * 实现通过感应光照来判断黑天和白天，从而开关窗帘
 * 窗帘打开时，当照度持续低于 200lux，关闭窗帘
 * 窗帘关闭时，当照度持续大于 200lux，打开窗帘
 * 状态信息（黑天白天，关注强度）通过 数码管和led显示
 * led：
 *      红色：白天
 *      黄色：黑天
 * 数码管：实时显示照度
 * @return
 */
int main(void) {
    // i2c初始化
    i2c0_gpio_config();
    i2c0_config();
    i2c1_gpio_config();
    i2c1_config();
    // 时钟初始化
    systick_config();//system tick configuration

    light_S2_i2c_init();
    display_E1_i2c_init();
    curtain_E3_i2c_init();
    led_E1_i2c_init();

    uint16_t is_sun = 0;
    uint16_t is_night = 0;
    bool is_open = true; // 初始状态开着的
    while (1) {
        uint16_t light = get_light();
        if (light > 400 && !is_open) {
            is_sun++;
        } else if (light < 200 && is_open) {
            is_night++;
        }
        if (is_sun > 10) {
            open_curtain();
            is_open = true;
            is_sun = 0;
        } else if (is_night > 10) {
            close_curtain();
            is_open = false;
            is_night = 0;
        }

        display_int(light);
        if (is_open) {
            led_control(1);
        } else {
            led_control(2);
        }
        delay_1ms(100);
    }
}
