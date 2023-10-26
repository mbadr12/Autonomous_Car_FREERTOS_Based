/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Mahmoud Badr
 *	   	   File:  NVIC_interface.h
 *		  Layer:  MCAL
 *       Module:  NVIC
 *		Version:  1.00
 *	
 *  Description:  -     
 *  
 *********************************************************************************************************************/
#ifndef NVIC_INTERFACE_H
#define NVIC_INTERFACE_H

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef enum
{
	NVIC_PORTA_IRQ=0,
	NVIC_PORTB_IRQ=1,
	NVIC_PORTC_IRQ=2,
	NVIC_PORTD_IRQ=3,
	NVIC_PORTE_IRQ=4,
	NVIC_UART0_IRQ=5,
	NVIC_UART1_IRQ=6,
	NVIC_SSI0_IRQ=7,
	NVIC_I2C0_IRQ=8,
	NVIC_PWM0FAULT_IRQ=9,
	NVIC_PWM0GENERATOR0_IRQ=10,
	NVIC_PWM0GENERATOR1_IRQ=11,
	NVIC_PWM0GENERATOR2_IRQ=12,
	NVIC_QEI0_IRQ=13,
	NVIC_ADC0SEQUENCE0_IRQ=14,
	NVIC_ADC0SEQUENCE1_IRQ=15,
	NVIC_ADC0SEQUENCE2_IRQ=16,
	NVIC_ADC0SEQUENCE3_IRQ=17,
	NVIC_WATCHDOGTIMERS0_1_IRQ=18,
	NVIC_TIMER0A_16_32_IRQ=19,
	NVIC_TIMER0B_16_32_IRQ=20,
	NVIC_TIMER1A_16_32_IRQ=21,
	NVIC_TIMER1B_16_32_IRQ=22,
	NVIC_TIMER2A_16_32_IRQ=23,
	NVIC_TIMER2B_16_32_IRQ=24,
	NVIC_ANALOG_COMPARTOR0_IRQ=25,
	NVIC_ANALOG_COMPARTOR1_IRQ=26,
	NVIC_SYSCTRL_IRQ=28,
	NVIC_FLASH_EEPROM_CTRL_IRQ=29,
	NVIC_PORTF_IRQ=30,
	NVIC_UART2_IRQ=33,
	NVIC_SSI1_IRQ=34,
	NVIC_TIMER3A_16_32_IRQ=35,
	NVIC_TIMER3B_16_32_IRQ=36,
	NVIC_I2C1_IRQ=37,
	NVIC_QEI1_IRQ=38,
	NVIC_CAN0_IRQ=39,
	NVIC_CAN1_IRQ=40,
	NVIC_HIB_IRQ=43,
	NVIC_USB0_IRQ=44,
	NVIC_PWM0_GENERATOR3_IRQ=45,
	NVIC_UDMA_IRQ=46,
	NVIC_UDMAERR_IRQ=47,
	NVIC_ADC1SEQUENCE0_IRQ=48,
	NVIC_ADC1SEQUENCE1_IRQ=49,
	NVIC_ADC1SEQUENCE2_IRQ=50,
	NVIC_ADC1SEQUENCE3_IRQ=51,
	NVIC_SSI2_IRQ=57,
	NVIC_SSI3_IRQ=58,
	NVIC_UART3_IRQ=59,
	NVIC_UART4_IRQ=60,
	NVIC_UART5_IRQ=61,
	NVIC_UART6_IRQ=62,
	NVIC_UART7_IRQ=63,
	NVIC_I2C2_IRQ=68,
	NVIC_I2C3_IRQ=69,
	NVIC_TIMER4A_16_32_IRQ=70,
	NVIC_TIMER4B_16_32_IRQ=71,
	NVIC_TIMER5A_16_32_IRQ=92,
	NVIC_TIMER5B_16_32_IRQ=93,
	NVIC_TIMER0A_32_64_IRQ=94,
	NVIC_TIMER0B_32_64_IRQ=95,
	NVIC_TIMER1A_32_64_IRQ=96,
	NVIC_TIMER1B_32_64_IRQ=97,
	NVIC_TIMER2A_32_64_IRQ=98,
	NVIC_TIMER2B_32_64_IRQ=99,
	NVIC_TIMER3A_32_64_IRQ=100,
	NVIC_TIMER3B_32_64_IRQ=101,
	NVIC_TIMER4A_32_64_IRQ=102,
	NVIC_TIMER4B_32_64_IRQ=103,
	NVIC_TIMER5A_32_64_IRQ=104,
	NVIC_TIMER5B_32_64_IRQ=105,
	NVIC_FPU_IRQ=106,
	NVIC_PMW1_GENERATOR0_IRQ=134,
	NVIC_PWM1_GENERATOR1_IRQ=135,
	NVIC_PWM1_GENERATOR2_IRQ=136,
	NVIC_PWM1_GENERATOR3_IRQ=137,
	NVIC_PWM1_FAULT_IRQ=138
}NVIC_IRQ_t;

typedef enum
{
    NVIC_INT_DISABLE=0,
    NVIC_INT_ENABLE,
}NVIC_IntState_t;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                    
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): parameterName   Parameter Describtion                                                    
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
ErrorState_t NVIC_SetInterruptState(NVIC_IRQ_t Copy_IRQ_n,NVIC_IntState_t Copy_State);

ErrorState_t NVIC_SetPendingFlag(NVIC_IRQ_t Copy_IRQ_n,NVIC_IntState_t Copy_State);

ErrorState_t NVIC_GetActiveFlag(NVIC_IRQ_t Copy_IRQ_n,NVIC_IntState_t* Copy_State);

#endif  /* NVIC_INTERFACE_H */

/**********************************************************************************************************************
 *  END OF FILE: NVIC_interface.h
 *********************************************************************************************************************/
