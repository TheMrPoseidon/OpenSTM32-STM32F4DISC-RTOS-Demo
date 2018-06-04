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

const int delta = 100;

void prvCalculateTask( void *pvParameters ) {
	int last_x = 0, last_y = 0;
	int ax = 0, ay = 0;
	uint32_t round = 0, checked = 0;
	for ( ;; )
	{
		ax = x;
		ay = y;
		if (abs(last_x - x) > delta && abs(last_y - y) > delta)
		{
			if(abs((last_x/last_y)*(x/y)) >= 0.9)
			{
				checked += 1;

				if (checked % 4 == 0)
				{
					xSemaphoreGive(xSemaphoreBlink);
				}
			}
			last_x = x;
			last_y = y;
		}
		else
			round += 1;

		if(round >= 100)
		{
			round = checked = 0;
		}

		vTaskDelay(20);
	}
}

static void prvSetupHardware( void )
{
    NVIC_SetPriorityGrouping( 0 );
}
