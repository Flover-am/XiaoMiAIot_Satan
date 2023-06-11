//
// Created by 18466 on 2023/6/11.
//

#ifndef CLION_TEST_DISPLAY_E1_H
#define CLION_TEST_DISPLAY_E1_H

void display_E1_i2c_init(void);

void enable_display(void);

void disable_display(void);

void display_byte(int which_place, int num);

void clear_display_byte(int which_place);

void display_int(int num);

void displya_double(double num) ;

#endif //CLION_TEST_DISPLAY_E1_H
