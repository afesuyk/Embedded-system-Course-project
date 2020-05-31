#ifndef DS3231_H_
#define DS3231_H_

#include "stdint.h"
#include "stm32f4xx_hal.h"

#define DS3231_ADDRESS 0xD0

typedef struct {
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t dayofweek;
	uint8_t dayofmonth;
	uint8_t month;
	uint8_t year;
} TIME;

TIME time;

uint8_t decToBcd(int val);
int bcdToDec(uint8_t val);
void setTime (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year, I2C_HandleTypeDef *hi2c3);
void getTime (I2C_HandleTypeDef *hi2c3);

#endif /* DS3231_H_ */
