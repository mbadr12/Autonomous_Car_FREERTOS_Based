/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Mahmoud Badr
 *	   	   File:  LCD_Program.c
 *		  Layer:  HAL
 *       Module:  LCD
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

#include "LCD.h"

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
void _delay_ms(u32 Copy_Delay)
{
    u32 Local_Counter=0;
    for( ;Local_Counter<(1000UL*Copy_Delay);Local_Counter++);
}

/***********************************************************************************************
 * \Syntax          : void LCD_SendComand(LCD_Config_t* Copy_Config,u8 Copy_Comand)
 * \Description     : Send Command to LCD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config     The Configuration structure for the module
 *                    Copy_Command     Command to be sent to LCD
 * \Parameters (out): None
 * \Return value:   : void
 *********************************************************************************************/
static void LCD_SendComand(LCD_Config_t* Copy_Config,u8 Copy_Comand)
{
    u8 Local_Counter=0;
    u8 MaxValue=Copy_Config->Mode_t;
    /*set RS and RW pin to zero*/
    GPIO_SetPinValue(Copy_Config->ControlPinsPorts[LCD_RS], Copy_Config->ControlPins[LCD_RS], GPIO_PIN_LOW);
    GPIO_SetPinValue(Copy_Config->ControlPinsPorts[LCD_RW], Copy_Config->ControlPins[LCD_RW], GPIO_PIN_LOW);
    /*send bits of command*/
    for( ;Local_Counter<MaxValue;Local_Counter++)
    {
        GPIO_SetPinValue(Copy_Config->DataPinsPorts[Local_Counter],Copy_Config->DataPins[Local_Counter],(GPIO_PinValue_t)GET_BIT(Copy_Comand,Local_Counter));
    }
    /*Enable Pulse*/
    GPIO_SetPinValue(Copy_Config->ControlPinsPorts[LCD_E], Copy_Config->ControlPins[LCD_E], GPIO_PIN_HIGH);
    _delay_ms(2);
    GPIO_SetPinValue(Copy_Config->ControlPinsPorts[LCD_E], Copy_Config->ControlPins[LCD_E], GPIO_PIN_LOW);
}

