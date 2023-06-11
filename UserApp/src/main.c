
#include "inc/i2c.h"
#include "inc/systick.h"
#include "inc/tools.h"
#include "inc/temper_S2.h"
#include "inc/display_E1.h"
#include "inc/key_S1.h"
#include "inc/led_E1.h"

i2c_addr_def key_addr;//Key address struct
i2c_addr_def LED_addr;//LED address struct
i2c_addr_def dis_sensor_addr;
i2c_addr_def display_addr;
i2c_addr_def temper_sensor_addr;
i2c_addr_def light_sensor_addr;

/**
 * 实现 数码管交替显示温湿度 的功能
 * 每 4 s ，切换一次显示内容
 *
 * 温度
 * 1.为 摄氏温度
 * 2.显示温度时，温度较高，led为红色，温度较低，led为蓝色
 * 湿度
 * 1.为 相对湿度
 * 2.显示湿度时候，led为黄色
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
    // 各个子板初始化
    temper_S2_i2c_init();/*温湿度子板*/
    Display_i2c_init();/*数码管显示器初始化*/
    led_E1_i2c_init();/*led初始化*/
    // 打开数码管显示功能
    enable_display();

    uint8_t key_now = 1; /*表示当前要输出什么*/

    while (1) {
        double b = get_temper();/*获取温度*/
        double c = get_hum();/*获取湿度*/

        /**
         * 显示温度
         */
        display_double(b);
        if (b > 24.0){
            led_control(1);
        } else{
            led_control(3);
        }
        delay_1ms(4000);
        /**
         * 显示湿度
         */
        display_double(c);
        led_control(2);
        delay_1ms(4000);

    }
}
