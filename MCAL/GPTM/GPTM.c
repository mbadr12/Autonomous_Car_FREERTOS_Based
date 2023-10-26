/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Mahmoud Badr
 *	   	   File:  GPTM_Program.c
 *		  Layer:  MCAL
 *       Module:  GPTM
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

#include "GPTM.h"

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
static GPTM_RegDef_t* GPTM_Arr[GPTMS_NUM]={GPTM0,GPTM1,GPTM2,GPTM3,GPTM4,GPTM5,WGPTM0,WGPTM1,WGPTM2,WGPTM3,WGPTM4,WGPTM5};

static void(*GPTM_CallBackArr[GPTMS_NUM][GPTM_CHANNELS][GPTM_INT_SOURCES])(void)={NULL};

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
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
ErrorState_t GPTM_Init(GPTM_Config_t* Copy_Config, TimersNum_t Copy_Number)
{
    ErrorState_t Local_ErrorState=E_OK;
    TimersNum_t Local_Counter=0UL;
    if(Copy_Config==NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        for(Local_Counter=0UL;Local_Counter<Copy_Number;Local_Counter++)
        {
            if((GPTMN_CONFIG.TimerBlock>GPTM_WTIMER5) || (GPTMN_CONFIG.TimerChannel>GPTM_TIMERB))
            {
                Local_ErrorState=E_WRONG_OPTION;
            }
            else
            {
                /*Make sure that our Timer is Disabled*/
                if(GET_BIT((GPTMN->CTL),((GPTMN_CONFIG.TimerChannel)*8))!=0)
                {
                    CLR_BIT((GPTMN->CTL),GPTM_TAEN);
                }
                /*Set the concatenation mode*/
                GPTMN->CFG = GPTMN_CONFIG.TimerConc;
                /*set the count direction*/
                INSERT_BIT((GPTMN->TxMR[GPTMN_CHANNEL]),GPTM_TACDIR,GPTMN_CONFIG.CountUpDown);
                /*Set the Timer concatenation state*/
                GPTMN->TxMR[GPTMN_CHANNEL] &= ~(0b11);
                GPTMN->TxMR[GPTMN_CHANNEL] |= GPTMN_CONFIG.TimerConState;
                /*set the Timer mode*/
                switch(GPTMN_CONFIG.TimerMode)
                {
                case GPTM_MODE_PWM: SET_BIT((GPTMN->TxMR[GPTMN_CHANNEL]),GPTM_TBAMS); CLR_BIT((GPTMN->TxMR[GPTMN_CHANNEL]),GPTM_TBCMR); break;
                case GPTM_MODE_EDGE_TIME: SET_BIT((GPTMN->TxMR[GPTMN_CHANNEL]),GPTM_TBCMR); CLR_BIT((GPTMN->TxMR[GPTMN_CHANNEL]),GPTM_TBAMS); break;
                case GPTM_MODE_EDGE_COUNT: CLR_BIT((GPTMN->TxMR[GPTMN_CHANNEL]),GPTM_TBAMS); CLR_BIT((GPTMN->TxMR[GPTMN_CHANNEL]),GPTM_TBCMR); break;
                }
                /*Set Prescaler*/
                GPTMN->TxPR[GPTMN_CONFIG.TimerChannel]=(GPTMN_CONFIG.Prescaler)-1;
            }
        }
    }
    SET_BIT((GPTM_Arr[GPTM_WTIMER5]->CTL),1);
    return Local_ErrorState;
}

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
ErrorState_t GPTM_SetState(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel,GPTM_State Copy_State)
{
    ErrorState_t Local_ErrorState=E_OK;
    if((Copy_TimerNum>GPTM_WTIMER5) || (Copy_TimerChannel>GPTM_TIMERB))
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        INSERT_BIT((GPTM_Arr[Copy_TimerNum]->CTL),GPTM_ENABLE,Copy_State);
    }
    return Local_ErrorState;
}

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
ErrorState_t GPTM_SetPWMOutputLevel(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel, GPTM_PWMOutLevel_t Copy_PWMOutput)
{
    ErrorState_t Local_ErrorState=E_OK;
    if((Copy_TimerNum>GPTM_WTIMER5) || (Copy_TimerChannel>GPTM_TIMERB))
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        INSERT_BIT((GPTM_Arr[Copy_TimerNum]->CFG),GPTM_SET_PWM,Copy_PWMOutput);
    }
    return Local_ErrorState;
}

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
ErrorState_t GPTM_SetEventTrigger(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel,GPTM_EventTrigger_t Copy_Trigger)
{
    ErrorState_t Local_ErrorState=E_OK;
    if((Copy_TimerNum>GPTM_WTIMER5) || (Copy_TimerChannel>GPTM_TIMERB))
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        GPTM_Arr[Copy_TimerNum]->CTL &= ~(0b11<<GPTM_SET_EVENT);
        GPTM_Arr[Copy_TimerNum]->CTL |= Copy_Trigger<<GPTM_SET_EVENT;
    }
    return Local_ErrorState;
}

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
ErrorState_t GPTM_SetTimerLoadValue(GPTM_Config_t* Copy_Config, TimerValue_t Copy_Value)
{
    ErrorState_t Local_ErrorState=E_OK;
    if(Copy_Config == NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        if((Copy_Config->TimerBlock>GPTM_WTIMER5) || (Copy_Config->TimerChannel>GPTM_TIMERB))
        {
            Local_ErrorState=E_WRONG_OPTION;
        }
        else
        {
            if(Copy_Config->TimerConc == GPTM_CONC)
            {
                GPTM_Arr[Copy_Config->TimerBlock]->xILR[GPTM_TIMERA]=(u32)Copy_Value;
                if(Copy_Config->TimerBlock > GPTM_TIMER5)
                {
                    /*set the 32-bit of COMPMATCH B in case of 64-bit concatenation*/
                    GPTM_Arr[Copy_Config->TimerBlock]->xILR[GPTM_TIMERB]=(u32)(Copy_Value>>32);
                }
                else
                {
                    /* MISRA */
                }
            }
            else
            {
                GPTM_Arr[Copy_Config->TimerBlock]->xILR[Copy_Config->TimerChannel]=(u32)Copy_Value;
            }
        }
    }
    return Local_ErrorState;
}

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
ErrorState_t GPTM_SetTimerValue(GPTM_Config_t* Copy_Config, TimerValue_t Copy_Value)
{
    ErrorState_t Local_ErrorState=E_OK;
    if(Copy_Config == NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        if((Copy_Config->TimerBlock>GPTM_WTIMER5) || (Copy_Config->TimerChannel>GPTM_TIMERB))
        {
            Local_ErrorState=E_WRONG_OPTION;
        }
        else
        {
            if(Copy_Config->TimerConc == GPTM_CONC)
            {
                GPTM_Arr[Copy_Config->TimerBlock]->TxV[GPTM_TIMERA]=(u32)Copy_Value;
                if(Copy_Config->TimerBlock > GPTM_TIMER5)
                {
                    /*set the 32-bit of COMPMATCH B in case of 64-bit concatenation*/
                    GPTM_Arr[Copy_Config->TimerBlock]->TxV[GPTM_TIMERB]=(u32)(Copy_Value>>32);
                }
                else
                {
                    /* MISRA */
                }
            }
            else
            {
                GPTM_Arr[Copy_Config->TimerBlock]->TxV[Copy_Config->TimerChannel]=(u32)Copy_Value;
            }
        }
    }
    return Local_ErrorState;
}

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
ErrorState_t GPTM_SetMatchValue(GPTM_Config_t* Copy_Config, TimerValue_t Copy_Value,u16 Copy_MatchPrescaler)
{
    ErrorState_t Local_ErrorState=E_OK;
    if(Copy_Config == NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        if((Copy_Config->TimerBlock>GPTM_WTIMER5) || (Copy_Config->TimerChannel>GPTM_TIMERB))
        {
            Local_ErrorState=E_WRONG_OPTION;
        }
        else
        {
            if(Copy_Config->TimerConc == GPTM_CONC)
            {
                GPTM_Arr[Copy_Config->TimerBlock]->xMATCHR[GPTM_TIMERA]=(u32)Copy_Value;
                if(Copy_Config->TimerBlock > GPTM_TIMER5)
                {
                    /*set the 32-bit of COMPMATCH B in case of 64-bit concatenation*/
                    GPTM_Arr[Copy_Config->TimerBlock]->xMATCHR[GPTM_TIMERB]=(u32)(Copy_Value>>32);
                }
                else
                {
                    /* MISRA */
                }
            }
            else
            {
                GPTM_Arr[Copy_Config->TimerBlock]->xMATCHR[Copy_Config->TimerChannel]=(u32)Copy_Value;
                GPTM_Arr[Copy_Config->TimerBlock]->TxPMR[Copy_Config->TimerChannel]=Copy_MatchPrescaler;
            }
        }
    }
    return Local_ErrorState;
}

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
ErrorState_t GPTM_GetTimerValue(GPTM_Config_t* Copy_Config, TimerValue_t* Copy_Value)
{
    ErrorState_t Local_ErrorState=E_OK;
    if(Copy_Config == NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        if((Copy_Config->TimerBlock>GPTM_WTIMER5) || (Copy_Config->TimerChannel>GPTM_TIMERB))
        {
            Local_ErrorState=E_WRONG_OPTION;
        }
        else
        {
            if(Copy_Config->TimerConc == GPTM_CONC)
            {
                /*Get the 32-bit from */
                *Copy_Value=(u32)GPTM_Arr[Copy_Config->TimerBlock]->TxR[GPTM_TIMERA];
                if(Copy_Config->TimerBlock > GPTM_TIMER5)
                {
                    /*set the 32-bit of COMPMATCH B in case of 64-bit concatenation*/
                    *Copy_Value |= (u64)((u64)(GPTM_Arr[Copy_Config->TimerBlock]->TxV[GPTM_TIMERB])<<32);
                }
                else
                {
                    /* MISRA */
                }
            }
            else
            {
                *Copy_Value=(u32)GPTM_Arr[Copy_Config->TimerBlock]->TxR[Copy_Config->TimerChannel];
            }
            if(Copy_Config->CountUpDown==GPTM_COUNT_UP)
            {
                *Copy_Value = GPTM_Arr[Copy_Config->TimerBlock]->TxV[Copy_Config->TimerChannel]/Copy_Config->Prescaler;
            }
            else
            {
                /* MISRA */
            }
        }
    }
    return Local_ErrorState;
}

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
ErrorState_t GPTM_SetInterruptState(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel, GPTM_Int_t Copy_IntType,GPTM_IntState_t Copy_IntState)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_IntBit=Copy_TimerChannel*8+Copy_IntType;
    if((Copy_TimerNum>GPTM_WTIMER5) || (Copy_TimerChannel>GPTM_TIMERB))
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        if((Copy_TimerChannel==GPTM_TIMERA)&&(Copy_IntType==GPTM_MATCH_INT))
        {
            Local_IntBit++;
        }
        INSERT_BIT(GPTM_Arr[Copy_TimerNum]->IMR,Local_IntBit,Copy_IntState);
    }
    return Local_ErrorState;
}

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
ErrorState_t GPTM_SetCallBack(GPTM_BlockNum_t Copy_TimerNum, GPTM_Channel_t Copy_TimerChannel,GPTM_Int_t Copy_IntType,void(*Copy_CallBackFunc)(void))
{
    ErrorState_t Local_ErrorState=E_OK;
    if(Copy_CallBackFunc==NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        if((Copy_TimerNum>GPTM_WTIMER5) || (Copy_TimerChannel>GPTM_TIMERB))
        {
            Local_ErrorState=E_WRONG_OPTION;
        }
        else
        {
            GPTM_CallBackArr[Copy_TimerNum][Copy_TimerChannel][Copy_IntType]=Copy_CallBackFunc;
        }
    }
    return Local_ErrorState;
}

