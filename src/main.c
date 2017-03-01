/**
 ******************************************************************************
 /**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 ******************************************************************************
 */

#include "bsp.h"
#include "stm32f4_discovery.h"

/* Standard includes */
#include <stdio.h>                          // prototype declarations for I/O functions
#include <string.h>

/* Scheduler includes */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

TaskHandle_t TaskB, TaskC;
SemaphoreHandle_t xSemaphore;
QueueHandle_t Queue1, Queue2;

void RESET_ALL_LED(void);

char buffer[100];
int size;
uint16_t ContadorTime = 100;

typedef struct {
	uint8_t Patron;
	uint16_t Velocidad;
} PatronVelocidad;

void SW_State(void* pvParameters) {
	(void) pvParameters;
	PatronVelocidad pv;
	Queue1 = xQueueCreate(10, sizeof(unsigned int));
	if (Queue1 == NULL) {
		/* Queue was not created and must not be used. */
	}
	Queue2 = xQueueCreate(10, sizeof(unsigned int));
	if (Queue2 == NULL) {
		/* Queue was not created and must not be used. */
	}

	pv.Velocidad = 100;
	pv.Patron = 0;
	int Actualizar = 1;

	for (;;) {

		if (!get_sw_state(SW_RIGHT)) {
			if (pv.Patron < 2) {
				pv.Patron++;
			} else {
				pv.Patron = 0;
			}
			Actualizar = 1;
		}
		if (!get_sw_state(SW_LEFT)) {
			Actualizar = 1;
			if (pv.Patron <= 0) {
				pv.Patron = 2;
			} else {
				pv.Patron--;
			}
		}
		if (!get_sw_state(SW_DOWN)) {

			if (pv.Velocidad < 5000) {
				if (pv.Velocidad <= 99) {
					pv.Velocidad = pv.Velocidad + 5;
				} else {
					pv.Velocidad = pv.Velocidad + 200;
				}
				Actualizar = 1;
			}
		}
		if (!get_sw_state(SW_UP)) {

			if (pv.Velocidad > 1) {
				if (pv.Velocidad <= 100) {
					pv.Velocidad = pv.Velocidad - 5;
				} else {
					pv.Velocidad = pv.Velocidad - 200;
				}
				Actualizar = 1;
			}
		}
		if (Actualizar) {
			Actualizar = 0;
			xQueueSend(Queue1, (void * ) &pv, (TickType_t ) 0);
			xQueueSend(Queue2, (void * ) &pv, (TickType_t ) 0);
			vTaskResume(TaskC);
		}
		vTaskDelay(300);
	}

}

