/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Mahmoud Badr
 *	   	   File:  NVIC_Program.c
 *		  Layer:  MCAL
 *       Module:  NVIC
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

#include "NVIC.h"

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/******************************************************************************
 * \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)
 * \Description     : Describe this service
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : parameterName   Parameter Description
 * \Parameters (out): parameterName   Parameter Description
 * \Return value:   : Std_ReturnType  E_OK
 *                                    E_NOT_OK
 *******************************************************************************/
ErrorState_t NVIC_SetInterruptState(NVIC_IRQ_t Copy_IRQ_n,NVIC_IntState_t Copy_State)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_RegNum=0,Local_BitNum=0;
    if(Copy_IRQ_n>NVIC_PWM1_FAULT_IRQ)
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        Local_RegNum=Copy_IRQ_n/32;
        Local_BitNum=Copy_IRQ_n%32;
        switch(Copy_State)
        {
        case NVIC_INT_ENABLE: NVIC->EN[Local_RegNum]=1<<Local_BitNum; break;
        case NVIC_INT_DISABLE: NVIC->DIS[Local_RegNum]=1<<Local_BitNum; break;
        default: Local_ErrorState=E_WRONG_OPTION;
        }
    }
    return Local_ErrorState;
}

ErrorState_t NVIC_SetPendingFlag(NVIC_IRQ_t Copy_IRQ_n,NVIC_IntState_t Copy_State)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_RegNum=0,Local_BitNum=0;
    if(Copy_IRQ_n>NVIC_PWM1_FAULT_IRQ)
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        Local_RegNum=Copy_IRQ_n/32;
        Local_BitNum=Copy_IRQ_n%32;
        switch(Copy_State)
        {
        case NVIC_INT_ENABLE: NVIC->PEND[Local_RegNum]=1<<Local_BitNum; break;
        case NVIC_INT_DISABLE: NVIC->DIS[Local_RegNum]=1<<Local_BitNum; break;
        default: Local_ErrorState=E_WRONG_OPTION;
        }
    }
    return Local_ErrorState;
}

ErrorState_t NVIC_GetActiveFlag(NVIC_IRQ_t Copy_IRQ_n,NVIC_IntState_t* Copy_State)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_RegNum=0,Local_BitNum=0;
    if(Copy_State==NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        if(Copy_IRQ_n>NVIC_PWM1_FAULT_IRQ)
        {
            Local_ErrorState=E_WRONG_OPTION;
        }
        else
        {
            Local_RegNum=Copy_IRQ_n/32;
            Local_BitNum=Copy_IRQ_n%32;
            *Copy_State=(NVIC_IntState_t)GET_BIT(NVIC->ACTIVE[Local_RegNum],Local_BitNum);
        }
    }
    return Local_ErrorState;
}

/**********************************************************************************************************************
 *  END OF FILE: NVIC_Program.c
 *********************************************************************************************************************/
