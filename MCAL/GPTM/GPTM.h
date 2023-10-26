/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Mahmoud Badr
 *	   	   File:  GPTM_interface.h
 *		  Layer:  MCAL
 *       Module:  GPTM
 *		Version:  1.00
 *	
 *  Description:  -     
 *  
 *********************************************************************************************************************/
#ifndef GPTM_INTERFACE_H
#define GPTM_INTERFACE_H

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef enum
{
    GPTM_TIMER0=0,
    GPTM_TIMER1,
    GPTM_TIMER2,
    GPTM_TIMER3,
    GPTM_TIMER4,
    GPTM_TIMER5,
    GPTM_WTIMER0,
    GPTM_WTIMER1,
    GPTM_WTIMER2,
    GPTM_WTIMER3,
    GPTM_WTIMER4,
    GPTM_WTIMER5
}GPTM_BlockNum_t;

typedef enum
{
    GPTM_CONC=0,
    GPTM_INDIVIDUAL=4
}GPTM_Conc_t;

typedef enum
{
    GPTM_TIMERA=0,
    GPTM_TIMERB
}GPTM_Channel_t;
 
typedef enum
{
    GPTM_ONE_SHOT=1,
    GPTM_PERIODIC,
    GPTM_CAPUTURE
}GPTM_ConState_t;

typedef enum
{
    GPTM_MODE_NORMAL=0,
    GPTM_MODE_PWM,
    GPTM_MODE_RTC,
    GPTM_MODE_EDGE_COUNT,
    GPTM_MODE_EDGE_TIME
}GPTM_Mode_t;

typedef enum
{
    GPTM_COUNT_DOWN=0,
    GPTM_COUNT_UP
}GPTM_Count_t;

typedef enum
{
    GPTM_STOP=0,
    GPTM_START
}GPTM_State;

typedef enum
{
    GPTM_TIMEOUT_INT=0,
    GPTM_CAPUTURE_INT,
    GPTM_CAPUTURE_EVENT_INT,
    GPTM_MATCH_INT
}GPTM_Int_t;


typedef enum
{
    GPTM_EVENT_POSITIVE_EDGE=0,
    GPTM_EVENT_NEGATIVE_EDGE=1,
    GPTM_EVENT_BOTH_EDGES=3
}GPTM_EventTrigger_t;

typedef enum
{
    GPT_PWM_UNAFFECTED=0,
    GPT_PWM_INVERTED
}GPTM_PWMOutLevel_t;

typedef enum
{
    GPTM_INT_DISABLE=0,
    GPTM_INT_ENABLE
}GPTM_IntState_t;

typedef u16     Prescaler_t;

typedef u8     TimersNum_t;

typedef u64     TimerValue_t;

typedef struct
{
    GPTM_BlockNum_t TimerBlock;
    GPTM_Conc_t TimerConc;
    GPTM_Channel_t TimerChannel;
    GPTM_ConState_t TimerConState;
    GPTM_Mode_t TimerMode;
    GPTM_Count_t CountUpDown;
    Prescaler_t Prescaler;
}GPTM_Config_t;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**************************************************************************************************
 * \Syntax          : ErrorState_t GPTM_Init(GPTM_Config_t* Copy_Config, TimersNum_t Copy_Number)
 * \Description     : Initialize the required Timers Channels
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config     GPTM configuration structure
 *                    Copy_Number     Channels Number
 * \Parameters (out): None
 * \Return value:   : ErrorState_t  E_OK
 ***************************************************************************************************/
ErrorState_t GPTM_Init(GPTM_Config_t* Copy_Config, TimersNum_t Copy_Number);

/*****************************************************************************************************************************************
 * \Syntax          : ErrorState_t GPTM_SetState(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel,GPTM_State Copy_State)
 * \Description     : Set the timer state start or stop
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_TimerNum         Timer Block Number
 *                    Copy_TimerChannel     Timer Channel Number
 *                    Copy_State            State of timer
 * \Parameters (out): None
 * \Return value:   : ErrorState_t  E_OK
 *****************************************************************************************************************************************/
ErrorState_t GPTM_SetState(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel,GPTM_State Copy_State);

/*************************************************************************************************************************************************************
 * \Syntax          : ErrorState_t GPTM_SetPWMOutputLevel(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel,GPTM_PWMOutLevel_t Copy_PWMOutput)
 * \Description     : Set the timer PWM mode output type
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_TimerNum         Timer Block Number
 *                    Copy_TimerChannel     Timer Channel Number
 *                    Copy_PWMOutput        PWM output type
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 *************************************************************************************************************************************************************/
ErrorState_t GPTM_SetPWMOutputLevel(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel,GPTM_PWMOutLevel_t Copy_PWMOutput);

/************************************************************************************************************************************************************
 * \Syntax          : ErrorState_t GPTM_SetEventTrigger(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel,GPTM_EventTrigger_t Copy_Trigger)
 * \Description     : Set the timer Event trigger
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_TimerNum         Timer Block Number
 *                    Copy_TimerChannel     Timer Channel Number
 *                    Copy_Trigger          Event trigger type
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 ************************************************************************************************************************************************************/
ErrorState_t GPTM_SetEventTrigger(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel,GPTM_EventTrigger_t Copy_Trigger);

