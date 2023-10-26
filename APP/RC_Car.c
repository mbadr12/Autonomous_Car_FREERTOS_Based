/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Mahmoud Badr - Adham El-Srougy- Hazem Magdy - Omar Ayman
 *	   	   File:  RC_Car.c
 *		  Layer:  App
 *       Module:  Car Application
 *		Version:  1.00
 *	
 *  Description:  -     
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "../LIB/Std_Types.h"
#include "../LIB/Bit_Math.h"

#include "../HAL/LCD/LCD.h"
#include "../HAL/LDR/LDR.h"
#include "../HAL/Ultrasonic/Ultrasonic.h"
#include "../HAL/Temperature/Temperature.h"
#include "../HAL/Switch/Switch.h"
#include "../HAL/Motor/Motor.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "event_groups.h"
#include "RC_Car.h"

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
static u8 APP_LCDX=0,APP_LCDY=0;
static LCD_Config_t APP_LCD={.ControlPins={GPIO_PIN7,GPIO_PIN7,GPIO_PIN5},.ControlPinsPorts={GPIO_PORTB,GPIO_PORTF,GPIO_PORTB},.Cursor=LCD_NO_CURSOR,
                             .DataPins={GPIO_PIN0,GPIO_PIN1,GPIO_PIN2,GPIO_PIN3},.Font=LCD_5_8_FONT,.LinesNum=LCD_TWO_LINES,.Mode_t=LCD_4BIT_MODE,
                             .XPosition=&APP_LCDX,.Yposition=&APP_LCDY,.DataPinsPorts={GPIO_PORTB,GPIO_PORTB,GPIO_PORTB,GPIO_PORTB}};

u8 APP_CarTimer=0;

u64 Local_Distance=0;

xSemaphoreHandle APP_UtrasonicSem=NULL;

xSemaphoreHandle APP_LCDMutex=NULL;

EventGroupHandle_t APP_CarEvents=NULL;

xSemaphoreHandle APP_StartSem=NULL;

xQueueHandle APP_DistanceMailBox=NULL;

xQueueHandle APP_LDRDiffMailBox=NULL;

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/******************************************************************************
 * \Syntax          : void Ultrasonic_Notifaction(void)
 * \Description     : Notification Function of the ultrasonic reading completion
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Ultrasonic_Notifaction(void)
{
    BaseType_t Local_XTaskWoken=pdFALSE;
    xSemaphoreGiveFromISR(APP_UtrasonicSem,&Local_XTaskWoken);
}

/******************************************************************************
 * \Syntax          : void Switch1_Notification(void)
 * \Description     : Notification Function of the Pressing Switch 1
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Switch1_Notification(void)
{
    static BaseType_t Local_XHigerTaskWoken=pdFALSE;
    static EventBits_t Local_XEvents=0;
    Local_XEvents=xEventGroupGetBitsFromISR(APP_CarEvents);
    /*check if the other button Was pressed or not*/
    if((Local_XEvents&APP_SW1FLAG))
    {
        /* Do Nothing */
    }
    /*check if the other button was pressed or not*/
    else if((Local_XEvents&APP_SW2FLAG))
    {
        /*Lower all Flags*/
        xEventGroupClearBitsFromISR(APP_CarEvents, (APP_SW1FLAG|APP_SW2FLAG|APP_CAR_MOVING));
    }
    else
    {
        /*Raise the system start flag*/
        xSemaphoreGiveFromISR(APP_StartSem,&Local_XHigerTaskWoken);
        /*Raise flag once the button Pressed*/
        xEventGroupSetBitsFromISR(APP_CarEvents,APP_SW1FLAG ,&Local_XHigerTaskWoken);
    }
}

