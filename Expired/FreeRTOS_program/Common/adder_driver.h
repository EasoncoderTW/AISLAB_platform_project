#ifndef ADDER_DRIVER_H
#define ADDER_DRIVER_H

int uart_puts(const char *msg);

void init_adder();
void set_adder(int a,int b);
int get_adder();
int HW_adder(int a, int b);

extern int HW_adder_irq_flag;

#endif