/***********************************************************************************************
 * \Syntax          : ErrorState_t LCD_SendData(LCD_Config_t* Copy_Config,char Copy_Data)
 * \Description     : Send data to LCD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config     The Configuration structure for the module
 *                    Copy_Data     Data to be Sent to LCD
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 *********************************************************************************************/
static ErrorState_t LCD_SendData(LCD_Config_t* Copy_Config,char Copy_Data)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_Counter=0;
    u8 MaxValue=Copy_Config->Mode_t;
    if(Copy_Config==NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        /*set RS pin to 1 and RW pin to zero*/
        GPIO_SetPinValue(Copy_Config->ControlPinsPorts[LCD_RS], Copy_Config->ControlPins[LCD_RS], GPIO_PIN_HIGH);
        GPIO_SetPinValue(Copy_Config->ControlPinsPorts[LCD_RW], Copy_Config->ControlPins[LCD_RW], GPIO_PIN_LOW);
        /*send bits of command*/
        for( ;Local_Counter<MaxValue;Local_Counter++)
        {
            GPIO_SetPinValue(Copy_Config->DataPinsPorts[Local_Counter],Copy_Config->DataPins[Local_Counter],(GPIO_PinValue_t)GET_BIT(Copy_Data,Local_Counter));
        }
        /*Enable Pulse*/
        GPIO_SetPinValue(Copy_Config->ControlPinsPorts[LCD_E], Copy_Config->ControlPins[LCD_E], GPIO_PIN_HIGH);
        _delay_ms(2);
        GPIO_SetPinValue(Copy_Config->ControlPinsPorts[LCD_E], Copy_Config->ControlPins[LCD_E], GPIO_PIN_LOW);
    }
    return Local_ErrorState;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/******************************************************************************
 * \Syntax          : ErrorState_t LCD_Init(LCD_Config_t* Copy_Config)
 * \Description     : Initialize the LCD module
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config     The Configuration structure for the module
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 *******************************************************************************/
ErrorState_t LCD_Init(LCD_Config_t* Copy_Config)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_Counter=0;
    u8 Local_Max=Copy_Config->Mode_t;
    u8 Local_FuncSet=0x30;
    u8 Local_DisSet=0x0c;
    GPIO_Config_t LCD_Config[11];
    if(Copy_Config==NULL)
    {
        Local_ErrorState=E_WRONG_OPTION;
    }
    else
    {
        /*Initialize the GPIO Pins*/
        for( ;Local_Counter<Local_Max;Local_Counter++)
        {
            LCD_Config[Local_Counter].Port=Copy_Config->DataPinsPorts[Local_Counter];
            LCD_Config[Local_Counter].Pin=Copy_Config->DataPins[Local_Counter];
            LCD_Config[Local_Counter].Dir=GPIO_PIN_OUTPUT;
            LCD_Config[Local_Counter].Mode=GPIO_PIN_DIGITAL;
            LCD_Config[Local_Counter].PinPull=GPIO_PIN_FLOATING;
            LCD_Config[Local_Counter].PinState=GPIO_PIN_PUSHPULL;
            LCD_Config[Local_Counter].OutputCurrent=GPIO_2MA_DRIVE;
            RCC_SetPrephralClockState((RCC_Prph_t)(RCC_GPIOA+Copy_Config->DataPinsPorts[Local_Counter]), RCC_CLK_ENABLE);
        }
        for(Local_Counter=0 ;Local_Counter<3;Local_Counter++)
        {
            LCD_Config[Local_Counter+Local_Max].Port=Copy_Config->ControlPinsPorts[Local_Counter];
            LCD_Config[Local_Counter+Local_Max].Pin=Copy_Config->ControlPins[Local_Counter];
            LCD_Config[Local_Counter+Local_Max].Dir=GPIO_PIN_OUTPUT;
            LCD_Config[Local_Counter+Local_Max].Mode=GPIO_PIN_DIGITAL;
            LCD_Config[Local_Counter+Local_Max].PinPull=GPIO_PIN_FLOATING;
            LCD_Config[Local_Counter+Local_Max].PinState=GPIO_PIN_PUSHPULL;
            LCD_Config[Local_Counter+Local_Max].OutputCurrent=GPIO_2MA_DRIVE;
            RCC_SetPrephralClockState((RCC_Prph_t)(RCC_GPIOA+Copy_Config->ControlPinsPorts[Local_Counter]), RCC_CLK_ENABLE);
        }
        Local_ErrorState=GPIO_Init(LCD_Config, Local_Max+3);
        /*set the font size*/
        INSERT_BIT(Local_FuncSet,LCD_FONT_BIT,(Copy_Config->Font));
        /*Set the number of lines*/
        INSERT_BIT(Local_FuncSet,LCD_LINE_BIT,(Copy_Config->LinesNum));
        /*set the cursor*/
        INSERT_VALUE(Local_DisSet,0,2,(Copy_Config->Cursor));
        /*Delay for 40 ms*/
        _delay_ms(40);
        if(Local_ErrorState==E_OK)
        {
            switch(Copy_Config->Mode_t)
            {
            case LCD_4BIT_MODE:
                /*set the mode*/
                CLR_BIT(Local_FuncSet,LCD_MODE_BIT);
                /*Function Set*/
                LCD_SendComand(Copy_Config, (Local_FuncSet>>4));
                LCD_SendComand(Copy_Config, (Local_FuncSet>>4));
                LCD_SendComand(Copy_Config, (Local_FuncSet&0x0f));
                /*Display on off*/
                LCD_SendComand(Copy_Config, 0x00);
                LCD_SendComand(Copy_Config, Local_DisSet);
                /*Display clear*/
                LCD_SendComand(Copy_Config, 0x00);
                LCD_SendComand(Copy_Config, LCD_CLEAR_VAL); break;
            case LCD_8BIT_MODE:
                /*Function Set*/
                LCD_SendComand(Copy_Config, Local_FuncSet);
                /*Display on off*/
                LCD_SendComand(Copy_Config, Local_DisSet);
                /*Display clear*/
                LCD_SendComand(Copy_Config, LCD_CLEAR_VAL); break;
            default: Local_ErrorState=E_WRONG_OPTION;
            }
        }
    }
    return Local_ErrorState;
}

/*****************************************************************************************
 * \Syntax          : ErrorState_t LCD_SendChar(LCD_Config_t* Copy_Config,char Copy_Char)
 * \Description     : Display a character on LCD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config     The Configuration structure for the module
 *                    Copy_Char       character to be displayed on LCD
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 ******************************************************************************************/
ErrorState_t LCD_SendChar(LCD_Config_t* Copy_Config,char Copy_Char)
{
    ErrorState_t Local_ErrorState=E_OK;
    if(Copy_Config->XPosition[0]==16)
    {
        TOGGLE_BIT(Copy_Config->Yposition[0],0);
        Copy_Config->XPosition[0]=0;
        LCD_GoToXY(Copy_Config, Copy_Config->XPosition[0], Copy_Config->Yposition[0]);
    }
    if(Copy_Config->Mode_t==LCD_4BIT_MODE)
    {
        LCD_SendData(Copy_Config, (Copy_Char>>4));
        _delay_ms(5);
    }
    Local_ErrorState=LCD_SendData(Copy_Config, Copy_Char);
    Copy_Config->XPosition[0]++;
    return Local_ErrorState;
}

