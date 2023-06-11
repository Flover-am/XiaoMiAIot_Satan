
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
#include "inc/distance_S6.h"
#include "inc/sound_E4.h"

i2c_addr_def key_addr;//Key address struct
i2c_addr_def LED_addr;//LED address struct
i2c_addr_def dis_sensor_addr;
i2c_addr_def display_addr;
i2c_addr_def temper_sensor_addr;
i2c_addr_def light_sensor_addr;
i2c_addr_def curtain_addr;

/**
 * 模拟人真实起床的场景：
 * 闹钟倒计时，时间到了，闹钟响
 *      -这时候，检测光照，得知外面是否是白天/晴天还是黑天/阴天
 *          -如果是白天，那么打开窗帘
 *          -如果是黑天，那么不开窗帘
 * 关闭闹钟的方式是检测人体朝着闹钟靠近（说明人已经起床了）
 * 闹钟关闭之后，交替显示当前的温度和湿度
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

    // 各个子板的初始化
    curtain_E3_i2c_init();
    display_E1_i2c_init();
    distance_S6_i2c_init();
    key_S1_i2c_init();
    led_E1_i2c_init();
    light_S2_i2c_init();
    sound_E4_init();

    bool is_curtain_open = false; // 初始状态是关着的
    bool is_alarm_ring = false;
    bool is_temp2display = false;

    uint64_t time_left = 30000;
    uint64_t distance_init = 0;
    while (1) {
        // 时间倒计时
        if (time_left > 0) {

            delay_1ms(1000);
            time_left--;
            continue;
        }
        // 时间到！ 播放闹铃, 检测人的距离
        if (!is_alarm_ring) {
            play_sound();
            distance_init = get_distance();
        }
        is_alarm_ring = true;

        // 检测光照，判断是否要开窗帘 //短路实现哈哈哈
        if (!is_curtain_open && get_light() > 300) {
            /*如果是白天，那么打开窗帘*/
            open_curtain();
            is_curtain_open = true;
        }

        // 此后开始检测人是否起床了，起床就关闭闹铃，不检测距离了
        // 起床之后，开始实时展示温度
        if (is_alarm_ring && get_distance() - distance_init > 1000) {
            is_alarm_ring = false;
            stop_sound();
            is_temp2display = true;
        }

        if (is_temp2display) {
            // 展示温度
            double temp = get_temper();
            double hum = get_hum();
            display_double(temp);
            delay_1ms(2000);
            display_double(hum);
            delay_1ms(2000);
        }

    }
}
