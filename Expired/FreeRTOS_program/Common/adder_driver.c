#include "adder_driver.h"
#include <stdint.h>

#ifdef __ASSEMBLER__
#define CONS(NUM, TYPE)NUM
#else
#define CONS(NUM, TYPE)NUM##TYPE
#endif /* __ASSEMBLER__ */

#define ADDER_ADDR		CONS(0x10000800, UL)
/* register definitions */
#define REG_ID                 0x0
#define REG_INIT               0x4
#define REG_CMD                0x8
#define REG_INT_STATUS         0xc
#define REG_A                  0x10
#define REG_B                  0x14
#define REG_RESULT             0x18

int HW_adder_irq_flag = 0;

void int_to_string(int n, char *s);

void init_adder()
{
    *((uint32_t*)(ADDER_ADDR+REG_INIT)) = 1;
    HW_adder_irq_flag = 0;
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

int HW_adder(int a, int b)
{
    set_adder(a,b);
    while(!HW_adder_irq_flag){;}
    HW_adder_irq_flag = 0;
    return get_adder();
}