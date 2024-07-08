// function declaration for the UART print function in boot.s
#include <stdint.h>

int puts(const char *msg);


#define MSTATUS_MIE_BIT   (1 << 3)
#define MSTATUS_MPIE_BIT  (1 << 7)
#define MIP_MTIP_BIT      (1 << 7)

uint32_t read_mstatus() {
    uint32_t mstatus;
    asm volatile ("csrr %0, mstatus" : "=r"(mstatus));
    return mstatus;
}

uint32_t read_mip() {
    uint32_t mip;
    asm volatile ("csrr %0, mip" : "=r"(mip));
    return mip;
}

uint64_t read_timer() {
    uint64_t timer_value;
    asm volatile ("rdtime %0" : "=r"(timer_value));
    return timer_value;
}

void get_time()
{
   uint64_t timer_value = read_timer();
    puts("Timer value:");

    char s[30];
    for(int i = 0;i<30;i++)s[i] = ' ';
    for(int i = 27;i > 0; i--)
    {
      s[i] = (char)(timer_value%10+'0');
      timer_value = timer_value/10;
      if(timer_value == 0)
      {
         break;
      } 
    }
    s[28] = '\n';
    s[29] = '\0';

    puts(s);
}

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

int main() {
    
   int x;
   uint32_t mstatus;
   uint32_t mip;
   for(int i = 0;i < 10 ;i++)
   {
      x = 0;
      for(int j = 0;j<1000;j++)
      {
         x++;
      }
      get_time();
      mstatus = read_mstatus();
      mip = read_mip();
      if ((mstatus & MSTATUS_MIE_BIT) && !(mstatus & MSTATUS_MPIE_BIT) && (mip & MIP_MTIP_BIT)) {
        puts("Timer interrupt triggered\n");
    } else {
        puts("No timer interrupt\n");
    }
   }
    return 0;
}
