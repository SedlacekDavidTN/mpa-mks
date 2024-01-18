/*
 * sct.c
 *
 *  Created on: Oct 31, 2023
 *      Author: xsedla1n
 */

// Includes
#include "stm32f0xx.h"
#include "main.h"
#include "sct.h"

static const uint32_t reg_values[1][10] = {
		{
				//PCDE--------GFAB @ DIS1
				0b0010000000000000 << 16, // Bottom
				0b0001000000000000 << 16, // Left Bottom
				0b0000000000000100 << 16, // Left Top
				0b0000000000000010 << 16, // Top
				//----PCDEGFAB---- @ DIS2
				0b0000000000100000 << 0, // Top
				//PCDE--------GFAB @ DIS3
				0b0000000000000010 << 0, // Top
				0b0000000000000001 << 0, // Right Top
				0b0100000000000000 << 0, // Right Bottom
				0b0010000000000000 << 0, // Bottom
				//----PCDEGFAB---- @ DIS2
				0b0000001000000000 << 0, // Bottom
		},
};


void sct_init(void) {
	sct_led(0);
}

void sct_led(uint32_t value) {
	// Enable the output
	HAL_GPIO_WritePin(SCT_NOE_GPIO_Port, SCT_NOE_Pin, 0); // Enable the output (output enabled when set low)

	// Shift the values
	for (uint8_t i = 0; i < 32; i++) {
		HAL_GPIO_WritePin(SCT_SDI_GPIO_Port, SCT_SDI_Pin, value & 1); // Bit mask for the shift register 0x00000 & 0x00001
		value >>= 1; // Shift bits from value right
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, 1); // Set CLK high so the bit can be shifted into the register (data is shifted at the rising edge)
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, 0); // Set CLK low (to it's default value)
	}

	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, 1); // Set the Latch HIGH so the data from shift register goes through
	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, 0); // Set the Latch LOW (to it's default state)

}

void sct_value(uint16_t value) {
	uint32_t reg = 0; // Initialize a variable to configure display values

	reg |= reg_values[0][value];

	sct_led(reg); // Send values to the display function
}

