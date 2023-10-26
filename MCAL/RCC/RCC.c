/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Mahmoud Badr
 *	   	   File:  RCC_Program.c
 *		  Layer:  MCAL
 *       Module:  RCC
 *		Version:  1.00
 *	
 *  Description:  -     
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "../../LIB/Std_Types.h"
#include "../../LIB/TM4C123xx.h"
#include "../../LIB/Bit_Math.h"

#include "RCC.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
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
ErrorState_t RCC_SetPrephralClockState(RCC_Prph_t Copy_Peripheral, RCC_PrphClkState_t Copy_ClockState)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_RegNum=Copy_Peripheral/8;
    u8 Local_BitNum=Copy_Peripheral%8;
    if((Copy_Peripheral>RCC_wGPTM5) || (Copy_ClockState>RCC_CLK_ENABLE))
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        INSERT_BIT(SYSCTL->RCGC[Local_RegNum],Local_BitNum,Copy_ClockState);
    }
    return Local_ErrorState;
}

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
ErrorState_t RCC_SetPrephralClockStateSleepMode(RCC_Prph_t Copy_Peripheral, RCC_PrphClkState_t Copy_ClockState)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_RegNum=Copy_Peripheral/8;
    u8 Local_BitNum=Copy_Peripheral%8;
    if((Copy_Peripheral>RCC_wGPTM5) || (Copy_ClockState>RCC_CLK_ENABLE))
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        INSERT_BIT(SYSCTL->SCGC[Local_RegNum],Local_BitNum,Copy_ClockState);
    }
    return Local_ErrorState;
}

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
ErrorState_t RCC_SetPrephralClockStateDeepSleepMode(RCC_Prph_t Copy_Peripheral, RCC_PrphClkState_t Copy_ClockState)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_RegNum=Copy_Peripheral/8;
    u8 Local_BitNum=Copy_Peripheral%8;
    if((Copy_Peripheral>RCC_wGPTM5) || (Copy_ClockState>RCC_CLK_ENABLE))
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        INSERT_BIT(SYSCTL->DCGC[Local_RegNum],Local_BitNum,Copy_ClockState);
    }
    return Local_ErrorState;
}

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
ErrorState_t RCC_IsPeripheralPresent(RCC_Prph_t Copy_Peripheral, bool* Copy_IsPersent)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_RegNum=Copy_Peripheral/8;
    u8 Local_BitNum=Copy_Peripheral%8;
    if((Copy_Peripheral>RCC_wGPTM5) || (Copy_IsPersent == NULL))
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        *Copy_IsPersent=(bool)GET_BIT(SYSCTL->PP[Local_RegNum],Local_BitNum);
    }
    return Local_ErrorState;
}

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
ErrorState_t RCC_PeripheralSWReset(RCC_Prph_t Copy_Peripheral)
{
    {
        ErrorState_t Local_ErrorState=E_OK;
        u8 Local_RegNum=Copy_Peripheral/8;
        u8 Local_BitNum=Copy_Peripheral%8;
        if(Copy_Peripheral>RCC_wGPTM5)
        {
            Local_ErrorState=E_WRONG_OPTION;
        }
        else
        {
            SET_BIT(SYSCTL->SR[Local_RegNum],Local_BitNum);
            CLR_BIT(SYSCTL->SR[Local_RegNum],Local_BitNum);
        }
        return Local_ErrorState;
    }
}

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
void RCC_ConfigureClock(u8 Clock_Freq, u32 PWM_Config)
{
    SYSCTL->RCC &= ~(1UL << 22); /* Clear USESYSDIV bit */
    SYSCTL->RCC &= ~(0xFUL << 6); /* Clear XTAL bits */
    SYSCTL->RCC |= (0x15UL << 6); /* XTAL 16 MHz */
    SYSCTL->RCC &= ~(1UL << 0); /* Enable MOSC */
    while ((SYSCTL->RIS & (1U << 8)) == 0U)
        ; /* Wait for MOSC to stabilize */
    SYSCTL->MISC |= (1UL << 8); /* Clear flag */
    SYSCTL->RCC2 |= (1UL << 31); /* Set USERCC2 bit */
    SYSCTL->RCC2 |= (1UL << 11); /* Set BYPASS2 bit */
    SYSCTL->RCC2 &= ~(3UL << 4); /* Use MOSC as source */
    SYSCTL->RCC2 &= ~(0x3FUL << 23); /* Clear SYSDIV2 bits */
    SYSCTL->RCC2 |= (1UL << 30); /* PLL 400 MHz */
    SYSCTL->RCC2 &= ~(1UL << 13); /* Power up PLL */
    u32 divisor = 400U / Clock_Freq; /* Calculate divisor */
    while (divisor < 3U || divisor > 128U)
        ; /* If divisor less than 3 or larger then 128 then infinite loop */
    SYSCTL->RCC2 |= (((divisor - 1UL) / 2UL) << 23); /* Set divisor in SYSDIV2 */
    SYSCTL->RCC2 &= (~1U << 22); /* Clear SYSDIV2LSB */
    SYSCTL->RCC2 |= (((divisor ^ 0x01UL) & 0x1UL) << 22); /* Set SYSDIV2LSB if divisor is even */
    SYSCTL->RCC |= (1UL << 22); /* Set USESYSDIV bit */
    while ((SYSCTL->RIS & (1UL << 6)) == 0U)
        ; /* Wait for PLL to stabilize */
    SYSCTL->MISC |= (1UL << 6); /* Clear flag */
    SYSCTL->RCC2 &= ~(1UL << 11); /* Clear BYPASS2 bit */
    if ((PWM_Config & RCC_USE_PWM) == RCC_USE_PWM)
    {
        SYSCTL->RCC |= RCC_USE_PWM;
        SYSCTL->RCC &= ~RCC_PWMDIV_64;
        SYSCTL->RCC |= PWM_Config & ~RCC_USE_PWM;
    }
    else
    {
    }
}

/**********************************************************************************************************************
 *  END OF FILE: RCC_Program.c
 *********************************************************************************************************************/
