/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Mahmoud Badr
 *	   	   File:  EXTI_Program.c
 *		  Layer:  MCAL
 *       Module:  EXTI
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

#include "EXTI.h"

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
static GPIO_RegDef_t* EXTI_Arr[EXTI_PORTS]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF};

static void (*EXTI_CallBackFunc[EXTI_PORTS][EXTI_PINS])(void)={{NULL}};

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/******************************************************************************************************************
 * \Syntax          : ErrorState_t EXTI_Init(EXTI_EXTI_Config_t* Copy_EXTI_Config,u8 Copy_PinsNum)
 * \Description     : set the required EXTI_Configuration to number of pins
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_EXTI_Config     EXTI_Configuration structure
 *                    Copy_PinsNum    Number of Pins to be EXTI_Configured
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 *******************************************************************************************************************/
ErrorState_t EXTI_Init(EXTI_Config_t* Copy_Config,u8 Copy_PinsNum)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_Counter=0;
    if(Copy_Config==NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        for( ;Local_Counter<Copy_PinsNum;Local_Counter++)
        {
            if((EXTI_Config.Port>EXTI_PORTF) || (EXTI_Config.Pin>EXTI_PIN7))
            {
                Local_ErrorState=E_WRONG_OPTION;
            }
            else
            {
                /*EXTI_Configure the sense type*/
                INSERT_BIT(EXTI_GPIO->IM,EXTI_Config.Pin,EXTI_Config.Sence);
                /*EXTI_Configure the trigger type if edge trigger*/
                if(EXTI_Config.Sence==EXTI_EDGE)
                {
                    if(EXTI_Config.TrigTye==EXTI_ONCHANGE)
                    {
                        SET_BIT(EXTI_GPIO->IBE,EXTI_Config.Pin);
                    }
                    else
                    {
                        INSERT_BIT(EXTI_GPIO->IEV,EXTI_Config.Pin,EXTI_Config.TrigTye);
                    }
                }
                /*Enable Interrupt of pin*/
                SET_BIT(EXTI_GPIO->IM,EXTI_Config.Pin);
            }
        }
    }
    return Local_ErrorState;
}

/*******************************************************************************************************************************
 * \Syntax          : ErrorState_t EXTI_SetCallBack(EXTI_Port_t Copy_Port, EXTI_Pin_t Copy_Pin,void (*Copy_CallBackFunc)(void))
 * \Description     : Set the callback function to specific pin
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Reentrant
 * \Parameters (in) : Copy_Port                         Port of pin to set the callback to
 *                    Copy_Pin                          Pin to set the callback to
 *                    void (*Copy_CallBackFunc)(void)   pointer to The callback function to be set
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 ********************************************************************************************************************************/
ErrorState_t EXTI_SetCallBack(EXTI_Port_t Copy_Port, EXTI_Pin_t Copy_Pin,void (*Copy_CallBackFunc)(void))
{
    ErrorState_t Local_ErrorState=E_OK;
    if(Copy_CallBackFunc==NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        if((Copy_Port>EXTI_PORTF) || (Copy_Pin>EXTI_PIN7))
        {
            Local_ErrorState=E_WRONG_OPTION;
        }
        else
        {
            /*Passing Callback function to global pointer to function*/
            EXTI_CallBackFunc[Copy_Port][Copy_Pin]=Copy_CallBackFunc;
        }
    }
    return Local_ErrorState;
}

/*******************************************************************************************************************************
 * \Syntax          : void GPIOA_Handler(void)
 * \Description     : EXTI Handler for GPIOA
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non-Reentrant
 * \Parameters (in) : none
 * \Parameters (out): None
 * \Return value:   : void
 ********************************************************************************************************************************/
void GPIOA_Handler(void)
{
    u8 Local_PinNum=0;
    for( ;Local_PinNum<EXTI_PINS;Local_PinNum++)
    {
        if(GET_BIT(EXTI_Arr[EXTI_PORTA]->RIS,Local_PinNum)==1)
        {
            if(EXTI_CallBackFunc[EXTI_PORTA][Local_PinNum] != NULL)
            {
                /*Invoke Callback function*/
                EXTI_CallBackFunc[EXTI_PORTA][Local_PinNum]();
            }
            /*Clear the Interrupt flag*/
            EXTI_Arr[EXTI_PORTA]->ICR=1<<Local_PinNum;
        }
    }
}

/*******************************************************************************************************************************
 * \Syntax          : void GPIOB_Handler(void)
 * \Description     : EXTI Handler for GPIOB
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non-Reentrant
 * \Parameters (in) : none
 * \Parameters (out): None
 * \Return value:   : void
 ********************************************************************************************************************************/