/******************************************************************************
 * \Syntax          : void Switch2_Notification(void)
 * \Description     : Notification Function of the Pressing Switch 2
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Switch2_Notification(void)
{
    static BaseType_t Local_HigerTaskWoken=pdFALSE;
    static EventBits_t Local_Events=0;
    Local_Events=xEventGroupGetBitsFromISR(APP_CarEvents);
    /*check if the other button Was pressed or not*/
    if((Local_Events&APP_SW2FLAG))
    {
        /* Do Nothing */
    }
    /*check if the other button was pressed or not*/
    else if((Local_Events&APP_SW1FLAG))
    {
        /*Lower all Flags*/
        xEventGroupClearBitsFromISR(APP_CarEvents, (APP_SW1FLAG|APP_SW2FLAG|APP_CAR_MOVING));
    }
    else
    {
        xSemaphoreGiveFromISR(APP_StartSem,&Local_HigerTaskWoken);
        /*Raise flag once the button Pressed*/
        xEventGroupSetBitsFromISR(APP_CarEvents,APP_SW2FLAG ,&Local_HigerTaskWoken);
    }
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/******************************************************************************
 * \Syntax          : void CAR_Init(void)
 * \Description     : Initialization of hardware and creation of tasks
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void CAR_Init(void)
{
    EXTI_Config_t Local_Switch1={.Port=EXTI_PORTF, .Pin=EXTI_PIN0,.TrigTye=EXTI_RAISING_EDGE};
    EXTI_Config_t Local_Switch2={.Port=EXTI_PORTF, .Pin=EXTI_PIN4,.TrigTye=EXTI_RAISING_EDGE};
    /*Initialize the system Hardware*/
    LCD_Init(&APP_LCD);
    Ultra_Sonic_init();
    LDR_Init();
    Temperature_Init();
    Motor_Init();
    Switch_Init(GPIO_PORTF, GPIO_PIN0, GPIO_PIN_PULLUP);
    Switch_Init(GPIO_PORTF, GPIO_PIN4, GPIO_PIN_PULLUP);
    LCD_SendString(&APP_LCD, "Temp: ");
    LCD_GoToXY(&APP_LCD, 8U, 0U);
    LCD_SendChar(&APP_LCD, 248U);
    LCD_SendChar(&APP_LCD, 'C');
    Switch_IntConfig(&Local_Switch1, &Switch1_Notification);
    Switch_IntConfig(&Local_Switch2, &Switch2_Notification);
    //ultrasonic_distance(&Local_Distance, &Ultrasonic_Notifaction);
    /*Create Event Group For car Events*/
    APP_CarEvents=xEventGroupCreate();
    /*Create a mailbox for the Distance*/
    APP_DistanceMailBox=xQueueCreate(1U,sizeof(u64));
    /*Create MailBox to hold LDR Difference*/
    APP_LDRDiffMailBox=xQueueCreate(1U,sizeof(u16));
    /*Create ultrasonic Semaphore and start Taken*/
    APP_UtrasonicSem=xSemaphoreCreateBinary();
    xSemaphoreGive(APP_UtrasonicSem);
    /*Create Start Semaphore and start Taken*/
    APP_StartSem=xSemaphoreCreateBinary();
    xSemaphoreTake(APP_StartSem,0U);
    /*Create LCD Mutex*/
    APP_LCDMutex=xSemaphoreCreateMutex();
    /*Create Utrasonic task to get reading*/
    xTaskCreate(&Ultrasonic_Task,"UltrasonicRead",200U, &Local_Distance, 6U, NULL);
    /*Create Avoid obstacles task to be executed */
    xTaskCreate(&avoid_obstacles,"ObstacleAvoid", 500U,NULL, 5U, NULL);
    /*Create Car start Task to be executed */
    xTaskCreate(&CarStart_Task,"MoveStart", 200U,NULL, 4U, NULL);
    /*Create Car Stop Task to be executed */
    xTaskCreate(&CarStop_Task,"MoveStop",200U, NULL, 3U, NULL);
    /*Create Watch Task to calcUate the elapsed time*/
    xTaskCreate(&Watch_Task,"TimerTask", 200U,NULL, 2U, NULL);
    /*Create LDR swing task to be executed*/
    xTaskCreate(&ldr_swing_car,"LightSwing",500U, NULL, 1U, NULL);
    /*Create Distance Display task to be Displayed*/
    xTaskCreate(&LCD_Distancedisplay,"LCDDistance",500U, NULL, 0U, NULL);
    /*Create LDR difference Display task to be Displayed*/
    xTaskCreate(&LCD_LDRDisplay,"LCDLight", 500U,NULL, 0U, NULL);
    /*Create Time elapsed Display task to be Displayed*/
    xTaskCreate(&LCD_TimeDisplay,"LCDTime",500U ,NULL, 0U, NULL);
    /*Create Temperature Task to be executed*/
    xTaskCreate(&Temperature_Task,"LCDTemp" ,500U ,NULL, 0U, NULL);
    /*Start schedUer*/
    vTaskStartScheduler();
}