/**************************************************************************************************
 * \Syntax          : void TIMER0A_Handler(void)
 * \Description     : Timer0 channelA Handler
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 ***************************************************************************************************/
void TIMER0A_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=0UL;Local_Counter<3UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_TIMER0]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_TIMER0][GPTM_TIMERA][Local_Counter]!=NULL)
            {
                GPTM_CallBackArr[GPTM_TIMER0][GPTM_TIMERA][Local_Counter]();
            }
            GPTM_Arr[GPTM_TIMER0]->ICR=1<<Local_Counter;
        }
    }
    if(GET_BIT((GPTM_Arr[GPTM_TIMER0]->RIS),(Local_Counter+1))==1)
    {
        if(GPTM_CallBackArr[GPTM_TIMER0][GPTM_TIMERA][GPTM_MATCH_INT]!=NULL)
        {
            GPTM_CallBackArr[GPTM_TIMER0][GPTM_TIMERA][Local_Counter]();
        }
        GPTM_Arr[GPTM_TIMER0]->ICR=1<<Local_Counter;
    }
}

/**************************************************************************************************
 * \Syntax          : void TIMER0B_Handler(void)
 * \Description     : Timer0 channelB Handler
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 ***************************************************************************************************/
void TIMER0B_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=8UL;Local_Counter<12UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_TIMER0]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_TIMER0][GPTM_TIMERB][(Local_Counter-8)]!=NULL)
            {
                GPTM_CallBackArr[GPTM_TIMER0][GPTM_TIMERB][(Local_Counter-8)]();
            }
            GPTM_Arr[GPTM_TIMER0]->ICR=1<<Local_Counter;
        }
    }
}

