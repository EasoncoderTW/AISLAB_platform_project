#ifndef ADDER_DRIVER_H
#define ADDER_DRIVER_H

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


void init_adder();
void set_adder(int a,int b);
int get_adder();

#endif