/******************************************************************************
 * \Syntax          : void Ultrasonic_Task(void* Copy_UltraPar)
 * \Description     : Get the Utrasonic Reading
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Ultrasonic_Task(void* Copy_UltraPar)
{
	static u64 Local_Distance=0;
	TickType_t Local_50msDelay=pdMS_TO_TICKS(50);
    while(1)
    {
        xSemaphoreTake(APP_UtrasonicSem,portMAX_DELAY);
		ultrasonic_distance(&Local_Distance, &Ultrasonic_Notifaction);
        /*Send the Distance in The mailbox*/
        xQueueOverwrite(APP_DistanceMailBox,&Local_Distance);
        vTaskDelay(Local_50msDelay);
    }
}

/******************************************************************************
 * \Syntax          : void avoid_obstacles(void* Copy_AvoidPar)
 * \Description     : A task Implements the logic of obstacles avoid
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void avoid_obstacles(void* Copy_AvoidPar)
{
    static uint8_t Timeout = 0;
    static u64 Local_RecDistance=0;
    static EventBits_t Local_Avoid=0;
    TickType_t Local_50msPer=pdMS_TO_TICKS(50U);
    while(1)
    {
        xEventGroupWaitBits(APP_CarEvents, APP_CAR_MOVING, pdFALSE, pdFALSE, portMAX_DELAY);
        xQueuePeek(APP_DistanceMailBox, &Local_RecDistance, portMAX_DELAY);
        //Local_RecDistance=Local_Distance;
        if(Local_RecDistance<=MIN_DISTANCE)
        {
            /*Its Time to avoid obstacle*/
            xEventGroupSetBits(APP_CarEvents, APP_CAR_AVOID);
        }
        else
        {
            /* MISRA */
        }
        Local_Avoid=xEventGroupGetBits(APP_CarEvents);
        if ((Local_Avoid&APP_CAR_AVOID))
        {
            if (Local_RecDistance > 30U)
            {
                Motor_Set_Direction(Motor_Left_Reverse);
                Motor_Set_Direction(Motor_Right_Forward);
                if((APP_CarTimer-Timeout) > 0U)
                {
                    xEventGroupClearBits(APP_CarEvents, APP_CAR_AVOID);
                }
            }
            else
            {
                Motor_Set_Speed(80U, Right_Motors);
                Motor_Set_Speed(80U, Left_Motors);
                Motor_Set_Direction(Motor_Left_Reverse);
                Motor_Set_Direction(Motor_Right_Reverse);
                Timeout=APP_CarTimer;
            }
        }
        vTaskDelay(Local_50msPer);
    }
}