/**************************************************************************************************
 * \Syntax          : void TIMER1A_Handler(void)
 * \Description     : Timer1 channelA Handler
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 ***************************************************************************************************/
void TIMER1A_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=0UL;Local_Counter<3UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_TIMER1]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_TIMER1][GPTM_TIMERA][Local_Counter]!=NULL)
            {
                GPTM_CallBackArr[GPTM_TIMER1][GPTM_TIMERA][Local_Counter]();
            }
            GPTM_Arr[GPTM_TIMER1]->ICR=1<<Local_Counter;
        }
    }
    if(GET_BIT((GPTM_Arr[GPTM_TIMER1]->RIS),(Local_Counter+1))==1)
    {
        if(GPTM_CallBackArr[GPTM_TIMER1][GPTM_TIMERA][GPTM_MATCH_INT]!=NULL)
        {
            GPTM_CallBackArr[GPTM_TIMER1][GPTM_TIMERA][Local_Counter]();
        }
        GPTM_Arr[GPTM_TIMER1]->ICR=1<<Local_Counter;
    }
}

/**************************************************************************************************
 * \Syntax          : void TIMER1B_Handler(void)
 * \Description     : Timer1 channelB Handler
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 ***************************************************************************************************/
void TIMER1B_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=8UL;Local_Counter<12UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_TIMER1]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_TIMER1][GPTM_TIMERB][(Local_Counter-8)]!=NULL)
            {
                GPTM_CallBackArr[GPTM_TIMER1][GPTM_TIMERB][(Local_Counter-8)]();
            }
            GPTM_Arr[GPTM_TIMER1]->ICR=1<<Local_Counter;
        }
    }
}

