//
// Created by 18466 on 2023/6/11.
//

#include "inc/WM8978.h"
#include "gd32f4xx_spi.h"

short play_data[] = {
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000,
        0, 3000, 0, -3000, 0, 3000, 0, -3000   // 1KHz正弦波,左声道
};

void sound_E4_init() {

    // 驱动初始化
    WM8978_Init();
    WM8978_ADDA_Cfg(1, 0);
    WM8978_Output_Cfg(1, 1);
    WM8978_HPvol_Set(50, 50);
    WM8978_I2S_Cfg(2, 0);

    // 配置GPIO使能I2S功能
    gpio_af_set(GPIOB, GPIO_AF_5, GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_13);
    // 初始化I2S2外设
    spi_i2s_deinit(SPI2);
    i2s_init(I2S2_ADD, I2S_MODE_SLAVETX, I2S_STD_PHILLIPS, I2S_CKPL_LOW);
    i2s_psc_config(I2S1_ADD, I2S_AUDIOSAMPLE_8K, I2S_FRAMEFORMAT_DT16B_CH16B, I2S_MCKOUT_DISABLE);
    // 中断配置
    spi_i2s_interrupt_enable(I2S2_ADD, SPI_I2S_INT_TBE);

}

void I2S2_IRQHandler(void) {
    int i = 0;
    if (spi_i2s_interrupt_flag_get(I2S2_ADD, SPI_I2S_INT_TBE) == SET) {
        //发送一个样本数据
        spi_i2s_data_transmit(I2S2_ADD, play_data[i]);
        i++;
    }
}

void play_sound() {
    //启动I2S2
    i2s_enable(I2S2_ADD);
}
void stop_sound(){
    i2s_disable(I2S2_ADD);
}