/******************************************************************************************************************
 * \Syntax          : ErrorState_t GPTM_SetTimerLoadValue(GPTM_Config_t* Copy_Config, TimerValue_t Copy_Value)
 * \Description     : Set the load value of the required timer channel
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config     GPTM configuration structure
 *                    Copy_Value      The value to be set
 * \Parameters (out): None
 * \Return value:   : ErrorState_t  E_OK
 ******************************************************************************************************************/
ErrorState_t GPTM_SetTimerLoadValue(GPTM_Config_t* Copy_Config, TimerValue_t Copy_Value);

/******************************************************************************************************************
 * \Syntax          : ErrorState_t GPTM_SetTimerValue(GPTM_Config_t* Copy_Config, TimerValue_t Copy_Value)
 * \Description     : Set the value of the required timer channel
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config     GPTM configuration structure
 *                    Copy_Value      The value to be set
 * \Parameters (out): None
 * \Return value:   : ErrorState_t  E_OK
 ******************************************************************************************************************/
ErrorState_t GPTM_SetTimerValue(GPTM_Config_t* Copy_Config, TimerValue_t Copy_Value);

/***********************************************************************************************************
 * \Syntax          : ErrorState_t GPTM_SetMatchValue(GPTM_Config_t* Copy_Config, TimerValue_t Copy_Value)
 * \Description     : Load the top value of timer
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config           GPTM configuration structure
 *                    Copy_Value            Value to be loaded in Timer Register
 *                    Copy_MatchPrescaler   Prescaler of match counter
 * \Parameters (out): None
 * \Return value:   : ErrorState_t  E_OK
 ***********************************************************************************************************/
ErrorState_t GPTM_SetMatchValue(GPTM_Config_t* Copy_Config, TimerValue_t Copy_Value,u16 Copy_MatchPrescaler);

/***********************************************************************************************************
 * \Syntax          : ErrorState_t GPTM_GetTimerValue(GPTM_Config_t* Copy_Config, TimerValue_t* Copy_Value)
 * \Description     : Get the current value of Timer
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config     GPTM configuration structure
 * \Parameters (out): Copy_Value      Current Value of Timer
 * \Return value:   : ErrorState_t
 **********************************************************************************************************/
ErrorState_t GPTM_GetTimerValue(GPTM_Config_t* Copy_Config, TimerValue_t* Copy_Value);

/********************************************************************************************************************************************************************************
 * \Syntax          : ErrorState_t GPTM_SetInterruptState(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel, GPTM_Int_t Copy_IntType,GPTM_IntState_t Copy_IntState)
 * \Description     : Set the timer interrupt state
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_TimerNum         Timer Block Number
 *                    Copy_TimerChannel     Timer Channel Number
 *                    Copy_IntType          Interrupt to set the callback function
 *                    Copy_IntState         Interrupt state to be set
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 ********************************************************************************************************************************************************************************/
ErrorState_t GPTM_SetInterruptState(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel, GPTM_Int_t Copy_IntType,GPTM_IntState_t Copy_IntState);

/*****************************************************************************************************************************************************************************
 * \Syntax          : ErrorState_t GPTM_SetCallBack(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel,GPTM_Int_t Copy_IntType,void(*Copy_CallBackFunc)(void))
 * \Description     : Set the call back to a specific mode in a specific channel in a specific Timer block
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_TimerNum         Timer Block Number
 *                    Copy_TimerChannel     Timer Channel Number
 *                    Copy_IntType          Interrupt to set the callback function
 *                    Copy_CallBackFunc     Pointer to called-back function
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 *****************************************************************************************************************************************************************************/
ErrorState_t GPTM_SetCallBack(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel,GPTM_Int_t Copy_IntType,void(*Copy_CallBackFunc)(void));

/**********************************************************************************************************************
 *  LOCAL MACROS CONSTANT\FUNCTION
 *********************************************************************************************************************/
#define GPTMS_NUM           12

#define GPTM_TACDIR         4

#define GPTM_TAEN           0

#define GPTM_TBAMS          3

#define GPTM_TBCMR          2

#define GPTM_TBMR           1

#define GPTM_CHANNELS       2

#define GPTM_INT_SOURCES    4

#define GPTMN_CONFIG        Copy_Config[Local_Counter]

#define GPTMN               GPTM_Arr[GPTMN_CONFIG.TimerBlock]

#define GPTMN_CHANNEL       GPTMN_CONFIG.TimerChannel

#define GPTM_ENABLE         (Copy_TimerChannel*8)

#define GPTM_SET_EVENT      (Copy_TimerChannel*8+2)

#define GPTM_SET_PWM        (Copy_TimerChannel*8+6)

#endif  /* GPTM_INTERFACE_H */

/**********************************************************************************************************************
 *  END OF FILE: GPTM_interface.h
 *********************************************************************************************************************/