/**************************************************************************************************
 * \Syntax          : void TIMER2A_Handler(void)
 * \Description     : Timer2 channelA Handler
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 ***************************************************************************************************/
void TIMER2A_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=0UL;Local_Counter<3UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_TIMER2]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_TIMER2][GPTM_TIMERA][Local_Counter]!=NULL)
            {
                GPTM_CallBackArr[GPTM_TIMER2][GPTM_TIMERA][Local_Counter]();
            }
            GPTM_Arr[GPTM_TIMER2]->ICR=1<<Local_Counter;
        }
    }
    if(GET_BIT((GPTM_Arr[GPTM_TIMER2]->RIS),(Local_Counter+1))==1)
    {
        if(GPTM_CallBackArr[GPTM_TIMER2][GPTM_TIMERA][GPTM_MATCH_INT]!=NULL)
        {
            GPTM_CallBackArr[GPTM_TIMER2][GPTM_TIMERA][Local_Counter]();
        }
        GPTM_Arr[GPTM_TIMER2]->ICR=1<<Local_Counter;
    }
}

/**************************************************************************************************
 * \Syntax          : void TIMER2B_Handler(void)
 * \Description     : Timer2 channelB Handler
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 ***************************************************************************************************/
void TIMER2B_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=8UL;Local_Counter<12UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_TIMER2]->RIS),Local_Counter)==1UL)
        {
            if(GPTM_CallBackArr[GPTM_TIMER2][GPTM_TIMERB][(Local_Counter-8UL)]!=NULL)
            {
                GPTM_CallBackArr[GPTM_TIMER2][GPTM_TIMERB][(Local_Counter-8UL)]();
            }
            GPTM_Arr[GPTM_TIMER2]->ICR=1<<Local_Counter;
        }
    }
}

