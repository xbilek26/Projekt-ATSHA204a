/*
 * sha204_timer.c
 *
 * Created: 4/16/2024 16:25:31
 *  Author: Student
 */ 

#include "sha204_timer.h"
#include <stdio.h>

void delay_10us(uint8_t delay)
{
	volatile uint8_t delay_10us;

	for (; delay > 0; delay--) {
		for (delay_10us = TIME_UTILS_LOOP_COUNT; delay_10us > 0; delay_10us--);
		TIME_UTILS_US_CALIBRATION;
	}
}

void delay_ms(uint8_t delay)
{
	uint8_t i;
	for (i = delay; i > 0; i--)
	delay_10us(TIME_UTILS_MS_CALIBRATION);
}