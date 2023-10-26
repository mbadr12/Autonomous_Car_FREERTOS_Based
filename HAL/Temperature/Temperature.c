#include "Temperature.h"

uint32_t value;

void Temperature_Init()
{
    /*Enable the ADC0 Module*/
    RCC_SetPrephralClockState(RCC_ADC0, RCC_CLK_ENABLE);

    ADC_Sequencer_InitTypeDef Temp = {};
    Temp.Sequencer = ADC_SEQUENCER3;
    Temp.Trigger = ADC_PROCESSOR_TRIGGER;
    Temp.Oversampling = ADC_OVERSAMPLING_64X;

    ADC_SequencerInit(ADC0,&Temp);

    ADC_Sequencer_ConfigTypeDef Seq = {};
    Seq.Sequence3_Control = ADC_TEMP_SENSOR | ADC_SAMPLE_INTERRUPT | ADC_END_OF_SEQUENCE;
    Seq.Sequencer = ADC_SEQUENCER3;

    ADC_SequencerConfig(ADC0, &Seq);

    ADC_SequencerEnable(ADC0, ADC_SEQUENCER3);

}
uint32_t Temp_Send_Read()
{
    uint32_t TEMP =0;
    ADC_StartConversion(ADC0,ADC_SEQUENCER3);

    value = ADC_GetData(ADC0, ADC_SEQUENCER3);

    TEMP = (1475 - ((75 * (33) *value ) / 4096))/10;

    return TEMP;
}

