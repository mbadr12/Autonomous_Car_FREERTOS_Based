/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Mahmoud Badr
 *	   	   File:  Switch_interface.h
 *		  Layer:  HAL
 *       Module:  Switch
 *		Version:  1.00
 *	
 *  Description:  -     
 *  
 *********************************************************************************************************************/
#ifndef SWITCH_INTERFACE_H
#define SWITCH_INTERFACE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "../../MCAL/GPIO/GPIO.h"
#include "../../MCAL/EXTI/EXTI.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef enum
{
    SW_PRESSED=0,
    SW_NOT_PRESSED
}Switch_State_t;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
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
ErrorState_t Switch_Init(GPIO_Port_t Copy_SwPort,GPIO_Pin_t Copy_SwPin,GPIO_PinPullUpDown_t Copy_SwState);

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
ErrorState_t Switch_IntConfig(EXTI_Config_t *Copy_Button,void(*Copy_NotificationFunc)(void));

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
ErrorState_t Switch_GetStatus(GPIO_Port_t Copy_SwPort,GPIO_Pin_t Copy_SwPin,GPIO_PinPullUpDown_t Copy_SwPullType,Switch_State_t* Copy_SwState);

#endif  /* SWITCH_INTERFACE_H */

/**********************************************************************************************************************
 *  END OF FILE: Switch_interface.h
 *********************************************************************************************************************/