/***********************************************************************************************
 * \Syntax          : ErrorState_t LCD_SendString(LCD_Config_t* Copy_Config,char *Copy_String)
 * \Description     : Display string on LCD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config     The Configuration structure for the module
 *                    Copy_String     String to be displayed on LCD
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 *********************************************************************************************/
ErrorState_t LCD_SendString(LCD_Config_t* Copy_Config,char *Copy_String)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_Counter=0;
    if(Copy_String==NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        while(Copy_String[Local_Counter] != '\0')
        {
            LCD_SendChar(Copy_Config,Copy_String[Local_Counter]);
            Local_Counter++;
        }
    }
    return Local_ErrorState;
}

/***********************************************************************************************
 * \Syntax          : ErrorState_t LCD_Clear(LCD_Config_t* Copy_Config)
 * \Description     : Clear display of LCD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config     The Configuration structure for the module
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 *********************************************************************************************/
ErrorState_t LCD_Clear(LCD_Config_t* Copy_Config)
{
    ErrorState_t Local_ErrorState=E_OK;
    if(Copy_Config==NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        if(Copy_Config->Mode_t==4)
        {
            LCD_SendComand(Copy_Config, 0x00);
        }
        LCD_SendComand(Copy_Config, LCD_CLEAR_VAL);
        Copy_Config->XPosition[0]=0;
        Copy_Config->Yposition[0]=0;
    }
    return Local_ErrorState;
}

/***********************************************************************************************
 * \Syntax          : ErrorState_t LCD_WriteNumber(LCD_Config_t* Copy_Config,s64 Copy_Number)
 * \Description     : Display Number on LCD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config     The Configuration structure for the module
 *                    Copy_Number     Number to be displayed on LCD
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 *********************************************************************************************/
ErrorState_t LCD_WriteNumber(LCD_Config_t* Copy_Config,s64 Copy_Number)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_Length=0;
    u8 Local_Counter=0;
    char Local_Str[32]={0};
    u32 Local_Rem=0;
    /*if number is 0 send '0'*/
    if(Copy_Number==0)
    {
        Local_ErrorState=LCD_SendChar(Copy_Config, '0');
    }
    else
    {
        /*if number < 0 send '-'*/
        if(Copy_Number<0)
        {
            LCD_SendChar(Copy_Config, '-');
            Copy_Number*=-1;
        }
        /*Get the number length*/
        Local_Rem=Copy_Number;
        while(Local_Rem > 0)
        {
            Local_Rem/=10;
            Local_Length++;
        }
        /*Put the number ASCII into the array*/
        for(Local_Counter=0 ;Local_Counter < Local_Length;Local_Counter++)
        {
            Local_Rem=Copy_Number%10;
            Copy_Number/=10;
            Local_Str[(Local_Length-Local_Counter-1)]=Local_Rem+'0';
        }
        Local_Str[Local_Length]='\0';
        Local_ErrorState=LCD_SendString(Copy_Config, Local_Str);
    }
    return Local_ErrorState;
}

/***********************************************************************************************
 * \Syntax          : ErrorState_t LCD_GoToXY(LCD_Config_t* Copy_Config,u8 Copy_XPosition,u8 Copy_YPosition)
 * \Description     : Go to specific position on LCD
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : Copy_Config     The Configuration structure for the module
 *                    Copy_XPosition  Location on the row
 *                    Copy_YPosition  Location on the Column
 * \Parameters (out): None
 * \Return value:   : ErrorState_t
 *********************************************************************************************/
ErrorState_t LCD_GoToXY(LCD_Config_t* Copy_Config,u8 Copy_XPosition,u8 Copy_YPosition)
{
    ErrorState_t Local_ErrorState=E_OK;
    u8 Local_Address=Copy_XPosition|0x80;
    if(Copy_Config == NULL)
    {
        Local_ErrorState=E_NULL_POINTER;
    }
    else
    {
        *(Copy_Config->XPosition)=Copy_XPosition-0x80UL;
        Copy_Config->Yposition[0]=Copy_YPosition;
        if(Copy_YPosition==1)
        {
            Local_Address+=0x40UL;
        }
        if(Copy_Config->Mode_t==4UL)
        {
            LCD_SendComand(Copy_Config, Local_Address>>4);
        }
        LCD_SendComand(Copy_Config, Local_Address);
    }
    return Local_ErrorState;
}

/**********************************************************************************************************************
 *  END OF FILE: LCD_Program.c
 *********************************************************************************************************************/
