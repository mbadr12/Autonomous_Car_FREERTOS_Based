/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Hazem Magdy
 *         File:  Ultrasonic.c
 *        Layer:  HAL
 *       Module:  Ultrasonic
 *      Version:  1.00
 *
 *  Description:  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "../../LIB/Std_Types.h"
#include "../../MCAL/GPIO/GPIO.h"
#include "../../MCAL/GPTM/GPTM.h"
#include "../../MCAL/RCC/RCC.h"
#include "../../MCAL/NVIC/NVIC.h"
#include "Ultrasonic.h"

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
static void (*ptr_func_gobal)(void);
static u64* distance_global = NULL;
static GPTM_Config_t GPTM_config = {.TimerBlock = GPTM_WTIMER5,.TimerConc = GPTM_INDIVIDUAL,.TimerChannel = GPTM_TIMERA,
                                    .TimerConState = GPTM_CAPUTURE , .TimerMode = GPTM_MODE_EDGE_TIME , .CountUpDown = GPTM_COUNT_DOWN,.Prescaler = 0};

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
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
static void Delay_In_Us(u32 us)
{
    u32 count = (us * 3UL) / 10UL;  /*Adjust this value based on your system clock frequency*/
    while (count != 0UL) {
        count--;
    }
}

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
void Get_Distance(void)
{
    static u64 risingEdgeTime = 0;
    static u64 fallingEdgeTime = 0;
    u64 difference = 0;
    u64 time1 = 0;
    static u8 captureFlag = 0;
    if(captureFlag == 0UL)
    {
        GPTM_GetTimerValue(&GPTM_config,&risingEdgeTime);
        captureFlag=1UL;
    }
    else
    {
        GPTM_GetTimerValue(&GPTM_config,&fallingEdgeTime);
        difference = risingEdgeTime - fallingEdgeTime ;
        time1 = difference * TICKS_TO_CM;
        *distance_global = time1 / F_CPU;
        ptr_func_gobal();
        captureFlag = 0UL;
    }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
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
void Ultra_Sonic_init(void)
{

    RCC_SetPrephralClockState(RCC_GPIOE,RCC_CLK_ENABLE);
    RCC_SetPrephralClockState(RCC_GPIOD,RCC_CLK_ENABLE);
    RCC_SetPrephralClockState(RCC_wGPTM5,RCC_CLK_ENABLE);
    /*GPIO configuration*/
    GPIO_Config_t GPIO_config_E0 = {.Port = GPIO_PORTE , .Pin = GPIO_PIN0 , .Dir = GPIO_PIN_OUTPUT , .Mode = GPIO_PIN_DIGITAL,
                                    .PinState = GPIO_PIN_PUSHPULL,.PinPull = GPIO_PIN_FLOATING,.AltFuncNum = GPIO_ALTFUNC0};
    GPIO_Config_t GPIO_config_D6 = {.Port = GPIO_PORTD , .Pin = GPIO_PIN6 , .Dir = GPIO_PIN_INPUT , .Mode = GPIO_PIN_ALTFUNC,
                                    .PinState = GPIO_PIN_PUSHPULL , .PinPull = GPIO_PIN_FLOATING, .AltFuncNum = GPIO_ALTFUNC7};
    GPIO_Init(&GPIO_config_E0,1UL);
    GPIO_Init(&GPIO_config_D6,1UL);
    /*GPTM configuration*/
    GPTM_Init(&GPTM_config,1UL);
    GPTM_SetEventTrigger(GPTM_WTIMER5,GPTM_TIMERA,GPTM_EVENT_BOTH_EDGES);
    GPTM_SetTimerLoadValue(&GPTM_config,0xFFFFFFFF);
    GPTM_SetState(GPTM_WTIMER5,GPTM_TIMERA,GPTM_START);
    GPTM_SetCallBack(GPTM_WTIMER5,GPTM_TIMERA,GPTM_CAPUTURE_EVENT_INT,&Get_Distance);
    NVIC_SetInterruptState(NVIC_TIMER5A_32_64_IRQ, NVIC_INT_ENABLE);

}

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
void ultrasonic_distance(u64* distance_local,void (*ptr_func_local)(void))
{
    distance_global = distance_local;
    ptr_func_gobal = ptr_func_local ;
    GPTM_SetInterruptState(GPTM_WTIMER5,GPTM_TIMERA,GPTM_CAPUTURE_EVENT_INT,GPTM_INT_ENABLE);
    GPIO_SetPinValue(GPIO_PORTE, GPIO_PIN0, GPIO_PIN_HIGH);
    Delay_In_Us(10UL);
    GPIO_SetPinValue(GPIO_PORTE, GPIO_PIN0, GPIO_PIN_LOW);
}

/**********************************************************************************************************************
 *  END OF FILE: Ultrasonic.c
 *********************************************************************************************************************/
