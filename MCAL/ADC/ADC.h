/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Adham EL-Serougy
 *         File:  ADC.h
 *        Layer:  MCAL
 *       Module:  ADC
 *      Version:  1.00
 *
 *  Description:  Interfaces Needed by user
 *
 *********************************************************************************************************************/
#ifndef MCAL_ADC_DRIVERS_ADC_H_
#define MCAL_ADC_DRIVERS_ADC_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "../../LIB/Std_Types.h"
#include "../../LIB/TM4C123xx.h"
#include <stdint.h>

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define ADC_SEQUENCER0                  0U
#define ADC_SEQUENCER1                  1U
#define ADC_SEQUENCER2                  2U
#define ADC_SEQUENCER3                  3U

#define ADC_CONTINOUS_SAMPLING          0xFU
#define ADC_PROCESSOR_TRIGGER           0U

#define ADC_TEMP_SENSOR                 8U
#define ADC_SAMPLE_INTERRUPT            4U
#define ADC_END_OF_SEQUENCE             2U

#define ADC_AIN0                        0U
#define ADC_AIN1                        1U
#define ADC_AIN2                        2U
#define ADC_AIN3                        3U
#define ADC_AIN4                        4U
#define ADC_AIN5                        5U
#define ADC_AIN6                        6U
#define ADC_AIN7                        7U
#define ADC_AIN8                        8U
#define ADC_AIN9                        9U
#define ADC_AIN10                       10U
#define ADC_AIN11                       11U

#define ADC_OVERSAMPLING_0X             0U
#define ADC_OVERSAMPLING_2X             1U
#define ADC_OVERSAMPLING_4X             2U
#define ADC_OVERSAMPLING_8X             3U
#define ADC_OVERSAMPLING_16X            4U
#define ADC_OVERSAMPLING_32X            5U
#define ADC_OVERSAMPLING_64X            6U

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef struct
{
    uint8_t Sequencer;
    uint8_t Trigger;
    uint8_t Oversampling;
} ADC_Sequencer_InitTypeDef;

typedef struct
{
    uint8_t Sequencer;
    uint8_t Sequence0[8];
    uint8_t Sequence0_Control[8];
    uint8_t Sequence1[4];
    uint8_t Sequence1_Control[4];
    uint8_t Sequence2[4];
    uint8_t Sequence2_Control[4];
    uint8_t Sequence3;
    uint8_t Sequence3_Control;
} ADC_Sequencer_ConfigTypeDef;

/* Interrupt Pointers */

void (*ptr_ADC[8])(void);

/******************************************************************************
 * \Syntax          : void ADC_SequencerInit(ADC_RegDef_t *ADCx,
                       ADC_Sequencer_InitTypeDef *Sequencer_Config)
 * \Description     : Initialize ADC Sequencer
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : *ADCx     ADC module to initialize
 *                    *Sequencer_Config     Sequencer Configuration
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void ADC_SequencerInit(ADC_RegDef_t *ADCx,
                       ADC_Sequencer_InitTypeDef *Sequencer_Config);
/******************************************************************************
 * \Syntax          : void ADC_SequencerConfig(ADC_RegDef_t *ADCx,
                         ADC_Sequencer_ConfigTypeDef *Sequence)
 * \Description     : Initialize ADC Sequence for Sequencers
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : *ADCx     ADC module to initialize
 *                    *Sequence     Sequence for sequencers
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void ADC_SequencerConfig(ADC_RegDef_t *ADCx,
                         ADC_Sequencer_ConfigTypeDef *Sequence);
/******************************************************************************
 * \Syntax          : void ADC_SequenceIntEnable(ADC_RegDef_t *ADCx, uint8_t Sequencer)
 * \Description     : Enable Sequencer Interrupt
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : *ADCx     ADC module to initialize
 *                    Sequencer     Sequencer to modify
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void ADC_SequenceIntEnable(ADC_RegDef_t *ADCx, uint8_t Sequencer);
/******************************************************************************
 * \Syntax          : void ADC_SequencerEnable(ADC_RegDef_t *ADCx, uint8_t Sequencer)
 * \Description     : Enable Sequencer
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : *ADCx     ADC module to initialize
 *                    Sequencer     Sequencer to modify
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void ADC_SequencerEnable(ADC_RegDef_t *ADCx, uint8_t Sequencer);
/******************************************************************************
 * \Syntax          : void ADC_SequencerDisable(ADC_RegDef_t *ADCx, uint8_t Sequencer)
 * \Description     : Disable Sequencer
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : *ADCx     ADC module to initialize
 *                    Sequencer     Sequencer to modify
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void ADC_SequencerDisable(ADC_RegDef_t *ADCx, uint8_t Sequencer);
/******************************************************************************
 * \Syntax          : void ADC_StartConversion(ADC_RegDef_t *ADCx, uint8_t Sequencer)
 * \Description     : Start conversion for processor trigger
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : *ADCx     ADC module to initialize
 *                    Sequencer     Sequencer to modify
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void ADC_StartConversion(ADC_RegDef_t *ADCx, uint8_t Sequencer);
/******************************************************************************
 * \Syntax          : uint16_t ADC_GetData(ADC_RegDef_t *ADCx, uint8_t Sequencer)
 * \Description     : Get data from ADC FIFO
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : *ADCx     ADC module to initialize
 *                    Sequencer     Sequencer to modify
 * \Parameters (out): None
 * \Return value:   : uint16_t
 *******************************************************************************/
uint16_t ADC_GetData(ADC_RegDef_t *ADCx, uint8_t Sequencer);
/******************************************************************************
 * \Syntax          : void ADC_Clear_Interrupt(ADC_RegDef_t *ADCx, uint8_t Sequencer)
 * \Description     : Clear Interrupt Flags of Sequencer
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : *ADCx     ADC module to initialize
 *                    Sequencer     Sequencer to modify
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void ADC_Clear_Interrupt(ADC_RegDef_t *ADCx, uint8_t Sequencer);
/******************************************************************************
 * \Syntax          : void ADC_IntRegister(ADC_RegDef_t *ADCx, uint8_t Sequencer,
                     void (*pfnIntHandler)(void))
 * \Description     : Set Callback function to interrupt
 *
 * \Sync\Async      : Synchronous
 * \Reentrancy      : Non Reentrant
 * \Parameters (in) : *ADCx     ADC module to initialize
 *                    Sequencer     Sequencer to modify
 *                    *pfnIntHandler   Address callback function
 * \Parameters (out): None
 * \Return value:   : void
 *******************************************************************************/
void ADC_IntRegister(ADC_RegDef_t *ADCx, uint8_t Sequencer,
                     void (*pfnIntHandler)(void));

#endif /* MCAL_ADC_DRIVERS_ADC_H_ */
