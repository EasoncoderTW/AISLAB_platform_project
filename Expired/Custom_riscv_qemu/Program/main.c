// function declaration for the UART print function in boot.s
#include <stdint.h>
#include "adder_driver.h"
int puts(const char *msg);

void int_to_string(int n, char *s)
{
	int i = n;
	int str_len = 0;
	while(i>0)
	{
		i = i/10;
		str_len++;
	}
	if(str_len == 0) str_len = 1; // '0'
	i = n;
	for(int x = str_len-1;x>=0;x--)
	{
		s[x] = (i%10)+'0';
		i = i/10;
	}
	s[str_len] = '\0';
}

void int_to_hex_string(uint64_t n, char *s)
{
	uint64_t i = n;
	int str_len = 0;
	while(i>0)
	{
		i = i/16;
		str_len++;
	}
	if(str_len == 0) str_len = 1; // '0'
	i = n;
	for(int x = str_len-1;x>=0;x--)
	{
        if((i%16) < 10)s[x] = (i%16)+'0';
        else s[x] = (i%16)-10+'A';
		i = i/16;
	}
	s[str_len] = '\0';
}

int main()
{
    char s[64];
    /*for(int i = 0;i<0x20;i+=4)
    {
        //uintptr_t addr = ADDER_ADDR+i;
        //int r = *( (uint32_t *) addr );
        

        int_to_string(i,s);
        puts(s);
        puts("\n");
    }*/

	uint8_t *y = (uint8_t*)0x10000804;
	*y = 1;


    y = (uint8_t*)0x10000800;
    int_to_hex_string(((uint64_t)y),s);
    puts(s);
    puts("\n");

    uint8_t x = *y;
    int_to_string(((int)x),s);
    puts(s);
    puts("\n");

    return 0;
}
