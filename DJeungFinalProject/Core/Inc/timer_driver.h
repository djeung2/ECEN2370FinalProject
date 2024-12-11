/*
 * timer_driver.h
 *
 *  Created on: Dec 10, 2024
 *      Author: djeun
 */
/*
#ifndef INC_TIMER_DRIVER_H_
#define INC_TIMER_DRIVER_H_

#define time_on			5
#define clk_freq		16000000
#define timer_arv_max	0xffffffff
#define psc_max			0xffff

#define MMS_Default		0
#define CDS_Default		0
#define no_prescalar	0
#define Edge_aligned	0

#define MMS_reset 		000
#define MMS_ENABLE		001
#define MMS_UPDATE		010
#define MMS_COMP_PULSE	011
#define MMS_CMP_OC1REF	100
#define MMS_CMP_OC2REF	101
#define MMS_CMP_OC3REF	110
#define MMS_CMP_OC4REF	111

//bit shifts
#define CR1_CKD_shift 8
#define CR1_CMS_shift 5
#define CR1_DIR_shift 4
#define CR1_ARPE_shift 7
#define CR1_OPM_shift	3
#define CR1_UDIS_shift 	1
#define CR1_UDS_shift	2

//clock division
#define CKS

typedef struct
{
	uint32_t ARV;	//autoreload in the ARR
	uint32_t MMS;	//MasterMode selection in CR2
	uint32_t CDS;	//clock division in CR1
	uint16_t Prescalar;	// in PSC
	uint8_t CAM;	//center aligned mode selection in CR1
	bool ARR_buffer;
	bool		TIM_COUNTDOWN_ENABLE;
	bool		INTERRUPT_ENR;
	bool		DISABLE_UPDATE;
	bool		PULSEMODE_ENR;
	bool		UIE;



}GPTimer_Config_t;


void LED_TIM5INIT();
void LED_TIM5START();
void LED_TIM5STOP();
void LED_TIM5RESET();
uint32_t TIM5_ARV();
uint32_t TIM5_COUNT();
#endif /* INC_TIMER_DRIVER_H_ */

