#include <FreeRTOS.h>
#include <task.h>

#include "riscv-virt.h"
#include "ns16550.h"

int uart_puts(const char *msg);

static void Task1(void *p_arg)
{ 
    for(int i = 0;i < 10;i++)
    {
        uart_puts("Task1\n");
        vTaskDelay(50);
    }

    vTaskDelete(NULL);
}

static void Task2(void *p_arg)
{ 
	for(int i = 0;i < 10;i++)
    {
        uart_puts("Task2\n");
        vTaskDelay(100);
    }

    vTaskDelete(NULL);
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

int main(void)
{
	//float f = 1.234;
    int i = 1234;
    char c[10];
    int_to_string(i,c);
    uart_puts(c);

	uart_puts("FreeRTOS! Start\n");
    
    xTaskCreate(Task1,"Task1",256,(void*)1,1,NULL);
	uart_puts("Task1 Create\n");
    xTaskCreate(Task2,"Task2",256,(void*)1,1,NULL);
	uart_puts("Task2 Create\n");

	vTaskStartScheduler();

    uart_puts("vTask Finished\n");

	return 0;
}

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	uart_puts("vApplicationMallocFailedHook\n");
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	uart_puts("vApplicationIdleHook\n");
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	uart_puts("vApplicationStackOverflowHook\n");
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
	//uart_puts("vApplicationTickHook\n");
}
/*-----------------------------------------------------------*/

void vAssertCalled( void )
{
	uart_puts("vAssertCalled\n");
    volatile uint32_t ulSetTo1ToExitFunction = 0;

	taskDISABLE_INTERRUPTS();
	while( ulSetTo1ToExitFunction != 1 )
	{
		__asm volatile( "NOP" );
	}
}