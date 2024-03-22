#include "adder_driver.h"
#include <stdint.h>

void init_adder()
{
    *((uint64_t*)(ADDER_ADDR+REG_INIT)) = 1;
}

void set_adder(int a,int b)
{
    *((uint64_t*)(ADDER_ADDR+REG_A)) = (uint64_t)a;
    *((uint64_t*)(ADDER_ADDR+REG_B)) = (uint64_t)b;
}

int get_adder()
{
    int r = (int)(*((uint64_t*)(ADDER_ADDR+REG_RESULT)));
    return r;
}