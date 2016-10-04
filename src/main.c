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
void SET_ALL_LED(void);
void TOGGEL_ALL_LED(void);

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
			if (pv.Patron < 5) {
				pv.Patron++;
			} else {
				pv.Patron = 0;
			}
			Actualizar = 1;
		}
		if (!get_sw_state(SW_LEFT)) {
			Actualizar = 1;
			if (pv.Patron <= 0) {
				pv.Patron = 5;
			} else {
				pv.Patron--;
			}
		}
		if (!get_sw_state(SW_DOWN)) {

			if (pv.Velocidad < 5000) {
				if (pv.Velocidad <= 99) {
					pv.Velocidad = pv.Velocidad + 1;
				} else {
					pv.Velocidad = pv.Velocidad + 100;
				}
				Actualizar = 1;
			}
		}
		if (!get_sw_state(SW_UP)) {

			if (pv.Velocidad > 1) {
				if (pv.Velocidad <= 100) {
					pv.Velocidad = pv.Velocidad - 1;
				} else {
					pv.Velocidad = pv.Velocidad - 100;
				}
				Actualizar = 1;
			}
		}
		if (get_sw_state(SW_DISC)) {
			Actualizar = 1;
		}
		if (Actualizar) {
			Actualizar = 0;
			//mandar mail box
			xQueueSend(Queue1, (void * ) &pv, (TickType_t ) 0);

			xQueueSend(Queue2, (void * ) &pv, (TickType_t ) 0);
			vTaskResume(TaskC);
		}
		vTaskDelay(400);
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
				if (contador > 22) {
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
					led_off(LED11_BOAR);
					led_on(LED10_BOAR);
					break;
				case 2:
					contador++;
					led_off(LED10_BOAR);
					led_on(LED7_BOAR);
					break;
				case 3:
					contador++;
					led_off(LED7_BOAR);
					led_on(LED6_BOAR);
					break;
				case 4:
					contador++;
					led_off(LED6_BOAR);
					led_on(LED3_BOAR);
					break;
				case 5:
					contador++;
					led_off(LED3_BOAR);
					led_on(LED2_BOAR);
					break;
				case 6:
					contador++;
					led_off(LED2_BOAR);
					led_on(LED1_BOAR);
					break;
				case 7:
					led_off(LED1_BOAR);
					led_on(LED0_BOAR);
					contador++;
					break;
				case 8:
					led_off(LED0_BOAR);
					led_on(LED_ROJO);
					contador++;
					break;
				case 9:
					led_off(LED_ROJO);
					led_on(LED_NARANJA);
					contador++;
					break;
				case 10:
					led_off(LED_NARANJA);
					led_on(LED_VERDE);
					contador++;
					break;
				case 11:
					led_off(LED_VERDE);
					led_on(LED_AZUL);
					contador++;
					break;
				case 12:
					led_off(LED_AZUL);
					led_on(LED_VERDE);
					contador++;
					break;
				case 13:
					led_off(LED_VERDE);
					led_on(LED_NARANJA);
					contador++;
					break;
				case 14:
					led_off(LED_NARANJA);
					led_on(LED_ROJO);
					contador++;
					break;
				case 15:
					led_off(LED_ROJO);
					led_on(LED0_BOAR);
					contador++;
					break;
				case 16:
					led_off(LED0_BOAR);
					led_on(LED1_BOAR);
					contador++;
					break;
				case 17:
					led_off(LED1_BOAR);
					led_on(LED2_BOAR);
					contador++;
					break;
				case 18:
					led_off(LED2_BOAR);
					led_on(LED3_BOAR);
					contador++;
					break;
				case 19:
					led_off(LED3_BOAR);
					led_on(LED6_BOAR);
					contador++;
					break;
				case 20:
					led_off(LED6_BOAR);
					led_on(LED7_BOAR);
					contador++;
					break;
				case 21:
					led_off(LED7_BOAR);
					led_on(LED10_BOAR);
					contador++;
					break;
				case 22:
					led_off(LED10_BOAR);
					led_on(LED11_BOAR);
					contador = 0;
					break;
				default:
					break;
				}
				break;

			case 1:
				if (contador > 11) {
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
					led_off(LED7_BOAR);
					led_on(LED11_BOAR);
					led_on(LED10_BOAR);
					break;
				case 1:
					contador++;
					led_off(LED11_BOAR);
					led_on(LED7_BOAR);
					break;
				case 2:
					contador++;
					led_off(LED10_BOAR);
					led_on(LED6_BOAR);
					break;
				case 3:
					contador++;
					led_off(LED7_BOAR);
					led_on(LED3_BOAR);
					break;
				case 4:
					contador++;
					led_off(LED6_BOAR);
					led_on(LED2_BOAR);
					break;
				case 5:
					contador++;
					led_off(LED3_BOAR);
					led_on(LED1_BOAR);
					break;
				case 6:
					contador++;
					led_off(LED2_BOAR);
					led_on(LED0_BOAR);
					break;
				case 7:
					led_off(LED0_BOAR);
					led_on(LED2_BOAR);
					contador++;
					break;
				case 8:
					led_off(LED1_BOAR);
					led_on(LED3_BOAR);
					contador++;
					break;
				case 9:
					led_off(LED2_BOAR);
					led_on(LED6_BOAR);
					contador++;
					break;
				case 10:
					led_off(LED3_BOAR);
					led_on(LED7_BOAR);
					contador++;
					break;
				case 11:
					led_off(LED6_BOAR);
					led_on(LED10_BOAR);
					contador = 0;
					break;
				default:
					break;
				}
				break;
			case 2:
				if (off) {
					off = 0;
					RESET_ALL_LED();
					contador = 0;
				}

				poten = BSP_GetBrightness();

				if (poten < 12) {
					reset = 1;
					led_off(LED0_BOAR);
					led_off(LED1_BOAR);
					led_off(LED2_BOAR);
					led_off(LED3_BOAR);
					led_off(LED6_BOAR);
					led_off(LED7_BOAR);
					led_off(LED10_BOAR);
					led_on(LED11_BOAR);
					led_off(LED_NARANJA);
					led_off(LED_VERDE);
					led_off(LED_ROJO);
					led_on(LED_AZUL);

				}
				if (poten < 24 && poten >= 12) {
					reset = 1;
					led_off(LED0_BOAR);
					led_off(LED1_BOAR);
					led_off(LED2_BOAR);
					led_off(LED3_BOAR);
					led_off(LED6_BOAR);
					led_off(LED7_BOAR);
					led_on(LED10_BOAR);
					led_off(LED11_BOAR);
					led_off(LED_NARANJA);
					led_on(LED_VERDE);
					led_off(LED_ROJO);
					led_off(LED_AZUL);

				}
				if (poten < 36 && poten >= 24) {
					reset = 1;
					led_off(LED0_BOAR);
					led_off(LED1_BOAR);
					led_off(LED2_BOAR);
					led_off(LED3_BOAR);
					led_off(LED6_BOAR);
					led_on(LED7_BOAR);
					led_off(LED10_BOAR);
					led_off(LED11_BOAR);
					led_on(LED_NARANJA);
					led_off(LED_VERDE);
					led_off(LED_ROJO);
					led_off(LED_AZUL);

				}
				if (poten < 48 && poten >= 36) {
					reset = 1;
					led_off(LED0_BOAR);
					led_off(LED1_BOAR);
					led_off(LED2_BOAR);
					led_off(LED3_BOAR);
					led_on(LED6_BOAR);
					led_off(LED7_BOAR);
					led_off(LED10_BOAR);
					led_off(LED11_BOAR);
					led_off(LED_NARANJA);
					led_off(LED_VERDE);
					led_on(LED_ROJO);
					led_off(LED_AZUL);

				}
				if (poten < 60 && poten >= 48) {
					reset = 1;
					led_off(LED0_BOAR);
					led_off(LED1_BOAR);
					led_off(LED2_BOAR);
					led_on(LED3_BOAR);
					led_off(LED6_BOAR);
					led_off(LED7_BOAR);
					led_off(LED10_BOAR);
					led_off(LED11_BOAR);
					led_on(LED_NARANJA);
					led_on(LED_VERDE);
					led_on(LED_ROJO);
					led_on(LED_AZUL);

				}
				if (poten < 72 && poten >= 60) {
					reset = 1;
					led_off(LED0_BOAR);
					led_off(LED1_BOAR);
					led_on(LED2_BOAR);
					led_off(LED3_BOAR);
					led_off(LED6_BOAR);
					led_off(LED7_BOAR);
					led_off(LED10_BOAR);
					led_off(LED11_BOAR);
					led_off(LED_NARANJA);
					led_off(LED_VERDE);
					led_off(LED_ROJO);
					led_off(LED_AZUL);
					if (contador > 3) {
						contador = 0;
					}

					switch (contador) {
					case 0:
						led_on(LED_AZUL);
						contador++;
						break;
					case 1:
						led_on(LED_VERDE);
						contador++;
						break;
					case 2:
						led_on(LED_NARANJA);
						contador++;
						break;
					case 3:
						led_on(LED_ROJO);
						contador = 0;
						break;

					default:
						break;
					}

				}
				if (poten < 84 && poten >= 72) {
					reset = 1;
					led_off(LED0_BOAR);
					led_on(LED1_BOAR);
					led_off(LED2_BOAR);
					led_off(LED3_BOAR);
					led_off(LED6_BOAR);
					led_off(LED7_BOAR);
					led_off(LED10_BOAR);
					led_off(LED11_BOAR);
					led_off(LED_NARANJA);
					led_off(LED_VERDE);
					led_off(LED_ROJO);
					led_off(LED_AZUL);
					if (contador > 3) {
						contador = 0;
					}

					switch (contador) {
					case 0:
						led_on(LED_ROJO);
						contador++;
						break;
					case 1:
						led_on(LED_NARANJA);
						contador++;
						break;
					case 2:
						led_on(LED_VERDE);
						contador++;
						break;
					case 3:
						led_on(LED_AZUL);
						contador = 0;
						break;

					default:
						break;
					}

				}
				if (poten <= 100 && poten >= 84) {
					if (reset) {
						reset = 0;
						RESET_ALL_LED();

					}

					led_on(LED0_BOAR);
					led_off(LED1_BOAR);
					led_off(LED2_BOAR);
					led_off(LED3_BOAR);
					led_off(LED6_BOAR);
					led_off(LED7_BOAR);
					led_off(LED10_BOAR);
					led_off(LED11_BOAR);
					led_toggle(LED_ROJO);
					led_toggle(LED_VERDE);
					led_toggle(LED_NARANJA);
					led_toggle(LED_AZUL);

				}

				break;
			case 3:
				if (off) {
					off = 0;
					RESET_ALL_LED();
					contador = 0;
				}

				poten = BSP_GetBrightness();

				if (poten < 12) {
					reset = 1;
					led_off(LED0_BOAR);
					led_off(LED1_BOAR);
					led_off(LED2_BOAR);
					led_off(LED3_BOAR);
					led_off(LED6_BOAR);
					led_off(LED7_BOAR);
					led_off(LED10_BOAR);
					led_on(LED11_BOAR);
					led_off(LED_NARANJA);
					led_off(LED_VERDE);
					led_off(LED_ROJO);
					led_off(LED_AZUL);
					led_on(LED_RGB_B);
					led_on(LED_RGB_R);
					led_on(LED_RGB_G);

				}
				if (poten < 24 && poten >= 12) {
					reset = 1;
					led_off(LED0_BOAR);
					led_off(LED1_BOAR);
					led_off(LED2_BOAR);
					led_off(LED3_BOAR);
					led_off(LED6_BOAR);
					led_off(LED7_BOAR);
					led_on(LED10_BOAR);
					led_on(LED11_BOAR);
					led_off(LED_NARANJA);
					led_off(LED_VERDE);
					led_off(LED_ROJO);
					led_off(LED_AZUL);
					led_on(LED_RGB_B);
					led_on(LED_RGB_R);
					led_on(LED_RGB_G);

				}
				if (poten < 36 && poten >= 24) {
					reset = 1;
					led_off(LED0_BOAR);
					led_off(LED1_BOAR);
					led_off(LED2_BOAR);
					led_off(LED3_BOAR);
					led_off(LED6_BOAR);
					led_on(LED7_BOAR);
					led_on(LED10_BOAR);
					led_on(LED11_BOAR);
					led_off(LED_NARANJA);
					led_off(LED_VERDE);
					led_off(LED_ROJO);
					led_off(LED_AZUL);
					led_on(LED_RGB_B);
					led_on(LED_RGB_R);
					led_on(LED_RGB_G);

				}
				if (poten < 48 && poten >= 36) {
					reset = 1;
					led_off(LED0_BOAR);
					led_off(LED1_BOAR);
					led_off(LED2_BOAR);
					led_off(LED3_BOAR);
					led_on(LED6_BOAR);
					led_on(LED7_BOAR);
					led_on(LED10_BOAR);
					led_on(LED11_BOAR);
					led_off(LED_NARANJA);
					led_off(LED_VERDE);
					led_off(LED_ROJO);
					led_off(LED_AZUL);
					led_on(LED_RGB_B);
					led_on(LED_RGB_R);
					led_on(LED_RGB_G);

				}
				if (poten < 60 && poten >= 48) {
					reset = 1;
					led_off(LED0_BOAR);
					led_off(LED1_BOAR);
					led_off(LED2_BOAR);
					led_on(LED3_BOAR);
					led_on(LED6_BOAR);
					led_on(LED7_BOAR);
					led_on(LED10_BOAR);
					led_on(LED11_BOAR);
					led_off(LED_NARANJA);
					led_off(LED_VERDE);
					led_off(LED_ROJO);
					led_off(LED_AZUL);
					led_on(LED_RGB_B);
					led_on(LED_RGB_R);
					led_on(LED_RGB_G);

				}
				if (poten < 72 && poten >= 60) {
					reset = 1;
					led_off(LED0_BOAR);
					led_off(LED1_BOAR);
					led_on(LED2_BOAR);
					led_on(LED3_BOAR);
					led_on(LED6_BOAR);
					led_on(LED7_BOAR);
					led_on(LED10_BOAR);
					led_on(LED11_BOAR);
					led_off(LED_NARANJA);
					led_off(LED_VERDE);
					led_off(LED_ROJO);
					led_off(LED_AZUL);
					led_on(LED_RGB_B);
					led_on(LED_RGB_R);
					led_on(LED_RGB_G);

				}
				if (poten < 84 && poten >= 72) {
					reset = 1;
					led_off(LED0_BOAR);
					led_on(LED1_BOAR);
					led_on(LED2_BOAR);
					led_on(LED3_BOAR);
					led_on(LED6_BOAR);
					led_on(LED7_BOAR);
					led_on(LED10_BOAR);
					led_on(LED11_BOAR);
					led_off(LED_NARANJA);
					led_off(LED_VERDE);
					led_off(LED_ROJO);
					led_off(LED_AZUL);
					led_on(LED_RGB_B);
					led_on(LED_RGB_R);
					led_on(LED_RGB_G);

				}
				if (poten < 94 && poten >= 84) {
					reset = 1;
					led_on(LED0_BOAR);
					led_on(LED1_BOAR);
					led_on(LED2_BOAR);
					led_on(LED3_BOAR);
					led_on(LED6_BOAR);
					led_on(LED7_BOAR);
					led_on(LED10_BOAR);
					led_on(LED11_BOAR);
					led_toggle(LED_ROJO);
					led_toggle(LED_VERDE);
					led_toggle(LED_NARANJA);
					led_toggle(LED_AZUL);
					led_on(LED_RGB_B);
					led_on(LED_RGB_R);
					led_on(LED_RGB_G);

				}
				if (poten <= 100 && poten >= 94) {
					if (reset) {
						reset = 0;
						RESET_ALL_LED();
					}

					TOGGEL_ALL_LED();
				}

				break;
			case 4:
				if (contador > 7) {
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
					led_on(LED2_BOAR);
					led_on(LED7_BOAR);
					break;
				case 2:
					contador++;
					led_on(LED1_BOAR);
					led_on(LED10_BOAR);
					break;
				case 3:
					contador++;
					led_on(LED0_BOAR);
					led_on(LED11_BOAR);
					break;
				case 4:
					contador++;
					led_off(LED0_BOAR);
					led_off(LED11_BOAR);
					break;
				case 5:
					contador++;
					led_off(LED1_BOAR);
					led_off(LED10_BOAR);
					break;
				case 6:
					contador++;
					led_off(LED2_BOAR);
					led_off(LED7_BOAR);
					break;
				case 7:
					led_off(LED3_BOAR);
					led_off(LED6_BOAR);
					contador = 0;
					break;
				default:
					break;
				}
				break;
			case 5:
				if (contador > 2) {
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
					led_on(LED_RGB_R);
					led_off(LED_RGB_B);

					break;
				case 1:
					contador++;
					led_on(LED_RGB_B);
					led_off(LED_RGB_G);
					break;
				case 2:
					led_on(LED_RGB_G);
					led_off(LED_RGB_R);
					contador = 0;
					break;
				default:
					break;
				}
				break;

			default:
				break;
			}
		}
		//ContadorTime--;
		//vTaskSuspend(NULL);
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
	led_off(LED_NARANJA);
	led_off(LED_VERDE);
	led_off(LED_ROJO);
	led_off(LED_AZUL);
	led_off(LED0_BOAR);
	led_off(LED1_BOAR);
	led_off(LED2_BOAR);
	led_off(LED3_BOAR);
	led_off(LED6_BOAR);
	led_off(LED7_BOAR);
	led_off(LED10_BOAR);
	led_off(LED11_BOAR);
	led_on(LED_RGB_B);
	led_on(LED_RGB_R);
	led_on(LED_RGB_G);
}
void TOGGEL_ALL_LED(void) {
	led_toggle(LED0_BOAR);
	led_toggle(LED1_BOAR);
	led_toggle(LED2_BOAR);
	led_toggle(LED3_BOAR);
	led_toggle(LED6_BOAR);
	led_toggle(LED7_BOAR);
	led_toggle(LED10_BOAR);
	led_toggle(LED11_BOAR);
	led_toggle(LED_ROJO);
	led_toggle(LED_VERDE);
	led_toggle(LED_NARANJA);
	led_toggle(LED_AZUL);
	led_toggle(LED_RGB_B);
	led_toggle(LED_RGB_R);
	led_toggle(LED_RGB_G);

}

void SET_ALL_LED(void) {
	led_on(LED0_BOAR);
	led_on(LED1_BOAR);
	led_on(LED2_BOAR);
	led_on(LED3_BOAR);
	led_on(LED6_BOAR);
	led_on(LED7_BOAR);
	led_on(LED10_BOAR);
	led_on(LED11_BOAR);
	led_on(LED_NARANJA);
	led_on(LED_VERDE);
	led_on(LED_ROJO);
	led_on(LED_AZUL);

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

