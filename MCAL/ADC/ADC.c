/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       Author:  Adham EL-Serougy
 *         File:  ADC.c
 *        Layer:  MCAL
 *       Module:  ADC
 *      Version:  1.00
 *
 *  Description:  Initialize and configure ADC peripheral
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ADC.h"
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

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
                       ADC_Sequencer_InitTypeDef *Sequencer_Config)
{
    ADCx->ACTSS &= ~(1U << Sequencer_Config->Sequencer); /* Disable Sequencer */
    ADCx->SAC &= ~7UL; /* Clear Sample Averaging Control */
    ADCx->SAC |= Sequencer_Config->Oversampling; /* Set Sample Averaging Control */
    ADCx->EMUX &= ~(0xFU << (4U * Sequencer_Config->Sequencer)); /* Clear Trigger source */
    ADCx->EMUX |= (Sequencer_Config->Trigger
            << (4U * Sequencer_Config->Sequencer)); /* Set Trigger source for Sequencer */
}

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
                         ADC_Sequencer_ConfigTypeDef *Sequence)
{
    uint8_t i;
    switch (Sequence->Sequencer)
    {
    case ADC_SEQUENCER0:
        ADCx->SSMUX0 = 0U; /* Clear Sequence */
        ADCx->SSCTL0 = 0U; /* Clear Control */
        for (i = 8U;
                (i > 0U)
                        /*&& (((Sequence->Sequence0_Control[8U - i])
                                & ADC_END_OF_SEQUENCE) == 0U)*/; i--)
        {
            ADCx->SSMUX0 |= (Sequence->Sequence0[8U - i] << ((8U - i) * 4U));
            ADCx->SSCTL0 |= (Sequence->Sequence0_Control[8U - i]
                    << ((8U - i) * 4U));
        }
        break;
    case ADC_SEQUENCER1:
        ADCx->SSMUX1 = 0U; /* Clear Sequence */
        ADCx->SSCTL1 = 0U; /* Clear Control */
        for (i = 4U; (i > 0U)
        /*&& (((Sequence->Sequence1_Control[4U - i])
         & ADC_END_OF_SEQUENCE) == 0U)*/; i--)
        {
            ADCx->SSMUX1 |= (Sequence->Sequence1[4U - i] << ((4U - i) * 4U));
            ADCx->SSCTL1 |= (Sequence->Sequence1_Control[4U - i]
                    << ((4U - i) * 4U));
        }
        break;
    case ADC_SEQUENCER2:
        ADCx->SSMUX2 = 0U; /* Clear Sequence */
        ADCx->SSCTL2 = 0U; /* Clear Control */
        for (i = 4U;
                (i > 0U)
                        /*&& (((Sequence->Sequence2_Control[4U - i])
                                & ADC_END_OF_SEQUENCE) == 0U)*/; i--)
        {
            ADCx->SSMUX2 |= (Sequence->Sequence2[4U - i] << ((4U - i) * 4U));
            ADCx->SSCTL2 |= (Sequence->Sequence2_Control[4U - i]
                    << ((4U - i) * 4U));
        }
        break;
    case ADC_SEQUENCER3:
        ADCx->SSMUX3 = 0U; /* Clear Sequence */
        ADCx->SSCTL3 = 0U; /* Clear Control */
        ADCx->SSMUX3 |= Sequence->Sequence3;
        ADCx->SSCTL3 |= Sequence->Sequence3_Control;
        break;
    default:
        break;
    }
}

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

void ADC_SequenceIntEnable(ADC_RegDef_t *ADCx, uint8_t Sequencer)
{
    ADCx->IM |= (1U << Sequencer); /* Set Interrupt Mask */
}

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

void ADC_SequencerEnable(ADC_RegDef_t *ADCx, uint8_t Sequencer)
{
    ADCx->ACTSS |= (1U << Sequencer); /* Enable Sequencer */
}

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

void ADC_SequencerDisable(ADC_RegDef_t *ADCx, uint8_t Sequencer)
{
    ADCx->ACTSS &= ~(1U << Sequencer); /* Disable Sequencer */
}

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

void ADC_StartConversion(ADC_RegDef_t *ADCx, uint8_t Sequencer)
{
    ADCx->PSSI |= (1U << Sequencer); /* Processor Trigger to start conversion */
}

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

uint16_t ADC_GetData(ADC_RegDef_t *ADCx, uint8_t Sequencer)
{
    uint16_t data;
    while ((ADCx->RIS & (1U << Sequencer)) == 0)
        ; /* Wait for end of conversion */
    switch (Sequencer)
    {
    case ADC_SEQUENCER0:
        data = ADCx->SSFIFO0; /* Get data of Sequencer 0 */
        break;
    case ADC_SEQUENCER1:
        data = ADCx->SSFIFO1; /* Get data of Sequencer 1 */
        break;
    case ADC_SEQUENCER2:
        data = ADCx->SSFIFO2; /* Get data of Sequencer 2 */
        break;
    case ADC_SEQUENCER3:
        data = ADCx->SSFIFO3; /* Get data of Sequencer 3 */
        break;
    default:
        break;
    }
    return data;
}

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

void ADC_Clear_Interrupt(ADC_RegDef_t *ADCx, uint8_t Sequencer)
{
    ADCx->ISC |= (1U << Sequencer); /* Clear Interrupt flag */
}

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
                     void (*pfnIntHandler)(void))
{
    switch ((uint32_t) &(*ADCx))
    {
    case ADC0_BASE_ADDRESS:
        ptr_ADC[Sequencer] = pfnIntHandler;
        break;
    case ADC1_BASE_ADDRESS:
        ptr_ADC[4U + Sequencer] = pfnIntHandler;
        break;
    default:
        break;
    }
}