void Led_Parametro(void* pvParameters) {
	(void) pvParameters;
	PatronVelocidad pn;
	uint8_t Patron = 0;
	uint16_t Velocidad = 0;
	uint8_t poten, contador = 0;
	uint8_t off, reset = 1;

	for (;;) {
		if (Queue1 != 0) {
			if (xQueueReceive(Queue1, (void * ) &pn, (TickType_t ) 10)) {
				Patron = pn.Patron;
				Velocidad = pn.Velocidad;
				ContadorTime = pn.Velocidad;
				off = 1;
			}
		}
		if (ContadorTime <= 0) {
			ContadorTime = Velocidad;
		}
		if (ContadorTime == Velocidad) {
			switch (Patron) {
			case 0:
				if (contador > 8) {
					contador = 0;
				}
				if (off) {
					off = 0;
					RESET_ALL_LED();
					contador = 0;
				}
				switch (contador) {
				case 0:
					contador++;
					led_on(LED0_BOAR);
					break;
				case 1:
					contador++;
					led_off(LED0_BOAR);
					led_on(LED1_BOAR);
					break;
				case 2:
					contador++;
					led_off(LED1_BOAR);
					led_on(LED2_BOAR);
					break;
				case 3:
					contador++;
					led_off(LED2_BOAR);
					led_on(LED3_BOAR);
					break;
				case 4:
					contador++;
					led_off(LED3_BOAR);
					led_on(LED6_BOAR);
					break;
				case 5:
					contador++;
					led_off(LED6_BOAR);
					led_on(LED7_BOAR);
					break;
				case 6:
					contador++;
					led_off(LED7_BOAR);
					led_on(LED10_BOAR);
					break;
				case 7:
					led_off(LED10_BOAR);
					led_on(LED11_BOAR);
					contador++;
					break;
				case 8:
					led_off(LED11_BOAR);
					contador++;
					break;
				default:
					break;
				}
				break;

			case 1:
				if (contador > 15) {
					contador = 0;
				}
				if (off) {
					off = 0;
					RESET_ALL_LED();
					contador = 0;
				}
				switch (contador) {
				case 0:
					contador++;
					led_on(LED11_BOAR);
					break;
				case 1:
					contador++;
					led_on(LED0_BOAR);
					break;
				case 2:
					contador++;
					led_on(LED10_BOAR);
					break;
				case 3:
					contador++;
					led_on(LED1_BOAR);
					break;
				case 4:
					contador++;
					led_on(LED7_BOAR);
					break;
				case 5:
					contador++;
					led_on(LED2_BOAR);
					break;
				case 6:
					contador++;
					led_on(LED6_BOAR);
					break;
				case 7:
					led_on(LED3_BOAR);
					contador++;
					break;
				case 8:
					led_off(LED11_BOAR);
					contador++;
					break;
				case 9:
					led_off(LED0_BOAR);
					contador++;
					break;
				case 10:
					led_off(LED10_BOAR);
					contador++;
					break;
				case 11:
					led_off(LED1_BOAR);
					contador++;
					break;
				case 12:
					led_off(LED7_BOAR);
					contador++;
					break;
				case 13:
					led_off(LED2_BOAR);
					contador++;
					break;
				case 14:
					led_off(LED6_BOAR);
					contador++;
					break;
				case 15:
					led_off(LED3_BOAR);
					contador++;
					break;
				default:
					break;
				}
				break;
			case 2:
				if (contador > 3) {
					contador = 0;
				}
				if (off) {
					off = 0;
					RESET_ALL_LED();
					contador = 0;
				}
				switch (contador) {
				case 0:
					contador++;
					led_on(LED3_BOAR);
					led_on(LED6_BOAR);
					break;
				case 1:
					contador++;
					led_off(LED3_BOAR);
					led_off(LED6_BOAR);
					led_on(LED7_BOAR);
					led_on(LED2_BOAR);
					break;
				case 2:
					contador++;
					led_off(LED7_BOAR);
					led_off(LED2_BOAR);
					led_on(LED10_BOAR);
					led_on(LED1_BOAR);
					break;
				case 3:
					contador++;
					led_off(LED10_BOAR);
					led_off(LED1_BOAR);
					led_on(LED11_BOAR);
					led_on(LED0_BOAR);
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}
		vTaskDelay(1);
	}
}

void UART_State(void* pvParameters) {
	(void) pvParameters;
	PatronVelocidad pn;
	uint8_t Patron = 0;
	uint16_t Velocidad = 0;
	for (;;) {
		if (Queue2 != 0) {
			if (xQueueReceive(Queue2, (void * ) &pn, (TickType_t ) 10)) {
				Patron = pn.Patron;
				Velocidad = pn.Velocidad;
			}
		}
		size = sprintf(buffer, "Velocidad (mSeg): %d\n\r Patron: %d \n\r",
				Velocidad, Patron);
		TransmitData(buffer, size);
		vTaskSuspend(NULL);

	}
}

void RESET_ALL_LED(void) {
	led_off(LED0_BOAR);
	led_off(LED1_BOAR);
	led_off(LED2_BOAR);
	led_off(LED3_BOAR);
	led_off(LED6_BOAR);
	led_off(LED7_BOAR);
	led_off(LED10_BOAR);
	led_off(LED11_BOAR);
}

int main(void) {

	BSP_Init();

	xTaskCreate(SW_State, (signed char * ) "TaskA", configMINIMAL_STACK_SIZE,
			(void * ) 1, tskIDLE_PRIORITY, ( xTaskHandle * ) NULL);

	xTaskCreate(Led_Parametro, (signed char * ) "TaskB",
			configMINIMAL_STACK_SIZE, (void * ) 1, tskIDLE_PRIORITY, &TaskB);

	xTaskCreate(UART_State, (signed char * ) "TaskC", configMINIMAL_STACK_SIZE,
			(void * ) 1, tskIDLE_PRIORITY, &TaskC);

	vTaskStartScheduler();

	//should never get here
	printf("ERORR: vTaskStartScheduler returned!");
	for (;;)
		;
}

void APP_1ms() {
	if (ContadorTime > 0) {
		ContadorTime--;
	}
}

