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

	init_adder();

	set_adder(3,5);

    int x = get_adder();
    int_to_string(x,s);
    puts(s);
    puts("\n");

    return 0;
}
