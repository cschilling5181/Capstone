/*
 * keypad.c
 *
 *  Created on: Dec. 2, 2019
 *      Author: Christoph
 */

#include "keypad.h"
#include "stm32f3xx_hal.h"
#include "main.h"

int keypadRead()
{
	uint8_t state = 16;

	do
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
		if(!HAL_GPIO_ReadPin(GPIOE, C0_In_Pin))
		{
			state = 0;
		}
		if(!HAL_GPIO_ReadPin(GPIOE, C1_In_Pin))
		{
			state = 1;
		}
		if(!HAL_GPIO_ReadPin(GPIOE, C2_In_Pin))
		{
			state = 2;
		}
		if(!HAL_GPIO_ReadPin(GPIOE, C3_In_Pin))
		{
			state = 3;
		}
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);

		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
		if(!HAL_GPIO_ReadPin(GPIOE, C0_In_Pin))
		{
			state = 4;
		}
		if(!HAL_GPIO_ReadPin(GPIOE, C1_In_Pin))
		{
			state = 5;
		}
		if(!HAL_GPIO_ReadPin(GPIOE, C2_In_Pin))
		{
			state = 6;
		}
		if(!HAL_GPIO_ReadPin(GPIOE, C3_In_Pin))
		{
			state = 7;
		}
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);

		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
		if(!HAL_GPIO_ReadPin(GPIOE, C0_In_Pin))
		{
			state = 8;
		}
		if(!HAL_GPIO_ReadPin(GPIOE, C1_In_Pin))
		{
			state = 9;
		}
		if(!HAL_GPIO_ReadPin(GPIOE, C2_In_Pin))
		{
			state = 10;
		}
		if(!HAL_GPIO_ReadPin(GPIOE, C3_In_Pin))
		{
			state = 11;
		}
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);

		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
		if(!HAL_GPIO_ReadPin(GPIOE, C0_In_Pin))
		{
			state = 12;
		}
		if(!HAL_GPIO_ReadPin(GPIOE, C1_In_Pin))
		{
			state = 13;
		}
		if(!HAL_GPIO_ReadPin(GPIOE, C2_In_Pin))
		{
			state = 14;
		}
		if(!HAL_GPIO_ReadPin(GPIOE, C3_In_Pin))
		{
			state = 15;
		}
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
	}while(state == 16);

	return state;
}
