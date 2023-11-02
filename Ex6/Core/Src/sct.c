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

static const uint32_t reg_values[4][10] = {
		{
				//PCDE--------GFAB @ DIS1
				0b0111000000000111 << 16,
				0b0100000000000001 << 16,
				0b0011000000001011 << 16,
				0b0110000000001011 << 16,
				0b0100000000001101 << 16,
				0b0110000000001110 << 16,
				0b0111000000001110 << 16,
				0b0100000000000011 << 16,
				0b0111000000001111 << 16,
				0b0110000000001111 << 16,
		},
		{
				//----PCDEGFAB---- @ DIS2
				0b0000011101110000 << 0,
				0b0000010000010000 << 0,
				0b0000001110110000 << 0,
				0b0000011010110000 << 0,
				0b0000010011010000 << 0,
				0b0000011011100000 << 0,
				0b0000011111100000 << 0,
				0b0000010000110000 << 0,
				0b0000011111110000 << 0,
				0b0000011011110000 << 0,
		},
		{
				//----PCDEGFAB---- @ DIS2_DP
				0b0000111101110000 << 0,
				0b0000110000010000 << 0,
				0b0000101110110000 << 0,
				0b0000111010110000 << 0,
				0b0000110011010000 << 0,
				0b0000111011100000 << 0,
				0b0000111111100000 << 0,
				0b0000110000110000 << 0,
				0b0000111111110000 << 0,
				0b0000111011110000 << 0,
		},
		{
				//PCDE--------GFAB @ DIS3
				0b0111000000000111 << 0,
				0b0100000000000001 << 0,
				0b0011000000001011 << 0,
				0b0110000000001011 << 0,
				0b0100000000001101 << 0,
				0b0110000000001110 << 0,
				0b0111000000001110 << 0,
				0b0100000000000011 << 0,
				0b0111000000001111 << 0,
				0b0110000000001111 << 0,
		},
};


void sct_init(void) {
	sct_led(0);
}

void sct_led(uint32_t value) {
	// Enable the output
	HAL_GPIO_WritePin(SCT_NOE_GPIO_Port, SCT_NOE_Pin, 0);		// Enable the output (output enabled when set low)
	// Shift the values
	for (uint8_t i = 0; i < 32; i++) {
		HAL_GPIO_WritePin(SCT_SDI_GPIO_Port, SCT_SDI_Pin, value & 1); 	// Bit mask for the shift register 0x00000 & 0x00001
		value >>= 1;													// Shift bits from value right
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, 1);			// Set CLK high so the bit can be shifted into the register (data is shifted at the rising edge)
		HAL_GPIO_WritePin(SCT_CLK_GPIO_Port, SCT_CLK_Pin, 0);			// Set CLK low (to it's default value)
	}
	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, 1);		// Set the Latch HIGH so the data from shift register goes through
	HAL_GPIO_WritePin(SCT_NLA_GPIO_Port, SCT_NLA_Pin, 0);		// Set the Latch LOW (to it's default state)

}

void sct_value(uint16_t value) {
	uint32_t reg = 0;		// Initialize a variable to configure display values
	/*
	reg |= reg_values[0][value / 100 % 10];		// Hundreds
	reg |= reg_values[1][value / 10 % 10];		// Tens
	reg |= reg_values[2][value / 1 % 10];		// Ones
	*/

	reg |= reg_values[0][value / 100];			// Hundreds
	// reg |= reg_values[1][value / 10 % 10];	// Tens
	reg |= reg_values[2][value / 10 % 10];		// Tens /w DP
	reg |= reg_values[3][value % 10];			// Ones

	sct_led(reg);
}

