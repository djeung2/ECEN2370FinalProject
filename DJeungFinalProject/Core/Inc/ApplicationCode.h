/*
 * ApplicationCode.h
 *
 *  Created on: Dec 30, 2023
 *      Author: Xavion
 */

#include "LCD_Driver.h"
#include "stm32f4xx_hal.h"


#include <stdio.h>
#include <stdbool.h>


#ifndef INC_APPLICATIONCODE_H_
#define INC_APPLICATIONCODE_H_

static char blockNums[10] = {'0','1', '2', '3','4','5','6','7','8','9'};
static uint16_t correctCenterx = 0;
static uint16_t correctCentery = 0;
static uint16_t	touchNum = 0;
static uint16_t gameGrid[3][4] = {0};
static uint32_t randomNumberx[12] = {0};
static uint32_t randomNumbery[12] = {0};



void ApplicationInit(void);
void LCD_Visual_Demo(void);

void generate_block	(uint16_t, uint16_t, uint16_t);
void remove_block	(uint16_t, uint16_t);
void hide_numbers();
void gameGrid_reset(void);
bool check_grid_empty();
void endscreen();
void homescreen();

#define abs(x) (x>0) ? (x) : (-x)


#if (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)
void LCD_Touch_Polling_Demo(void);
#endif // (COMPILE_TOUCH_FUNCTIONS == 1) && (COMPILE_TOUCH_INTERRUPT_SUPPORT == 0)

#endif /* INC_APPLICATIONCODE_H_ */
