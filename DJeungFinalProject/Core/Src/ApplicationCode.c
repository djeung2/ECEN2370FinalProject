/*
 * ApplicationCode.c
 *
 *  Created on: Dec 30, 2023 (updated 11/12/2024) Thanks Donavon! 
 *      Author: Xavion
 */

#include "ApplicationCode.h"

/* Static variables */


extern void initialise_monitor_handles(void); 

#if COMPILE_TOUCH_FUNCTIONS == 1
static STMPE811_TouchData StaticTouchData;
#if TOUCH_INTERRUPT_ENABLED == 1
static EXTI_HandleTypeDef LCDTouchIRQ;
void LCDTouchScreenInterruptGPIOInit(void);
#endif // TOUCH_INTERRUPT_ENABLED
#endif // COMPILE_TOUCH_FUNCTIONS

void ApplicationInit(void)
{
	initialise_monitor_handles(); // Allows printf functionality
    LTCD__Init();
    LTCD_Layer_Init(0);
    LCD_Clear(0,LCD_COLOR_WHITE);

    #if COMPILE_TOUCH_FUNCTIONS == 1
	InitializeLCDTouch();

	// This is the orientation for the board to be direclty up where the buttons are vertically above the screen
	// Top left would be low x value, high y value. Bottom right would be low x value, low y value.
	StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;

	#if TOUCH_INTERRUPT_ENABLED == 1
	LCDTouchScreenInterruptGPIOInit();
	#endif // TOUCH_INTERRUPT_ENABLED

	#endif // COMPILE_TOUCH_FUNCTIONS
}

void LCD_Visual_Demo(void)
{
	visualDemo();
}

void gameGrid_reset(void)
{
	LCD_Clear(0,LCD_COLOR_WHITE);
	for (uint8_t row = 0; row < 3; row++)
		for (uint8_t col = 0; col<4; col++)
			gameGrid[row][col] = 0;
}

bool check_grid_empty()
{
	for (uint8_t row = 0; row < 4; row++)
		for (uint8_t col = 0; col<3; col++)
			if (gameGrid[col][row] != 0)
				return 0;
	touchNum = 0;
	return 1;
}

void homescreen()
{
	LCD_Clear(0,LCD_COLOR_WHITE);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font16x24);

	LCD_DrawMonkey(120, 160);

	LCD_DisplayChar(90, 50, 'C');
	LCD_DisplayChar(105, 50, 'H');
	LCD_DisplayChar(120, 50, 'I');
	LCD_DisplayChar(135, 50, 'M');
	LCD_DisplayChar(150, 50, 'P');

	LCD_DisplayChar(100, 70, 'T');
	LCD_DisplayChar(115, 70, 'E');
	LCD_DisplayChar(130, 70, 'S');
	LCD_DisplayChar(145, 70, 'T');

	LCD_DisplayChar(90, 200, 'P');
	LCD_DisplayChar(105, 200, 'R');
	LCD_DisplayChar(120, 200, 'E');
	LCD_DisplayChar(135, 200, 'S');
	LCD_DisplayChar(150, 200, 'S');

	LCD_DisplayChar(110, 220, 'T');
	LCD_DisplayChar(125, 220, 'O');

	LCD_DisplayChar(90, 240, 'S');
	LCD_DisplayChar(105, 240, 'T');
	LCD_DisplayChar(120, 240, 'A');
	LCD_DisplayChar(135, 240, 'R');
	LCD_DisplayChar(150, 240, 'T');

	while (startGame_flag == 0);

}

void endscreen()
{
	LCD_Clear(0,LCD_COLOR_WHITE);
	LCD_DrawMonkey(120, 140);

	LCD_DisplayChar(80,80,'U');
	LCD_DisplayChar(95,80,'H');

	LCD_DisplayChar(125,80,'O');
	LCD_DisplayChar(140,80,'H');

	LCD_DisplayChar(60,200,'Y');
	LCD_DisplayChar(75,200,'O');
	LCD_DisplayChar(90,200,'U');

	LCD_DisplayChar(120,200,'L');
	LCD_DisplayChar(135,200,'O');
	LCD_DisplayChar(150,200,'S');
	LCD_DisplayChar(165,200,'T');

}

void random_block(uint16_t num, RNG_HandleTypeDef hrng)
{

	hrng.Instance = RNG;
	if (HAL_RNG_Init(&hrng) != HAL_OK)
	{
		Error_Handler();
	}

	num--;

	do
	{
		randomNumbery[num] = HAL_RNG_GetRandomNumber(&hrng)%4;
		randomNumberx[num] = HAL_RNG_GetRandomNumber(&hrng)%3;
	} while (gameGrid[randomNumberx[num]][randomNumbery[num]] != 0);

	gameGrid[randomNumberx[num]][randomNumbery[num]] = num + 1;

	generate_block(randomNumberx[num], randomNumbery[num], num + 1);



}

void hide_numbers()
{
	for (uint16_t x = 0; x<3; x++)
		for (uint16_t y = 0; y<4; y++)
			if (gameGrid[x][y] != 0)
				LCD_Draw_Square_Fill(((x * 80)+ 2), ((y * 80) + 2), 76, LCD_COLOR_BLUE);
}
void generate_block	(uint16_t x, uint16_t y, uint16_t num)
{
	LCD_Draw_Square_Fill((x * 80), (y * 80), 80, LCD_COLOR_BLACK);
	LCD_Draw_Square_Fill(((x * 80)+ 2), ((y * 80) + 2), 76, LCD_COLOR_BLUE);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font16x24);
	if (num > 9)
	{

		LCD_DisplayChar((x * 80) + 25, (y * 80) + 30, blockNums[num / 10] );
		LCD_DisplayChar((x * 80) + 35, (y * 80) + 30, blockNums[num % 10] );
	}
	else
	{
		LCD_DisplayChar((x * 80) + 30, (y * 80) + 30, blockNums[num] );
	}
}


