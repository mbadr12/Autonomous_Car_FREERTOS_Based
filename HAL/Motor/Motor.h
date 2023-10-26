/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Adham EL-Serougy
 *         File:  Motor.h
 *        Layer:  HAL
 *       Module:  Motor
 *      Version:  1.00
 *
 *  Description:  Interfaces Needed by user
 *
 *********************************************************************************************************************/
#ifndef HAL_MOTOR_MOTOR_H_
#define HAL_MOTOR_MOTOR_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <stdint.h>
#include "../../LIB/Std_Types.h"
#include "MCAL/GPIO/GPIO.h"
#include "MCAL/PWM/PWM.h"
#include "MCAL/RCC/RCC.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define Right_Motors                    0U
#define Left_Motors                     1U

#define Motor_Right_Forward             0U
#define Motor_Right_Reverse             1U
#define Motor_Left_Forward              2U
#define Motor_Left_Reverse              3U

/******************************************************************************
 * \Syntax          : void Motor_Init(void)
 * \Description     : Initialize Motors
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : void
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Motor_Init(void);
/******************************************************************************
 * \Syntax          : void Motor_Start(void)
 * \Description     : Start Motors
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : void
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Motor_Start(void);
/******************************************************************************
 * \Syntax          : void Motor_Set_Speed(uint16_t Speed, uint8_t Motors)
 * \Description     : Set Motor Speed
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Speed   Speed in percentage
 *                    Motors   which motor to change speed
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Motor_Set_Speed(uint16_t Speed, uint8_t Motors);
/******************************************************************************
 * \Syntax          : void Motor_Set_Direction(uint8_t Direction)
 * \Description     : Set Motor Direction
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Direction   Forward or Backward
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Motor_Set_Direction(uint8_t Direction);
/******************************************************************************
 * \Syntax          : void Motor_Stop(void)
 * \Description     : Stop Motors
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : void
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Motor_Stop(void);

#endif /* HAL_MOTOR_MOTOR_H_ */
