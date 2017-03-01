/*
 * bsp.c
 *
 *  Created on: 11 de ago. de 2016
 *      Author: LCSR-AF
 */
#include "bsp.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

extern void APP_1ms(void);

void bsp_sw_init(void);

TIM_HandleTypeDef TIM2_Handle;
TIM_HandleTypeDef TIM4_Handle;
ADC_HandleTypeDef ADC_HandleStruct;
UART_HandleTypeDef UART3_Handle;
uint8_t rxBuffer[50];

uint32_t* const leds_pwm[] = { &TIM4->CCR1, &TIM4->CCR3, &TIM4->CCR2,
		&TIM4->CCR4 };

uint16_t SW_PIN[] =
		{ GPIO_PIN_2, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_6, GPIO_PIN_0 };

GPIO_TypeDef* SW_PORT[] = { GPIOE, GPIOE, GPIOE, GPIOE, GPIOA };

uint8_t get_sw_state(SW_typedef sw) {
	return HAL_GPIO_ReadPin(SW_PORT[sw], SW_PIN[sw]);
	//return GPIO_ReadInputDataBit(SW_PORT[sw], SW_PIN[sw]);

}

void bsp_sw_init(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	__GPIOA_CLK_ENABLE()
	;

	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	//GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	__GPIOE_CLK_ENABLE()
	;

	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	//GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

void BSP_UART_Init(void) {

	bsp_sw_init();

	GPIO_InitTypeDef GPIO_InitStruct;

	__GPIOD_CLK_ENABLE()
	;

	__HAL_RCC_USART3_CLK_ENABLE()
	;

	GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	UART3_Handle.Instance = USART3;
	UART3_Handle.Init.BaudRate = 1200;
	UART3_Handle.Init.WordLength = UART_WORDLENGTH_8B;
	UART3_Handle.Init.StopBits = UART_STOPBITS_1;
	UART3_Handle.Init.Parity = UART_PARITY_NONE;
	UART3_Handle.Init.Mode = UART_MODE_TX_RX;
	UART3_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	UART3_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&UART3_Handle);

	HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

	HAL_UART_Receive_IT(&UART3_Handle, rxBuffer, 50);
}

void BSP_Init(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	__PWR_CLK_ENABLE()
	;

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
			| RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

	__GPIOD_CLK_ENABLE()
	;

	GPIO_InitTypeDef GPIO_Init;

	GPIO_Init.Mode = GPIO_MODE_AF_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Alternate = GPIO_AF2_TIM4;
	GPIO_Init.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(LEDS_PORT, &GPIO_Init);

	__TIM2_CLK_ENABLE()
	;

	TIM2_Handle.Instance = TIM2;
	TIM2_Handle.Init.Period = 1000 - 1;
	TIM2_Handle.Init.Prescaler = 84 - 1;
	TIM2_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM2_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;

	HAL_TIM_Base_Init(&TIM2_Handle);
	HAL_TIM_Base_Start_IT(&TIM2_Handle);

	HAL_NVIC_SetPriority(TIM2_IRQn, 10, 1);
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	__GPIOA_CLK_ENABLE()
	;

	GPIO_Init.Mode = GPIO_MODE_INPUT;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FAST;
	GPIO_Init.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOA, &GPIO_Init);

	__TIM3_CLK_ENABLE()
	;

	TIM_MasterConfigTypeDef TIM_MasterConfig;
	TIM_OC_InitTypeDef TIM_OC_Init;

	__TIM4_CLK_ENABLE()
	;

	TIM4_Handle.Instance = TIM4;
	TIM4_Handle.Init.Prescaler = 84 - 1;
	;
	TIM4_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM4_Handle.Init.Period = 1500;
	TIM4_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV2;
	HAL_TIM_PWM_Init(&TIM4_Handle);

	TIM_MasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	TIM_MasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM4_Handle, &TIM_MasterConfig);

	TIM_OC_Init.OCMode = TIM_OCMODE_PWM2;
	TIM_OC_Init.Pulse = 0;
	TIM_OC_Init.OCPolarity = TIM_OCPOLARITY_LOW;
	TIM_OC_Init.OCFastMode = TIM_OCFAST_ENABLE;

	HAL_TIM_PWM_ConfigChannel(&TIM4_Handle, &TIM_OC_Init, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&TIM4_Handle, &TIM_OC_Init, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&TIM4_Handle, &TIM_OC_Init, TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&TIM4_Handle, &TIM_OC_Init, TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(&TIM4_Handle, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&TIM4_Handle, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&TIM4_Handle, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&TIM4_Handle, TIM_CHANNEL_4);

	BSP_ADC_Init();
	BSP_UART_Init();
	BSP_LED();
}

uint16_t LED_PIN[] = { GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15,
		GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3,
		GPIO_PIN_6, GPIO_PIN_7, GPIO_PIN_10, GPIO_PIN_11};

GPIO_TypeDef* LED_PORT[] = { GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD, GPIOD,
		GPIOD, GPIOD, GPIOD, GPIOD,
		GPIOD };

void BSP_LED() {
	GPIO_InitTypeDef GPIO_InitStruct;
	__GPIOD_CLK_ENABLE()
	;

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
			| GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12
			| GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	//GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

void led_on(LED_typedef Led) {
	HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);
}
void led_off(LED_typedef Led) {
	HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);
}

void BSP_ADC_Init(void) {

	ADC_ChannelConfTypeDef ChannelConfStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	EXP_BOARD_POT_PIN_CLK_ENABLE()
	;
	EXP_BOARD_POT_ADC_CLK_ENABLE()
	;

	ADC_HandleStruct.Instance = ADC1;

	ADC_HandleStruct.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	ADC_HandleStruct.Init.Resolution = ADC_RESOLUTION_12B;
	ADC_HandleStruct.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC_HandleStruct.Init.ScanConvMode = DISABLE;
	ADC_HandleStruct.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	ADC_HandleStruct.Init.ContinuousConvMode = DISABLE;
	ADC_HandleStruct.Init.NbrOfConversion = 1;
	ADC_HandleStruct.Init.DiscontinuousConvMode = DISABLE;
	ADC_HandleStruct.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	ADC_HandleStruct.Init.DMAContinuousRequests = DISABLE;
	ADC_HandleStruct.Init.NbrOfDiscConversion = 0;
	ADC_HandleStruct.Init.ExternalTrigConvEdge =
	ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADC_HandleStruct.Init.EOCSelection = DISABLE;

	GPIO_InitStruct.Pin = EXP_BOARD_POT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(EXP_BOARD_POT_PORT, &GPIO_InitStruct);

	ChannelConfStruct.Channel = EXP_BOARD_POT_CHANNEL;
	ChannelConfStruct.Offset = 0;
	ChannelConfStruct.Rank = 1;
	ChannelConfStruct.SamplingTime = ADC_SAMPLETIME_15CYCLES;

	HAL_ADC_Init(&ADC_HandleStruct);
	HAL_ADC_ConfigChannel(&ADC_HandleStruct, &ChannelConfStruct);
	HAL_ADC_Start(&ADC_HandleStruct);

}

void TIM2_IRQHandler(void) {

	__HAL_TIM_CLEAR_FLAG(&TIM2_Handle, TIM_FLAG_UPDATE);
	APP_1ms();

}

void USART3_IRQHandler(void) {
	HAL_UART_IRQHandler(&UART3_Handle);
	HAL_UART_Receive_IT(&UART3_Handle, rxBuffer, 50);

}

void TransmitData(uint8_t *buffer, uint8_t size) {
	HAL_UART_Transmit(&UART3_Handle, buffer, size, 100);

}

