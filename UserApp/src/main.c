
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
* 实现通过按键，控制窗帘开关
 * 状态信息（黑天白天）通过 led显示
 * led：
 *      红色：白天
 *      黄色：黑天
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

//    light_S2_i2c_init();
    key_S1_i2c_init();
    display_E1_i2c_init();
    curtain_E3_i2c_init();
    led_E1_i2c_init();
    int time = 0;
    bool is_open = true; // 初始状态开着的
    while (1) {
         int key = get_key();
        if (key == 1 && !is_open) {
            is_open = true;
            open_curtain();
        } else if (key == 2 && is_open) {
            close_curtain();
            is_open = false;
        }
        if (is_open) {
            led_control(1);
        } else {
            led_control(2);
        }
        delay_1ms(20);
        disable_display();
    }
}
