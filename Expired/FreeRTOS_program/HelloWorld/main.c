#include <FreeRTOS.h>
#include <task.h>

#include "riscv-virt.h"
#include "ns16550.h"

int uart_puts(const char *msg);

void HelloWorld(void *p_arg)
{ 
	//configASSERT(p_arg);
	
	for( ;; )
    {
		uart_puts("Hello World\n");
		vTaskDelay(100);
        /* Task code goes here. */
    }
}

int main(void)
{
	uart_puts("Hello FreeRTOS!\n");
	
    BaseType_t xReturned ;
    
    xReturned  = xTaskCreate(HelloWorld,"HelloWorld",128,(void*)1,1,NULL);

    if(xReturned == pdPASS)
    {
        uart_puts("xTaskCreate PASS\n");
    }
    else
    {
        uart_puts("xTaskCreate FAIL\n");
    }

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