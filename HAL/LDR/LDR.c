#include "LDR.h"

void LDR_Init()
{
    GPIO_Config_t LDR_Sensor[2] = { };

    /*Enable the ADC0 Module*/
    RCC_SetPrephralClockState(RCC_ADC0, RCC_CLK_ENABLE);
    RCC_SetPrephralClockState(RCC_GPIOE, RCC_CLK_ENABLE);

    LDR_Sensor[0].Port = GPIO_PORTE;
    LDR_Sensor[0].Pin = GPIO_PIN1;
    LDR_Sensor[0].PinPull = GPIO_PIN_FLOATING;
    LDR_Sensor[0].Mode = GPIO_PIN_ANALOG;

    LDR_Sensor[1].Port = GPIO_PORTE;
    LDR_Sensor[1].Pin = GPIO_PIN2;
    LDR_Sensor[1].PinPull = GPIO_PIN_FLOATING;
    LDR_Sensor[1].Mode = GPIO_PIN_ANALOG;

    /*Enable PORTE and Set PE2 as analog input*/
    GPIO_Init(LDR_Sensor, 2);

    ADC_Sequencer_InitTypeDef LDR = { };
    LDR.Sequencer = ADC_SEQUENCER1;
    LDR.Trigger = ADC_PROCESSOR_TRIGGER;
    LDR.Oversampling = ADC_OVERSAMPLING_64X;

    ADC_SequencerInit(ADC0, &LDR);

    ADC_Sequencer_ConfigTypeDef Seq = { };
    Seq.Sequence1_Control[1] = ADC_SAMPLE_INTERRUPT | ADC_END_OF_SEQUENCE;
    Seq.Sequence1[0] = ADC_AIN2;
    Seq.Sequence1[1] = ADC_AIN1;
    Seq.Sequencer = ADC_SEQUENCER1;

    ADC_SequencerConfig(ADC0, &Seq);

    ADC_SequencerEnable(ADC0, ADC_SEQUENCER1);
}

void LDR_Read(uint32_t *arr)
{
    ADC_StartConversion(ADC0, ADC_SEQUENCER1);

    arr[0] = ADC_GetData(ADC0, ADC_SEQUENCER1);
    arr[1] = ADC_GetData(ADC0, ADC_SEQUENCER1);
    ADC_Clear_Interrupt(ADC0, ADC_SEQUENCER1);
}

