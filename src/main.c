/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/
#include "FreeRTOS.h" /* Must come first. */

#include "hardware.h"

static void prvSetupHardware( void );
void prvATask(void *pvParameter);
void prvBTask(void *pvParameter);
void prvCTask(void *pvParameter);
void prvDTask(void *pvParameter);

int main(void)
{
	/* Setup the microcontroller hardware for the demo. */
	prvSetupHardware();


	/*
	 * Create the first LED Task
	 */
	xTaskCreate(prvATask, "ATask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(prvBTask, "BTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(prvCTask, "CTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(prvDTask, "DTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	vTaskStartScheduler();

	for(;;) { }

	return 0;
}

void prvATask( void *pvParameters )
{
	BSP_LED_Init(LED3);
	for( ;; )
	{
		BSP_LED_Toggle(LED3);
		vTaskDelay(21);
	}

}

void prvBTask( void *pvParameters )
{
	BSP_LED_Init(LED4);
	for( ;; )
	{
		BSP_LED_Toggle(LED4);
		vTaskDelay(42);
	}

}

void prvCTask( void *pvParameters )
{
	BSP_LED_Init(LED5);
	for( ;; )
	{
		BSP_LED_Toggle(LED5);
		vTaskDelay(84);
	}

}

void prvDTask( void *pvParameters )
{
	BSP_LED_Init(LED6);
	for( ;; )
	{
		BSP_LED_Toggle(LED6);
		vTaskDelay(168);
	}

}

static void prvSetupHardware( void )
{
    NVIC_SetPriorityGrouping( 0 );
}