/**************************************************************************************************
 * \Syntax          : void TIMER3A_Handler(void)
 * \Description     : Timer3 channelA Handler
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 ***************************************************************************************************/
void TIMER3A_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=0UL;Local_Counter<3UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_TIMER3]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_TIMER3][GPTM_TIMERA][Local_Counter]!=NULL)
            {
                GPTM_CallBackArr[GPTM_TIMER3][GPTM_TIMERA][Local_Counter]();
            }
            GPTM_Arr[GPTM_TIMER3]->ICR=1<<Local_Counter;
        }
    }
    if(GET_BIT((GPTM_Arr[GPTM_TIMER3]->RIS),(Local_Counter+1))==1)
    {
        if(GPTM_CallBackArr[GPTM_TIMER3][GPTM_TIMERA][GPTM_MATCH_INT]!=NULL)
        {
            GPTM_CallBackArr[GPTM_TIMER3][GPTM_TIMERA][Local_Counter]();
        }
        GPTM_Arr[GPTM_TIMER3]->ICR=1<<Local_Counter;
    }
}

/**************************************************************************************************
 * \Syntax          : void TIMER3B_Handler(void)
 * \Description     : Timer3 channelB Handler
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 ***************************************************************************************************/
void TIMER3B_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=8UL;Local_Counter<12UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_TIMER3]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_TIMER3][GPTM_TIMERB][(Local_Counter-8)]!=NULL)
            {
                GPTM_CallBackArr[GPTM_TIMER3][GPTM_TIMERB][(Local_Counter-8)]();
            }
            GPTM_Arr[GPTM_TIMER3]->ICR=1<<Local_Counter;
        }
    }
}

/**************************************************************************************************
 * \Syntax          : void TIMER4A_Handler(void)
 * \Description     : Timer4 channelA Handler
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 ***************************************************************************************************/
void TIMER4A_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=0UL;Local_Counter<3UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_TIMER4]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_TIMER4][GPTM_TIMERA][Local_Counter]!=NULL)
            {
                GPTM_CallBackArr[GPTM_TIMER4][GPTM_TIMERA][Local_Counter]();
            }
            GPTM_Arr[GPTM_TIMER4]->ICR=1<<Local_Counter;
        }
    }
    if(GET_BIT((GPTM_Arr[GPTM_TIMER4]->RIS),(Local_Counter+1))==1)
    {
        if(GPTM_CallBackArr[GPTM_TIMER4][GPTM_TIMERA][GPTM_MATCH_INT]!=NULL)
        {
            GPTM_CallBackArr[GPTM_TIMER4][GPTM_TIMERA][Local_Counter]();
        }
        GPTM_Arr[GPTM_TIMER4]->ICR=1<<Local_Counter;
    }
}

/**************************************************************************************************
 * \Syntax          : void TIMER4B_Handler(void)
 * \Description     : Timer4 channelB Handler
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 ***************************************************************************************************/
void TIMER4B_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=8UL;Local_Counter<12UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_TIMER4]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_TIMER4][GPTM_TIMERB][(Local_Counter-8)]!=NULL)
            {
                GPTM_CallBackArr[GPTM_TIMER4][GPTM_TIMERB][(Local_Counter-8)]();
            }
            GPTM_Arr[GPTM_TIMER4]->ICR=1<<Local_Counter;
        }
    }
}

/**************************************************************************************************
 * \Syntax          : void TIMER5A_Handler(void)
 * \Description     : Timer5 channelA Handler
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 ***************************************************************************************************/
void TIMER5A_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=0UL;Local_Counter<3UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_TIMER5]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_TIMER5][GPTM_TIMERA][Local_Counter]!=NULL)
            {
                GPTM_CallBackArr[GPTM_TIMER5][GPTM_TIMERA][Local_Counter]();
            }
            GPTM_Arr[GPTM_TIMER5]->ICR=1<<Local_Counter;
        }
    }
    if(GET_BIT((GPTM_Arr[GPTM_TIMER5]->RIS),(Local_Counter+1))==1)
    {
        if(GPTM_CallBackArr[GPTM_TIMER5][GPTM_TIMERA][GPTM_MATCH_INT]!=NULL)
        {
            GPTM_CallBackArr[GPTM_TIMER5][GPTM_TIMERA][Local_Counter]();
        }
        GPTM_Arr[GPTM_TIMER5]->ICR=1<<Local_Counter;
    }
}

/**************************************************************************************************
 * \Syntax          : void TIMER5B_Handler(void)
 * \Description     : Timer5 channelB Handler
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 ***************************************************************************************************/
void TIMER5B_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=8UL;Local_Counter<12UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_TIMER5]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_TIMER5][GPTM_TIMERB][(Local_Counter-8)]!=NULL)
            {
                GPTM_CallBackArr[GPTM_TIMER5][GPTM_TIMERB][(Local_Counter-8)]();
            }
            GPTM_Arr[GPTM_TIMER5]->ICR=1<<Local_Counter;
        }
    }
}

