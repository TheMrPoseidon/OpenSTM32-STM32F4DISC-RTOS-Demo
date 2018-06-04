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
#include "semphr.h"
#include "hardware.h"

int16_t x, y, z;
SemaphoreHandle_t xSemaphoreBlink;

static void prvSetupHardware( void );
void prvBlinkTask(void *pvParameter);
void prvAccelerometerTask( void *pvParameters );
void prvCalculateTask( void *pvParameters );

int main(void)
{
	/* Setup the microcontroller hardware for the demo. */
	prvSetupHardware();

	xSemaphoreBlink = xSemaphoreCreateBinary();

	/*
	 * Create the first LED Task
	 */
	xTaskCreate(prvAccelerometerTask, "AccelerometerTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(prvBlinkTask, "ATask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(prvCalculateTask, "CalculateTask", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

	vTaskStartScheduler();

	for(;;) { }

	return 0;
}

void prvBlinkTask( void *pvParameters )
{
	BSP_LED_Init(LED3);
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LED_Init(LED6);
	for( ;; )
	{
		if( xSemaphoreTake( xSemaphoreBlink, ( TickType_t ) 100 ) == pdTRUE )
		{
			for(int i = 0; i < 4; i++)
			{
				BSP_LED_On(LED3); BSP_LED_On(LED4); BSP_LED_On(LED5); BSP_LED_On(LED6);
				vTaskDelay(200);

				BSP_LED_Off(LED3); BSP_LED_Off(LED4); BSP_LED_Off(LED5); BSP_LED_Off(LED6);
				vTaskDelay(200);
			}
		}
	}
}

void prvAccelerometerTask( void *pvParameters )
{
	BSP_ACCELERO_Init();
	int16_t axes[3];

	for( ;; )
	{
		BSP_ACCELERO_GetXYZ(axes);
		x = axes[0];
		y = axes[1];
		z = axes[2];
		vTaskDelay(10);
	}
}

const int tap = 1020;

void prvCalculateTask( void *pvParameters ) {
	for(;;)
	{
		if(z > tap) {
			while(z > tap - 10) vTaskDelay(10);

			vTaskDelay(100);

			for(int i = 0; i < 10; i++)
			{
				if(z > tap){
					xSemaphoreGive(xSemaphoreBlink);
					break;
				}
				vTaskDelay(10);
			}
		}
		vTaskDelay(50);
	}
}

static void prvSetupHardware( void )
{
    NVIC_SetPriorityGrouping( 0 );
}
