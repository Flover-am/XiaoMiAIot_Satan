#include "led.h"

void LED_Init(void)
{
    //时钟使能
    rcu_periph_clock_enable(LED_CLOCK);
    //配置GPIO模式
    gpio_mode_set(LED_Port, LED_Mode, LED_Pull, LED_Pin);
    //配置推挽输出  输出速度50Mhz
    gpio_output_options_set(LED_Port, LED_Out_Type, LED_Pin_Speed, LED_Pin);
    
}