void WTIMER0A_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=0UL;Local_Counter<3UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_WTIMER0]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_WTIMER0][GPTM_TIMERA][Local_Counter]!=NULL)
            {
                GPTM_CallBackArr[GPTM_WTIMER0][GPTM_TIMERA][Local_Counter]();
            }
            GPTM_Arr[GPTM_WTIMER0]->ICR=1<<Local_Counter;
        }
    }
    if(GET_BIT((GPTM_Arr[GPTM_WTIMER0]->RIS),(Local_Counter+1))==1)
    {
        if(GPTM_CallBackArr[GPTM_WTIMER0][GPTM_TIMERA][GPTM_MATCH_INT]!=NULL)
        {
            GPTM_CallBackArr[GPTM_WTIMER0][GPTM_TIMERA][Local_Counter]();
        }
        GPTM_Arr[GPTM_WTIMER0]->ICR=1<<Local_Counter;
    }
}

void WTIMER0B_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=8UL;Local_Counter<12UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_WTIMER0]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_WTIMER0][GPTM_TIMERB][(Local_Counter-8)]!=NULL)
            {
                GPTM_CallBackArr[GPTM_WTIMER0][GPTM_TIMERB][(Local_Counter-8)]();
            }
            GPTM_Arr[GPTM_WTIMER0]->ICR=1<<Local_Counter;
        }
    }
}

void WTIMER1A_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=0UL;Local_Counter<3UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_WTIMER1]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_WTIMER1][GPTM_TIMERA][Local_Counter]!=NULL)
            {
                GPTM_CallBackArr[GPTM_WTIMER1][GPTM_TIMERA][Local_Counter]();
            }
            GPTM_Arr[GPTM_WTIMER1]->ICR=1<<Local_Counter;
        }
    }
    if(GET_BIT((GPTM_Arr[GPTM_WTIMER1]->RIS),(Local_Counter+1))==1)
    {
        if(GPTM_CallBackArr[GPTM_WTIMER1][GPTM_TIMERA][GPTM_MATCH_INT]!=NULL)
        {
            GPTM_CallBackArr[GPTM_WTIMER1][GPTM_TIMERA][Local_Counter]();
        }
        GPTM_Arr[GPTM_WTIMER1]->ICR=1<<Local_Counter;
    }
}

void WTIMER1B_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=8UL;Local_Counter<12UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_WTIMER1]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_WTIMER1][GPTM_TIMERB][(Local_Counter-8)]!=NULL)
            {
                GPTM_CallBackArr[GPTM_WTIMER1][GPTM_TIMERB][(Local_Counter-8)]();
            }
            GPTM_Arr[GPTM_WTIMER1]->ICR=1<<Local_Counter;
        }
    }
}

void WTIMER2A_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=0UL;Local_Counter<3UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_WTIMER2]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_WTIMER2][GPTM_TIMERA][Local_Counter]!=NULL)
            {
                GPTM_CallBackArr[GPTM_WTIMER2][GPTM_TIMERA][Local_Counter]();
            }
            GPTM_Arr[GPTM_WTIMER2]->ICR=1<<Local_Counter;
        }
    }
    if(GET_BIT((GPTM_Arr[GPTM_WTIMER2]->RIS),(Local_Counter+1))==1)
    {
        if(GPTM_CallBackArr[GPTM_WTIMER2][GPTM_TIMERA][GPTM_MATCH_INT]!=NULL)
        {
            GPTM_CallBackArr[GPTM_WTIMER2][GPTM_TIMERA][Local_Counter]();
        }
        GPTM_Arr[GPTM_WTIMER2]->ICR=1<<Local_Counter;
    }
}

void WTIMER2B_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=8UL;Local_Counter<12UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_WTIMER2]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_WTIMER2][GPTM_TIMERB][(Local_Counter-8)]!=NULL)
            {
                GPTM_CallBackArr[GPTM_WTIMER2][GPTM_TIMERB][(Local_Counter-8)]();
            }
            GPTM_Arr[GPTM_WTIMER2]->ICR=1<<Local_Counter;
        }
    }
}