/******************************************************************************
 * \Syntax          : void CarStart_Task(void* Copy_StartPar)
 * \Description     : Start Moving the Car
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void CarStart_Task(void* Copy_StartPar)
{
    TickType_t Local_50MS=pdMS_TO_TICKS(50U);
    TickType_t Lcoal_FirstStart=xTaskGetTickCount();
    while(1)
    {
        xSemaphoreTake(APP_StartSem,portMAX_DELAY);
        Motor_Start();
        xEventGroupSetBits(APP_CarEvents, APP_CAR_MOVING);
        vTaskDelayUntil(&Lcoal_FirstStart, Local_50MS);
    }
}

/******************************************************************************
 * \Syntax          : void CarStop_Task(void* Copy_StopPar)
 * \Description     : Stop Moving the Car
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void CarStop_Task(void* Copy_StopPar)
{
    TickType_t Local_FirstOcure=xTaskGetTickCount();
    TickType_t Local_50MSPer=pdMS_TO_TICKS(50U);
    EventBits_t Local_EventBits=0;
    while(1)
    {
        Local_EventBits=xEventGroupGetBits(APP_CarEvents);
        if((APP_CarTimer>=60U) || ((Local_EventBits&APP_CAR_MOVING)==0))
        {
            Motor_Stop();
            APP_CarTimer=0U;
            xEventGroupClearBits(APP_CarEvents, (APP_SW1FLAG|APP_SW2FLAG|APP_CAR_MOVING));
        }
        vTaskDelayUntil(&Local_FirstOcure, Local_50MSPer);
    }
}

/******************************************************************************
 * \Syntax          : void Watch_Task(void* Copy_WatchPar)
 * \Description     : CalcUate the elapsed time of the car moving
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Watch_Task(void* Copy_WatchPar)
{
    TickType_t Local_OneSecond=pdMS_TO_TICKS(1000);
    while(1)
    {
        xEventGroupWaitBits(APP_CarEvents, APP_CAR_MOVING, pdFALSE, pdFALSE, portMAX_DELAY);
        APP_CarTimer++;
        vTaskDelay(Local_OneSecond);
    }
}

/******************************************************************************
 * \Syntax          : void ldr_swing_car(void* Copy_SwingPar)
 * \Description     : A task that Implements the logic of Swing of car
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void ldr_swing_car(void* Copy_SwingPar)
{
    static u32 Local_LDRArr[2]={0};
    static u16 Local_LDRDiff=0;
    static Flag_State_t APP_LDRFlag=FLAG_LOW;
    EventBits_t Local_AvoidBit=0;
    TickType_t Lcoal_50MsDelay=pdMS_TO_TICKS(75U);
    while(1)
    {
        xEventGroupWaitBits(APP_CarEvents, APP_CAR_MOVING, pdFALSE, pdFALSE, portMAX_DELAY);
        Local_AvoidBit=xEventGroupGetBits(APP_CarEvents);
        if((Local_AvoidBit&APP_CAR_AVOID)==0)
        {
            LDR_Read(Local_LDRArr);
            if(Local_LDRArr[0]>=Local_LDRArr[1])
            {
                Local_LDRDiff=(u16)(Local_LDRArr[0]-Local_LDRArr[1]);
                APP_LDRFlag=FLAG_HIGH;
            }
            else
            {
                Local_LDRDiff=(u16)(Local_LDRArr[1]-Local_LDRArr[0]);
                APP_LDRFlag=FLAG_LOW;
            }
            /*Send LDR Difference*/
            xQueueOverwrite(APP_LDRDiffMailBox,&Local_LDRDiff);
            if(Local_LDRDiff>=LDR_DIFF)
            {
                if(APP_LDRFlag==FLAG_HIGH)
                {
                    Motor_Set_Direction(Motor_Left_Forward);
                    Motor_Set_Direction(Motor_Right_Reverse);
                    Motor_Set_Speed(80U, Right_Motors);
                    Motor_Set_Speed(80U, Left_Motors);
                }
                else
                {
                    Motor_Set_Direction(Motor_Left_Reverse);
                    Motor_Set_Direction(Motor_Right_Forward);
                    Motor_Set_Speed(80U, Left_Motors);
                    Motor_Set_Speed(80U, Right_Motors);
                }
            }
            else
            {
                Motor_Set_Direction(Motor_Left_Forward);
                Motor_Set_Direction(Motor_Right_Forward);
                Motor_Set_Speed(80U, Left_Motors);
                Motor_Set_Speed(80U, Right_Motors);
            }
        }
        else
        {
            /*MISRA*/
        }
        vTaskDelay(Lcoal_50MsDelay);
    }
}

/******************************************************************************
 * \Syntax          : void LCD_Distancedisplay(void* Copy_DisPar)
 * \Description     : Display the Distance on LCD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void LCD_Distancedisplay(void* Copy_DisPar)
{
    static u64 Local_LastDistance=0;
    static u64 Local_MessuredDistance=0;
    TickType_t Local_StartTick=xTaskGetTickCount();
    TickType_t Local_100MSDelay=pdMS_TO_TICKS(100U);
    while(1)
    {
        xQueuePeek(APP_DistanceMailBox, &Local_MessuredDistance, portMAX_DELAY);
        if(Local_LastDistance != Local_MessuredDistance)
        {
            xSemaphoreTake(APP_LCDMutex,portMAX_DELAY);
            LCD_GoToXY(&APP_LCD, 8U, 1U);
            LCD_WriteNumber(&APP_LCD, (s64)Local_MessuredDistance);
            LCD_SendString(&APP_LCD, "  ");
            Local_LastDistance=Local_MessuredDistance;
            xSemaphoreGive(APP_LCDMutex);
        }
        else
        {
            /*MISRA*/
        }
        vTaskDelayUntil(&Local_StartTick,Local_100MSDelay);
    }
}