void GPIOB_Handler(void)
{
    u8 Local_PinNum=0;
    for( ;Local_PinNum<EXTI_PINS;Local_PinNum++)
    {
        if(GET_BIT(EXTI_Arr[EXTI_PORTB]->RIS,Local_PinNum)==1)
        {
            if(EXTI_CallBackFunc[EXTI_PORTB][Local_PinNum] != NULL)
            {
                /*Invoke Callback function*/
                EXTI_CallBackFunc[EXTI_PORTB][Local_PinNum]();
            }
            /*Clear the Interrupt flag*/
            EXTI_Arr[EXTI_PORTB]->ICR=1<<Local_PinNum;
        }
    }
}

/*******************************************************************************************************************************
 * \Syntax          : void GPIOB_Handler(void)
 * \Description     : EXTI Handler for GPIOB
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non-Reentrant
 * \Parameters (in) : none
 * \Parameters (out): None
 * \Return value:   : void
 ********************************************************************************************************************************/
void GPIOC_Handler(void)
{
    u8 Local_PinNum=0;
    for( ;Local_PinNum<EXTI_PINS;Local_PinNum++)
    {
        if(GET_BIT(EXTI_Arr[EXTI_PORTC]->RIS,Local_PinNum)==1)
        {
            if(EXTI_CallBackFunc[EXTI_PORTC][Local_PinNum] != NULL)
            {
                /*Invoke Callback function*/
                EXTI_CallBackFunc[EXTI_PORTC][Local_PinNum]();
            }
            /*Clear the Interrupt flag*/
            EXTI_Arr[EXTI_PORTC]->ICR=1<<Local_PinNum;
        }
    }
}

/*******************************************************************************************************************************
 * \Syntax          : void GPIOB_Handler(void)
 * \Description     : EXTI Handler for GPIOD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non-Reentrant
 * \Parameters (in) : none
 * \Parameters (out): None
 * \Return value:   : void
 ********************************************************************************************************************************/
void GPIOD_Handler(void)
{
    u8 Local_PinNum=0;
    for( ;Local_PinNum<EXTI_PINS;Local_PinNum++)
    {
        if(GET_BIT(EXTI_Arr[EXTI_PORTD]->RIS,Local_PinNum)==1)
        {
            if(EXTI_CallBackFunc[EXTI_PORTD][Local_PinNum] != NULL)
            {
                /*Invoke Callback function*/
                EXTI_CallBackFunc[EXTI_PORTD][Local_PinNum]();
            }
            /*Clear the Interrupt flag*/
            EXTI_Arr[EXTI_PORTD]->ICR=1<<Local_PinNum;
        }
    }
}

/*******************************************************************************************************************************
 * \Syntax          : void GPIOE_Handler(void)
 * \Description     : EXTI Handler for GPIOD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non-Reentrant
 * \Parameters (in) : none
 * \Parameters (out): None
 * \Return value:   : void
 ********************************************************************************************************************************/
void GPIOE_Handler(void)
{
    u8 Local_PinNum=0;
    for( ;Local_PinNum<EXTI_PINS;Local_PinNum++)
    {
        if(GET_BIT(EXTI_Arr[EXTI_PORTE]->RIS,Local_PinNum)==1)
        {
            if(EXTI_CallBackFunc[EXTI_PORTE][Local_PinNum] != NULL)
            {
                /*Invoke Callback function*/
                EXTI_CallBackFunc[EXTI_PORTE][Local_PinNum]();
            }
            /*Clear the Interrupt flag*/
            EXTI_Arr[EXTI_PORTE]->ICR=1<<Local_PinNum;
        }
    }
}

/*******************************************************************************************************************************
 * \Syntax          : void GPIOF_Handler(void)
 * \Description     : EXTI Handler for GPIOD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non-Reentrant
 * \Parameters (in) : none
 * \Parameters (out): None
 * \Return value:   : void
 ********************************************************************************************************************************/
void GPIOF_Handler(void)
{
    u8 Local_PinNum=0;
    for( ;Local_PinNum<EXTI_PINS;Local_PinNum++)
    {
        if(GET_BIT(EXTI_Arr[EXTI_PORTF]->RIS,Local_PinNum)==1)
        {
            if(EXTI_CallBackFunc[EXTI_PORTF][Local_PinNum] != NULL)
            {
                /*Invoke Callback function*/
                EXTI_CallBackFunc[EXTI_PORTF][Local_PinNum]();
            }
            /*Clear the Interrupt flag*/
            EXTI_Arr[EXTI_PORTF]->ICR=1<<Local_PinNum;
        }
    }
}

/**********************************************************************************************************************
 *  END OF FILE: EXTI_Program.c
 *********************************************************************************************************************/