void WTIMER3A_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=0UL;Local_Counter<3UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_WTIMER3]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_WTIMER3][GPTM_TIMERA][Local_Counter]!=NULL)
            {
                GPTM_CallBackArr[GPTM_WTIMER3][GPTM_TIMERA][Local_Counter]();
            }
            GPTM_Arr[GPTM_WTIMER3]->ICR=1<<Local_Counter;
        }
    }
    if(GET_BIT((GPTM_Arr[GPTM_WTIMER3]->RIS),(Local_Counter+1))==1)
    {
        if(GPTM_CallBackArr[GPTM_WTIMER3][GPTM_TIMERA][GPTM_MATCH_INT]!=NULL)
        {
            GPTM_CallBackArr[GPTM_WTIMER3][GPTM_TIMERA][Local_Counter]();
        }
        GPTM_Arr[GPTM_WTIMER3]->ICR=1<<Local_Counter;
    }
}

void WTIMER3B_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=8UL;Local_Counter<12UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_WTIMER3]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_WTIMER3][GPTM_TIMERB][(Local_Counter-8)]!=NULL)
            {
                GPTM_CallBackArr[GPTM_WTIMER3][GPTM_TIMERB][(Local_Counter-8)]();
            }
            GPTM_Arr[GPTM_WTIMER3]->ICR=1<<Local_Counter;
        }
    }
}

void WTIMER4A_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=0UL;Local_Counter<3UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_WTIMER4]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_WTIMER4][GPTM_TIMERA][Local_Counter]!=NULL)
            {
                GPTM_CallBackArr[GPTM_WTIMER4][GPTM_TIMERA][Local_Counter]();
            }
            GPTM_Arr[GPTM_WTIMER4]->ICR=1<<Local_Counter;
        }
    }
    if(GET_BIT((GPTM_Arr[GPTM_WTIMER4]->RIS),(Local_Counter+1))==1)
    {
        if(GPTM_CallBackArr[GPTM_WTIMER4][GPTM_TIMERA][GPTM_MATCH_INT]!=NULL)
        {
            GPTM_CallBackArr[GPTM_WTIMER4][GPTM_TIMERA][Local_Counter]();
        }
        GPTM_Arr[GPTM_WTIMER4]->ICR=1<<Local_Counter;
    }
}

void WTIMER4B_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=8UL;Local_Counter<12UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_WTIMER4]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_WTIMER4][GPTM_TIMERB][(Local_Counter-8)]!=NULL)
            {
                GPTM_CallBackArr[GPTM_WTIMER4][GPTM_TIMERB][(Local_Counter-8)]();
            }
            GPTM_Arr[GPTM_WTIMER4]->ICR=1<<Local_Counter;
        }
    }
}

void WTIMER5A_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=0UL;Local_Counter<3UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_WTIMER5]->RIS),Local_Counter)==1UL)
        {
            if(GPTM_CallBackArr[GPTM_WTIMER5][GPTM_TIMERA][Local_Counter]!=NULL)
            {
                GPTM_CallBackArr[GPTM_WTIMER5][GPTM_TIMERA][Local_Counter]();
            }
            GPTM_Arr[GPTM_WTIMER5]->ICR=1<<Local_Counter;
        }
    }
    if(GET_BIT((GPTM_Arr[GPTM_WTIMER5]->RIS),(Local_Counter+1))==1)
    {
        if(GPTM_CallBackArr[GPTM_WTIMER5][GPTM_TIMERA][GPTM_MATCH_INT]!=NULL)
        {
            GPTM_CallBackArr[GPTM_WTIMER5][GPTM_TIMERA][Local_Counter]();
        }
        GPTM_Arr[GPTM_WTIMER5]->ICR=1<<Local_Counter;
    }
}

void WTIMER5B_Handler(void)
{
    u8 Local_Counter=0UL;
    for(Local_Counter=8UL;Local_Counter<12UL;Local_Counter++)
    {
        if(GET_BIT((GPTM_Arr[GPTM_WTIMER5]->RIS),Local_Counter)==1)
        {
            if(GPTM_CallBackArr[GPTM_WTIMER5][GPTM_TIMERB][(Local_Counter-8)]!=NULL)
            {
                GPTM_CallBackArr[GPTM_WTIMER5][GPTM_TIMERB][(Local_Counter-8)]();
            }
            GPTM_Arr[GPTM_WTIMER5]->ICR=1<<Local_Counter;
        }
    }
}

/**********************************************************************************************************************
 *  END OF FILE: GPTM_Program.c
 *********************************************************************************************************************/
