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

int16_t x, y, z;

static void prvSetupHardware( void );
void prvATask(void *pvParameter);
void prvBTask(void *pvParameter);
void prvCTask(void *pvParameter);
void prvDTask(void *pvParameter);
void prvAccelerometerTask( void *pvParameters );

int main(void)
{
	/* Setup the microcontroller hardware for the demo. */
	prvSetupHardware();
	BSP_ACCELERO_Init();

	/*
	 * Create the first LED Task
	 */
	xTaskCreate(prvAccelerometerTask, "AcceleroTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
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
	BSP_LED_Init(LED5);
	for( ;; )
	{
		if(x > 100)
			BSP_LED_On(LED5);
		else
			BSP_LED_Off(LED5);
		vTaskDelay(50);
	}
}

void prvBTask( void *pvParameters )
{
	BSP_LED_Init(LED4);
	for( ;; )
	{
		if(x <  -100)
			BSP_LED_On(LED4);
		else
			BSP_LED_Off(LED4);
		vTaskDelay(50);
	}

}

void prvCTask( void *pvParameters )
{
	BSP_LED_Init(LED3);
	for( ;; )
	{
		if(y > 100)
			BSP_LED_On(LED3);
		else
			BSP_LED_Off(LED3);
		vTaskDelay(50);
	}

}

void prvDTask( void *pvParameters )
{
	BSP_LED_Init(LED6);
	for( ;; )
	{
		if(y < -100)
			BSP_LED_On(LED6);
		else
			BSP_LED_Off(LED6);
		vTaskDelay(50);
	}
}

void prvAccelerometerTask( void *pvParameters )
{
	int16_t axes[3];

	for( ;; )
	{
		BSP_ACCELERO_GetXYZ(axes);
		x = axes[0];
		y = axes[1];
		z = axes[2];
		vTaskDelay(50);
	}
}

static void prvSetupHardware( void )
{
    NVIC_SetPriorityGrouping( 0 );
}