/******************************************************************************
 * \Syntax          : void LCD_LDRDisplay(void* Copy_LDRPar)
 * \Description     : Display the Difference of the 2 LDRs Reading on LCD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void LCD_LDRDisplay(void* Copy_LDRPar)
{
    static u16 Local_LastLDRDifference=0;
    static u16 Local_LDRDiff=0;
    TickType_t Local_StartTime=xTaskGetTickCount();
    TickType_t Local_175mSDelay=pdMS_TO_TICKS(175U);
    while(1)
    {
        xQueuePeek(APP_LDRDiffMailBox, &Local_LDRDiff, portMAX_DELAY);
        if(Local_LastLDRDifference!=Local_LDRDiff)
        {
            xSemaphoreTake(APP_LCDMutex,portMAX_DELAY);
            LCD_GoToXY(&APP_LCD, 12U, 0U);
            LCD_WriteNumber(&APP_LCD, (s64)Local_LDRDiff);
            LCD_SendString(&APP_LCD, "   ");
            Local_LastLDRDifference=Local_LDRDiff;
            xSemaphoreGive(APP_LCDMutex);
        }
        else
        {
            /*MISRA*/
        }
        vTaskDelayUntil(&Local_StartTime,Local_175mSDelay);
    }
}

/******************************************************************************
 * \Syntax          : void LCD_TimeDisplay(void* Copy_TimePar)
 * \Description     : Display the Difference of the 2 LDRs Reading on LCD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void LCD_TimeDisplay(void* Copy_TimePar)
{
    TickType_t Local_LastActive=xTaskGetTickCount();
    TickType_t Local_OneSecondBlocking=pdMS_TO_TICKS(1000);
    u8 Local_LastTime=0;
    while(1)
    {
        if(Local_LastTime!=APP_CarTimer)
        {
            xSemaphoreTake(APP_LCDMutex,portMAX_DELAY);
            LCD_GoToXY(&APP_LCD, 0U, 1U);
            LCD_WriteNumber(&APP_LCD, (s64)APP_CarTimer);
            /*Give the Mutex after finishing write the Time*/
            xSemaphoreGive(APP_LCDMutex);
            Local_LastTime=APP_CarTimer;
        }
        vTaskDelayUntil(&Local_LastActive,Local_OneSecondBlocking);
    }
}

/******************************************************************************
 * \Syntax          : void Temperature_Task(void* Copy_TemPar)
 * \Description     : Get the temperature every 5 Seconds
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : None
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void Temperature_Task(void* Copy_TemPar)
{
    static u32 Local_Temp=0U;
    static u32 Local_LastRead=0U;
    TickType_t Local_LastWake=xTaskGetTickCount();
    TickType_t Local_FiveSeconds=pdMS_TO_TICKS(5000UL);
    while(1)
    {
        /*Read the Temperature*/
        Local_Temp=Temp_Send_Read();
        /*reenter the Temperature if it was changed*/
        if(Local_LastRead!=Local_Temp)
        {
            /*Try to take the Mutex to write on LCD*/
            xSemaphoreTake(APP_LCDMutex,portMAX_DELAY);
            LCD_GoToXY(&APP_LCD, 6U, 0U);
            /*Send Temperature to LCD*/
            LCD_WriteNumber(&APP_LCD, (s64)Local_Temp);
            /*Take the current as last Reading*/
            Local_LastRead=Local_Temp;
            /*Give the Mutex after finishing write the Time*/
            xSemaphoreGive(APP_LCDMutex);
        }
        else
        {
            /*Do Nothing*/
        }
        vTaskDelayUntil(&Local_LastWake,Local_FiveSeconds);
    }
}

/**********************************************************************************************************************
 *  END OF FILE: RC_Car_Program.c
 *********************************************************************************************************************/
