/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Hazem Magdy
 *         File:  Ultrasonic.h
 *        Layer:  HAL
 *       Module:  Ultrasonic
 *      Version:  1.00
 *
 *  Description:  -
 *
 *********************************************************************************************************************/

#ifndef HAL_ULTRA_SONIC_H_
#define HAL_ULTRA_SONIC_H_

#define F_CPU           16000000UL

#define TICKS_TO_CM     17150UL

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS PROTOTYPES
 *********************************************************************************************************************/

/******************************************************************************
 * \Syntax          : static void Delay_In_Us(u32 us)
 * \Description     : Software Delay in micro-second
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : us Delay in us
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
static void Delay_In_Us(u32 us);

/******************************************************************************
 * \Syntax          : void Get_Distance(void)
 * \Description     : Called-back function implements the logic and calculations
 *                          to get the distance
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Get_Distance(void);

/**********************************************************************************************************************
 *  Global FUNCTIONS PROTOTYPES
 *********************************************************************************************************************/

/******************************************************************************
 * \Syntax          : void Ultra_Sonic_init(void)
 * \Description     : Initialize the Ultrasonic Module
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Ultra_Sonic_init(void);

/*************************************************************************************************
 * \Syntax          : void ultrasonic_distance(u64*distance_local,void (*ptr_func_local)(void))
 * \Description     : Get the distance
 *
 * \Sync\Async      : Asynchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : distance_local    Pointer to hold The Distance on it
 *                    ptr_func_local    Notification Function indicates the Distance getting
 * \Parameters (out): None
 * \Return value:   : void
 *************************************************************************************************/
void ultrasonic_distance(u64*distance_local,void (*ptr_func_local)(void));

#endif /* HAL_ULTRA_SONIC_H_ */
