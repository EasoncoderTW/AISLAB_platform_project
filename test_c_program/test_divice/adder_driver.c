#include "adder_driver.h"
#include <stdint.h>

void init_adder()
{
    *((uint32_t*)(ADDER_ADDR+REG_INIT)) = 1;
}

void set_adder(int a,int b)
{
    *((uint32_t*)(ADDER_ADDR+REG_A)) = (uint32_t)a;
    *((uint32_t*)(ADDER_ADDR+REG_B)) = (uint32_t)b;
}

int get_adder()
{
    int r = (int)(*((uint32_t*)(ADDER_ADDR+REG_RESULT)));
    return r;
}