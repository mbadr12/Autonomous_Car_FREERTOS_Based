/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Mahmoud Badr
 *         File:  GPIO_Program.c
 *        Layer:  MCAL
 *       Module:  GPIO
 *      Version:  1.00
 *
 *  Description:  Implementation of GPIO Driver
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "../../LIB/Std_Types.h"
#include "../../LIB/TM4C123xx.h"
#include "../../LIB/Bit_Math.h"

#include "GPIO.h"

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
static GPIO_RegDef_t* GPIO_Arr[GPIO_PORTNUM]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF};

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/***************************************************************************************************************************
* \Syntax          : ErrorState_t GPIO_Init(const GPIO_Config_t* Copy_Config,u8 Copy_PinNum)
* \Description     : Initialize the GPIO Pins
*
* \Sync\Async      : Synchronous
* \Reentrancy      : Non Reentrant
* \Parameters (in) : GPIO_Config_t   Array of configuration structure for GPIO
*                    Copy_PinNum     Number of Pins to be initialized
* \Parameters (out): None
* \Return value:   : ErrorState_t
***************************************************************************************************************************/
ErrorState_t GPIO_Init(const GPIO_Config_t* Copy_Config,u8 Copy_PinNum)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_Counter=0;
    if(Copy_Config == NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {

        for(Local_Counter=0; Local_Counter<Copy_PinNum; Local_Counter++)
        {
            if((GPIO_Config.Port > GPIO_PORTF)||(GPIO_Config.Pin>GPIO_PIN7))
            {
                Local_ErrorState=E_WRONG_OPTION;
            }
            else
            {
                if((GPIO->LOCK==1) && (GPIO_Config.Port!=GPIO_PORTC))
                {
                    /*Unlock the port*/
                    GPIO->LOCK=GPIO_UNLOCK;
                    /*Unlock commit for pins*/
                    SET_BIT(GPIO->CR,GPIO_Config.Pin);
                }
                else
                {
                    /* For MISRA */
                }
                /*Set the pin Direction*/
                INSERT_BIT(GPIO->DIR,GPIO_Config.Pin,GPIO_Config.Dir);
                /*Set pin Mode*/
                switch(GPIO_Config.Mode)
                {
                case GPIO_PIN_DIGITAL: SET_BIT(GPIO->DEN,GPIO_Config.Pin); break;
                case GPIO_PIN_ALTFUNC: SET_BIT(GPIO->AFSEL,GPIO_Config.Pin); SET_BIT(GPIO->DEN,GPIO_Config.Pin);
                GPIO->PCTL&=(~((0b1111)<<(4*GPIO_Config.Pin))); GPIO->PCTL|=((GPIO_Config.AltFuncNum)<<(4*GPIO_Config.Pin)); break;
                case GPIO_PIN_ANALOG: SET_BIT(GPIO->AMSEL,GPIO_Config.Pin); break;
                default: Local_ErrorState=E_WRONG_OPTION; break;
                }
                if(GPIO_Config.PinState==GPIO_PIN_OPENDRAIN)
                {
                    /*Set Pin State*/
                    SET_BIT(GPIO->ODR,GPIO_Config.Pin);
                }
                else
                {
                    /* For MISRA */
                }
                if(GPIO_Config.PinPull != GPIO_PIN_FLOATING)
                {
                    /*Set the pull type*/
                    SET_BIT(GPIO->PxR[GPIO_Config.PinPull],GPIO_Config.Pin);
                }
                else
                {
                    /* For MISRA */
                }
                /*set the output Current*/
                if(GPIO_Config.OutputCurrent<=GPIO_8MA_DRIVE)
                {
                    SET_BIT((GPIO->DRxR[GPIO_Config.OutputCurrent]),GPIO_Config.Pin);
                }
                else
                {
                    /*Do Nothing*/
                }
            }
        }
    }
    return Local_ErrorState;
}

