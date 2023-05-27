//
// Created by 18466 on 2023/5/27.
//

#ifndef CLION_TEST_TOOLS_H
#define CLION_TEST_TOOLS_H

#include "inc/tools.h"
#include "stdlib.h"
#include "math.h"

int binary2int(char *a) {
    char *endp;
    int num = 0;
    num = strtol(a, endp, 2);
    return num;
}

int get_digit(int num, int place) {
    // supposed 0 digit 1234 -> 4
    num /= (int) pow(10, place);
    int d = num % 10;
    return d;
}


#endif //CLION_TEST_TOOLS_H
