/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Mahmoud Badr
 *	   	   File:  RCC_interface.h
 *		  Layer:  MCAL
 *       Module:  RCC
 *		Version:  1.00
 *	
 *  Description:  -     
 *  
 *********************************************************************************************************************/
#ifndef RCC_INTERFACE_H
#define RCC_INTERFACE_H

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef enum
{
	RCC_WTD1=0,
	RCC_WTD2=1,
	RCC_GPTM0=8,
	RCC_GPTM1=9,
	RCC_GPTM2=10,
	RCC_GPTM3=11,
	RCC_GPTM4=12,
	RCC_GPTM5=13,
	RCC_GPIOA=16,
	RCC_GPIOB=17,
	RCC_GPIOC=18,
	RCC_GPIOD=19,
	RCC_GPIOE=20,
	RCC_GPIOF=21,
	RCC_UDMA=24,
	RCC_HIB=40,
	RCC_UART0=48,
	RCC_UART1=49,
	RCC_UART2=50,
	RCC_UART3=51,
	RCC_UART4=52,
	RCC_UART5=53,
	RCC_UART6=54,
	RCC_UART7=55,
	RCC_SSI0=56,
	RCC_SSI1=57,
	RCC_SSI2=58,
	RCC_SSI3=59,
	RCC_I2C0=64,
	RCC_I2C1=65,
	RCC_I2C2=66,
	RCC_I2C3=67,
	RCC_USB=80,
	RCC_CAN0=104,
	RCC_CAN1=105,
	RCC_ADC0=112,
	RCC_ADC1=113,
	RCC_ACMP=120,
	RCC_PWM0=128,
	RCC_PWM1=129,
	RCC_QEI0=136,
	RCC_QEI1=137,
	RCC_EEPROM=176,
	RCC_wGPTM0=184,
	RCC_wGPTM1=185,
	RCC_wGPTM2=186,
	RCC_wGPTM3=187,
	RCC_wGPTM4=188,
	RCC_wGPTM5=189
}RCC_Prph_t;

typedef enum
{
	RCC_CLK_DISABLE=0,
	RCC_CLK_ENABLE
}RCC_PrphClkState_t;
 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/******************************************************************************************************************************
 * \Syntax          : ErrorState_t RCC_SetPrephralClockState(RCC_Prph_t Copy_Peripheral, RCC_PrphClkState_t Copy_ClockState)
 * \Description     : Set the peripheral clock state
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Peripheral   The peripheral to control its clock
 *                    Copy_ClockState   The state to be set
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 ******************************************************************************************************************************/
ErrorState_t RCC_SetPrephralClockState(RCC_Prph_t Copy_Peripheral, RCC_PrphClkState_t Copy_ClockState);

/**************************************************************************************************************************************
 * \Syntax          : ErrorState_t RCC_SetPrephralClockStateSleepMode(RCC_Prph_t Copy_Peripheral, RCC_PrphClkState_t Copy_ClockState)
 * \Description     : Set the peripheral clock state in sleep mode
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Peripheral   The peripheral to control its clock
 *                    Copy_ClockState   The state to be set
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 **************************************************************************************************************************************/
ErrorState_t RCC_SetPrephralClockStateSleepMode(RCC_Prph_t Copy_Peripheral, RCC_PrphClkState_t Copy_ClockState);

/**************************************************************************************************************************************
 * \Syntax          : ErrorState_t RCC_SetPrephralClockStateDeepSleepMode(RCC_Prph_t Copy_Peripheral, RCC_PrphClkState_t Copy_ClockState)
 * \Description     : Set the peripheral clock state in Deep sleep mode
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Peripheral   The peripheral to control its clock
 *                    Copy_ClockState   The state to be set
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 **************************************************************************************************************************************/
ErrorState_t RCC_SetPrephralClockStateDeepSleepMode(RCC_Prph_t Copy_Peripheral, RCC_PrphClkState_t Copy_ClockState);

/**************************************************************************************************************************************
 * \Syntax          : ErrorState_t RCC_IsPeripheralPresent(RCC_Prph_t Copy_Peripheral, bool* Copy_IsPersent)
 * \Description     : Check if the peripheral present or not
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Reentrant
 * \Parameters (in) : Copy_Peripheral   The peripheral to control its clock
 * \Parameters (out): Copy_IsPersent    The state of Peripheral
 * \Return value:   : ErrorState_t
 **************************************************************************************************************************************/
ErrorState_t RCC_IsPeripheralPresent(RCC_Prph_t Copy_Peripheral, bool* Copy_IsPersent);

/**************************************************************************************************************************************
 * \Syntax          : ErrorState_t RCC_PeripheralSWReset(RCC_Prph_t Copy_Peripheral)
 * \Description     : Do software reset on a peripheral
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Peripheral   The peripheral to control its clock
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 **************************************************************************************************************************************/
ErrorState_t RCC_PeripheralSWReset(RCC_Prph_t Copy_Peripheral);

/**************************************************************************************************************************************
 * \Syntax          : void RCC_ConfigureClock(u8 Clock_Freq, u8 PWM_Config)
 * \Description     : Set the System Clock and PWM clock Configuration
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Clock_Freq   		The System Frequency
 *                    PWM_Config   		The PWM Configuration
 * \Parameters (out): None
 * \Return value:   : void
 **************************************************************************************************************************************/
void RCC_ConfigureClock(u8 Clock_Freq, u32 PWM_Config);

#define NO_PWM                          0U
#define RCC_USE_PWM                     0x00010000UL /* USEPWMDIV */
#define RCC_PWMDIV_2                    0x00000000UL /* PWMDIV /2 */
#define RCC_PWMDIV_4                    0x00002000UL /* PWMDIV /4 */
#define RCC_PWMDIV_8                    0x00004000UL /* PWMDIV /8 */
#define RCC_PWMDIV_16                   0x00006000UL /* PWMDIV /16 */
#define RCC_PWMDIV_32                   0x00008000UL /* PWMDIV /32 */
#define RCC_PWMDIV_64                   0x0000E000UL /* PWMDIV /64 */

#endif  /* RCC_INTERFACE_H */

/**********************************************************************************************************************
 *  END OF FILE: RCC_interface.h
 *********************************************************************************************************************/