void remove_block	(uint16_t x, uint16_t y)
{
	LCD_Draw_Square_Fill((x * 80), (y * 80), 80, LCD_COLOR_WHITE);
}


#if COMPILE_TOUCH_FUNCTIONS == 1
void LCD_Touch_Polling_Demo(void)
{
	LCD_Clear(0,LCD_COLOR_GREEN);
	while (1) {
		/* If touch pressed */
		if (returnTouchStateAndLocation(&StaticTouchData) == STMPE811_State_Pressed) {
			/* Touch valid */
			printf("\nX: %03d\nY: %03d\n", StaticTouchData.x, StaticTouchData.y);
			LCD_Clear(0, LCD_COLOR_RED);
		} else {
			/* Touch not pressed */
			printf("Not Pressed\n\n");
			LCD_Clear(0, LCD_COLOR_GREEN);
		}
	}
}


// TouchScreen Interrupt
#if TOUCH_INTERRUPT_ENABLED == 1

void LCDTouchScreenInterruptGPIOInit(void)
{
	GPIO_InitTypeDef LCDConfig = {0};
    LCDConfig.Pin = GPIO_PIN_15;
    LCDConfig.Mode = GPIO_MODE_IT_RISING_FALLING;
    LCDConfig.Pull = GPIO_NOPULL;
    LCDConfig.Speed = GPIO_SPEED_FREQ_HIGH;
    
    // Clock enable
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // GPIO Init      
    HAL_GPIO_Init(GPIOA, &LCDConfig);

    // Interrupt Configuration
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	LCDTouchIRQ.Line = EXTI_LINE_15;

}

void makeLevel(uint16_t level, RNG_HandleTypeDef hrng)
{
	//gameGrid_reset();

	for (int i = level; i>0; i--)
		random_block((i), hrng);

	HAL_Delay(1000);
	hide_numbers();


	while(check_grid_empty() == 0);
}

#define TOUCH_DETECTED_IRQ_STATUS_BIT   (1 << 0)  // Touchscreen detected bitmask

static uint8_t statusFlag;

void EXTI15_10_IRQHandler()
{
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); // May consider making this a universial interrupt guard
	bool isTouchDetected = false;

	static uint32_t count;
	count = 0;
	while(count == 0){
		count = STMPE811_Read(STMPE811_FIFO_SIZE);
	}

	// Disable touch interrupt bit on the STMPE811
	uint8_t currentIRQEnables = ReadRegisterFromTouchModule(STMPE811_INT_EN);
	WriteDataToTouchModule(STMPE811_INT_EN, 0x00);

	// Clear the interrupt bit in the STMPE811
	statusFlag = ReadRegisterFromTouchModule(STMPE811_INT_STA);
	uint8_t clearIRQData = (statusFlag | TOUCH_DETECTED_IRQ_STATUS_BIT); // Write one to clear bit
	WriteDataToTouchModule(STMPE811_INT_STA, clearIRQData);
	
	uint8_t ctrlReg = ReadRegisterFromTouchModule(STMPE811_TSC_CTRL);
	if (ctrlReg & 0x80)
	{
		isTouchDetected = true;
	}

	// Determine if it is pressed or unpressed
	if(isTouchDetected) // Touch has been detected
	{
		printf("\nPressed");
		// May need to do numerous retries? 
		DetermineTouchPosition(&StaticTouchData);
		/* Touch valid */
		printf("\nX: %03d\nY: %03d \n", StaticTouchData.x, StaticTouchData.y);
		//LCD_Clear(0, LCD_COLOR_RED);

	}else{


		if (startGame_flag)
		{
			correctCenterx = randomNumberx[touchNum] * 80 + 40;
			correctCentery = randomNumbery[touchNum] * 80 + 40;

			DetermineTouchPosition(&StaticTouchData);
			if (((StaticTouchData.x - correctCenterx) > -40) && ((StaticTouchData.x - correctCenterx) < 40) && (((320 - StaticTouchData.y) - correctCentery) > -40) && (((320 - StaticTouchData.y) - correctCentery) < 40))
			{
				remove_block(randomNumberx[touchNum], randomNumbery[touchNum]);
				gameGrid[randomNumberx[touchNum]][randomNumbery[touchNum]] = 0;
				touchNum++;

				// wait a little bit for debounce


			}else
			{
				endscreen();
				while (1);
			}
		}else
		{
			startGame_flag = 1;
		}



		//if	(((abs(StaticTouchData.x - correctCenterx)) < 20) && ((abs(StaticTouchData.y - correctCentery)) < 20))
			//remove_block(1, 1);
		//LCD_Clear(0, LCD_COLOR_GREEN);
	}


	STMPE811_Write(STMPE811_FIFO_STA, 0x01);
	STMPE811_Write(STMPE811_FIFO_STA, 0x00);

	// Re-enable IRQs
    WriteDataToTouchModule(STMPE811_INT_EN, currentIRQEnables);
	HAL_EXTI_ClearPending(&LCDTouchIRQ, EXTI_TRIGGER_RISING_FALLING);

	HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	//Potential ERRATA? Clearing IRQ bit again due to an IRQ being triggered DURING the handling of this IRQ..
	WriteDataToTouchModule(STMPE811_INT_STA, clearIRQData);

}
#endif // TOUCH_INTERRUPT_ENABLED
#endif // COMPILE_TOUCH_FUNCTIONS