/***************************************************************************************************************************
* \Syntax          : ErrorState_t GPIO_SetPinValue(GPIO_Port_t Copy_Port,GPIO_Pin_t Copy_Pin,GPIO_PinValue_t Copy_PinValue)
* \Description     : Set Value to Digital Pin
*
* \Sync\Async      : Synchronous
* \Reentrancy      : Reentrant
* \Parameters (in) : Copy_Port       The port of pin to set its value
*                    Copy_Pin        The pin to set its Value
*                    Copy_PinValue   The Pin value
* \Parameters (out): None
* \Return value:   : ErrorState_t
***************************************************************************************************************************/
ErrorState_t GPIO_SetPinValue(GPIO_Port_t Copy_Port,GPIO_Pin_t Copy_Pin,GPIO_PinValue_t Copy_PinValue)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_RegNum=1<<Copy_Pin;
    if((Copy_Port>GPIO_PORTF) || (Copy_Pin>GPIO_PIN7))
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        GPIO_Arr[Copy_Port]->DATA[Local_RegNum]=Copy_PinValue<<Copy_Pin;
    }
    return Local_ErrorState;
}

/***************************************************************************************************************************
* \Syntax          : ErrorState_t GPIO_SetPortValue(GPIO_Port_t Copy_Port,Gpio_PortValue_t Copy_PortValue)
* \Description     : Set Value to Digital Port
*
* \Sync\Async      : Synchronous
* \Reentrancy      : Reentrant
* \Parameters (in) : Copy_Port       The port to set its value
*                    Copy_PortValue  The Port value
* \Parameters (out): None
* \Return value:   : ErrorState_t
***************************************************************************************************************************/
ErrorState_t GPIO_SetPortValue(GPIO_Port_t Copy_Port,Gpio_PortValue_t Copy_PortValue)
{
    ErrorState_t Local_ErrorState=E_OK;
    if((Copy_Port>GPIO_PORTF) || (Copy_PortValue>255))
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        GPIO_Arr[Copy_Port]->DATA[GPIO_PORT_DATA]=(u32)Copy_PortValue;
    }
    return Local_ErrorState;
}

/***************************************************************************************************************************
* \Syntax          : ErrorState_t GPIO_GetPinValue(GPIO_Port_t Copy_Port,GPIO_Pin_t Copy_Pin,GPIO_PinValue_t* Copy_PinValue)
* \Description     : Get the Value of Digital Pin
*
* \Sync\Async      : Synchronous
* \Reentrancy      : Reentrant
* \Parameters (in) : Copy_Port       The port of pin to get its value
*                    Copy_Pin        The pin to get its Value
* \Parameters (out): Copy_PinValue   The Pin value
* \Return value:   : ErrorState_t
***************************************************************************************************************************/
ErrorState_t GPIO_GetPinValue(GPIO_Port_t Copy_Port,GPIO_Pin_t Copy_Pin,GPIO_PinValue_t* Copy_PinValue)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_RegNum=1<<Copy_Pin;
    if(Copy_PinValue == NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        if((Copy_Port>GPIO_PORTF) || (Copy_Pin>GPIO_PIN7))
        {
            Local_ErrorState=E_WRONG_OPTION;
        }
        else
        {
            *Copy_PinValue=(GPIO_PinValue_t)((GPIO_Arr[Copy_Port]->DATA[Local_RegNum])>>Copy_Pin);
        }
    }
    return Local_ErrorState;
}

/***************************************************************************************************************************
* \Syntax          : ErrorState_t GPIO_GetPortValue(GPIO_Port_t Copy_Port,Gpio_PortValue_t* Copy_PortValue)
* \Description     : Get the value of Digital Port
*
* \Sync\Async      : Synchronous
* \Reentrancy      : Reentrant
* \Parameters (in) : Copy_Port       The port to get its value
* \Parameters (out): Copy_PortValue  The Port value
* \Return value:   : ErrorState_t
***************************************************************************************************************************/
ErrorState_t GPIO_GetPortValue(GPIO_Port_t Copy_Port,Gpio_PortValue_t* Copy_PortValue)
{
    ErrorState_t Local_ErrorState=E_OK;
    if(Copy_PortValue == NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        if(Copy_Port>GPIO_PORTF)
        {
            Local_ErrorState=E_WRONG_OPTION;
        }
        else
        {
            *Copy_PortValue=GPIO_Arr[Copy_Port]->DATA[GPIO_PORT_DATA];
        }
    }
    return Local_ErrorState;
}

/**********************************************************************************************************************
 *  END OF FILE: GPIO_Program.c
 *********************************************************************************************************************/
