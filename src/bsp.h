/*
 /*
 * bsp.h
 *
 *  Created on: 11 de ago. de 2016
 *      Author: LCSR-AF
 */

#ifndef BSP_BSP_H_
#define BSP_BSP_H_

#include "stdint.h"

#define LEDS_PORT GPIOD

//enum{
//	LED_VERDE = 0,
//	LED_NARANJA,
//	LED_ROJO,
//	LED_AZUL
//}leds;


typedef enum{
	SW_RIGHT=0, SW_LEFT=1, SW_UP=2, SW_DOWN=3, SW_DISC=4
}SW_typedef;


typedef enum{LED_VERDE = 0,LED_NARANJA=1,LED_ROJO=2,LED_AZUL=3,LED0_BOAR=4, LED1_BOAR=5, LED2_BOAR=6, LED3_BOAR=7, LED6_BOAR=8,LED7_BOAR=9,LED10_BOAR=10,LED11_BOAR=11,LED_RGB_R=12,LED_RGB_G=13,LED_RGB_B=14
}LED_typedef;


#define EXP_BOARD_POT_PIN				   GPIO_PIN_2
#define EXP_BOARD_POT_PORT                   GPIOC
#define EXP_BOARD_POT_PIN_CLK_ENABLE()           __GPIOC_CLK_ENABLE()
#define EXP_BOARD_POT_ADC_CLK_ENABLE()		__ADC1_CLK_ENABLE()
#define EXP_BOARD_POT_CLK_DISABLE()          __GPIOD_CLK_DISABLE()
#define EXP_BOARD_POT_CHANNEL				   ADC_CHANNEL_12


void bsp_init(void);

uint8_t get_sw_state(SW_typedef sw);

void led_on(uint8_t led);
void led_off(uint8_t led);
void led_toggle(uint8_t led);


void BSP_Init(void);

uint32_t Get_SW_State(void);

void led_setBright(uint8_t led, uint8_t value);

void BSP_ADC_Init(void);
uint8_t BSP_GetBrightness(void);

#endif /* BSP_BSP_H_ */
