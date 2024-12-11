/*
 * timer_driver.c
 *
 *  Created on: Dec 10, 2024
 *      Author: djeun
 */

/*
//TIM5
void LED_TIM5INIT() //NOTE: the TIM5 ARR must be confitured using the TIM%_ARV_Config function after TIM5 initialization
{
	GPTimer_Config_t TIMConfig;

	TIMConfig.ARV = timer_arv_max;
	TIMConfig.MMS = MMS_Default;
	TIMConfig.CDS = CDS_Default;
	TIMConfig.Prescalar = no_prescalar;
	TIMConfig.CAM = Edge_aligned;
	TIMConfig.ARR_buffer = DISABLE;
	TIMConfig.TIM_COUNTDOWN_ENABLE = DISABLE;
	TIMConfig.INTERRUPT_ENR = ENABLE;
	TIMConfig.DISABLE_UPDATE = DISABLE;
	TIMConfig.PULSEMODE_ENR = DISABLE;

	timer_init(TIM5, & TIMConfig);


}
void LED_TIM5START()
{
	timer_start(TIM5);
}
void LED_TIM5STOP()
{
	timer_stop(TIM5);
}
void LED_TIM5RESET()
{
	timer_reset(TIM5);
}
uint32_t TIM5_ARV()
{
	return (TIM5 -> ARR);
}
uint32_t TIM5_COUNT()
{
	return (TIM5 -> CNT);
}
*/
