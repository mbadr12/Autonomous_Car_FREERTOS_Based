/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Mahmoud Badr
 *	   	   File:  Switch_Program.c
 *		  Layer:  HAL
 *       Module:  Switch
 *		Version:  1.00
 *	
 *  Description:  -     
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "../../LIB/Std_Types.h"
#include "../../LIB/Bit_Math.h"
#include "../../MCAL/RCC/RCC.h"
#include "../../MCAL/GPIO/GPIO.h"
#include "../../MCAL/EXTI/EXTI.h"
#include "../../MCAL/NVIC/NVIC.h"
#include "Switch.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/********************************************************************************************************************************
 * \Syntax          : ErrorState_t Switch_Init(GPIO_Port_t Copy_SwPort,GPIO_Pin_t Copy_SwPin,GPIO_PinPullUpDown_t Copy_SwState)
 * \Description     : Initialize the Switch
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_SwPort     The port of pin switch connected to
 *                    Copy_SwPin      The pin switch connected to
 *                    Copy_SwState    The wanted Pull state of Switch
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 ********************************************************************************************************************************/
ErrorState_t Switch_Init(GPIO_Port_t Copy_SwPort,GPIO_Pin_t Copy_SwPin,GPIO_PinPullUpDown_t Copy_SwState)
{
    ErrorState_t Local_ErrorState=E_OK;
    GPIO_Config_t Local_SwConfig={.Dir=GPIO_PIN_INPUT,.Mode=GPIO_PIN_DIGITAL,.PinPull=Copy_SwState,.Pin=Copy_SwPin,.Port=Copy_SwPort,.Mode=GPIO_PIN_DIGITAL};
    /*Enable POrt Clock*/
    RCC_SetPrephralClockState((RCC_Prph_t)(RCC_GPIOA+Copy_SwPort), RCC_CLK_ENABLE);
    /*Initialize the Switch Pin*/
    Local_ErrorState=GPIO_Init(&Local_SwConfig, 1);
    return Local_ErrorState;
}

/********************************************************************************************************************************
 * \Syntax          : ErrorState_t Switch_IntConfig(EXTI_Config_t *Copy_Button,void(*Copy_NotificationFunc)(void))
 * \Description     : Initialize The interrupt configuration of the Switch
 *
 * \Sync\Async      : Asynchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Button                The Interrupt Configuration structure of the Switch
 *                    Copy_NotificationFunc      The Notification function of the switch Pressing
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 ********************************************************************************************************************************/
ErrorState_t Switch_IntConfig(EXTI_Config_t *Copy_Button,void(*Copy_NotificationFunc)(void))
{
    ErrorState_t Local_ErrorState=E_OK;
    Copy_Button->Sence=EXTI_EDGE;
    /*Initialize the EXTI for the Corresponding Button*/
    Local_ErrorState=EXTI_Init(Copy_Button, 1);
    if(Local_ErrorState==E_OK)
    {
        Local_ErrorState=EXTI_SetCallBack(Copy_Button->Port, Copy_Button->Pin, Copy_NotificationFunc);
        /*Enable the IRQ from NVIC*/
        if(Copy_Button->Port<EXTI_PORTF)
        {
            NVIC_SetInterruptState((NVIC_IRQ_t)(NVIC_PORTA_IRQ+Copy_Button->Port), NVIC_INT_ENABLE);
        }
        else
        {
            NVIC_SetInterruptState(NVIC_PORTF_IRQ, NVIC_INT_ENABLE);
        }
    }
    else
    {
        /*MISRA*/
    }
    return Local_ErrorState;
}

/********************************************************************************************************************************************************************
 * \Syntax          : ErrorState_t Switch_GetStatus(GPIO_Port_t Copy_SwPort,GPIO_Pin_t Copy_SwPin,GPIO_PinPullUpDown_t Copy_SwPullType,Switch_State_t* Copy_SwState)
 * \Description     : Get the Switch state Pressed or not
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_SwPort     The port of pin switch connected to
 *                    Copy_SwPin      The pin switch connected to
 *                    Copy_SwPullType The wanted Pull state of Switch
 * \Parameters (out): Copy_SwState    The Switch state Pressed or Not
 * \Return value:   : ErrorState_t
 ********************************************************************************************************************************************************************/
ErrorState_t Switch_GetStatus(GPIO_Port_t Copy_SwPort,GPIO_Pin_t Copy_SwPin,GPIO_PinPullUpDown_t Copy_SwPullType,Switch_State_t* Copy_SwState)
{
    ErrorState_t Local_ErrorState=E_OK;
    GPIO_PinValue_t Local_PressedState=GPIO_PIN_LOW;
    GPIO_PinValue_t Local_PinState=GPIO_PIN_LOW;
    *Copy_SwState=SW_NOT_PRESSED;
    /*Check for Pull type*/
    if(Copy_SwPullType==GPIO_PIN_PULLDOWN)
    {
        Local_PressedState=GPIO_PIN_HIGH;
    }
    else
    {
        /*MISRA*/
    }
    Local_ErrorState=GPIO_GetPinValue(Copy_SwPort, Copy_SwPin, &Local_PinState);
    if(Local_PinState==Local_PressedState)
    {
        *Copy_SwState=SW_PRESSED;
    }
    else
    {
        /*MISRA*/
    }
    return Local_ErrorState;
}

/**********************************************************************************************************************
 *  END OF FILE: Switch_Program.c
 *********************************************************************************************************************/
