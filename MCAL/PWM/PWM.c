#include "LIB/Std_Types.h"
#include "MCAL/PWM/PWM.h"

void PWM_Init(PWM_RegDef_t *PWMx, PWM_InitTypeDef *PWM_Config)
{
    switch (PWM_Config->Channel)
    {
    case PWM_Channel_0:
        PWMx->_0_CTL = 0; /* Disable Generator */
        PWMx->_0_GENA = 0; /* Clear Generator Controls */
        PWMx->_0_GENA |= PWM_Config->Channel_Action_Zero;
        PWMx->_0_GENA |= (PWM_Config->Channel_Action_Load << 2U);
        PWMx->_0_GENA |= (PWM_Config->Channel_Action_CompADowm << 6U);
        PWMx->_0_GENA |= (PWM_Config->Channel_Action_CompBDowm << 10U);
        break;
    case PWM_Channel_1:
        PWMx->_0_CTL = 0; /* Disable Generator */
        PWMx->_0_GENB = 0; /* Clear Generator Controls */
        PWMx->_0_GENB |= PWM_Config->Channel_Action_Zero;
        PWMx->_0_GENB |= (PWM_Config->Channel_Action_Load << 2U);
        PWMx->_0_GENB |= (PWM_Config->Channel_Action_CompADowm << 6U);
        PWMx->_0_GENB |= (PWM_Config->Channel_Action_CompBDowm << 10U);
        break;
    case PWM_Channel_2:
        PWMx->_1_CTL = 0; /* Disable Generator */
        PWMx->_1_GENA = 0; /* Clear Generator Controls */
        PWMx->_1_GENA |= PWM_Config->Channel_Action_Zero;
        PWMx->_1_GENA |= (PWM_Config->Channel_Action_Load << 2U);
        PWMx->_1_GENA |= (PWM_Config->Channel_Action_CompADowm << 6U);
        PWMx->_1_GENA |= (PWM_Config->Channel_Action_CompBDowm << 10U);
        break;
    case PWM_Channel_3:
        PWMx->_1_CTL = 0; /* Disable Generator */
        PWMx->_1_GENB = 0; /* Clear Generator Controls */
        PWMx->_1_GENB |= PWM_Config->Channel_Action_Zero;
        PWMx->_1_GENB |= (PWM_Config->Channel_Action_Load << 2U);
        PWMx->_1_GENB |= (PWM_Config->Channel_Action_CompADowm << 6U);
        PWMx->_1_GENB |= (PWM_Config->Channel_Action_CompBDowm << 10U);
        break;
    case PWM_Channel_4:
        PWMx->_2_CTL = 0; /* Disable Generator */
        PWMx->_2_GENA = 0; /* Clear Generator Controls */
        PWMx->_2_GENA |= PWM_Config->Channel_Action_Zero;
        PWMx->_2_GENA |= (PWM_Config->Channel_Action_Load << 2U);
        PWMx->_2_GENA |= (PWM_Config->Channel_Action_CompADowm << 6U);
        PWMx->_2_GENA |= (PWM_Config->Channel_Action_CompBDowm << 10U);
        break;
    case PWM_Channel_5:
        PWMx->_2_CTL = 0; /* Disable Generator */
        PWMx->_2_GENB = 0; /* Clear Generator Controls */
        PWMx->_2_GENB |= PWM_Config->Channel_Action_Zero;
        PWMx->_2_GENB |= (PWM_Config->Channel_Action_Load << 2U);
        PWMx->_2_GENB |= (PWM_Config->Channel_Action_CompADowm << 6U);
        PWMx->_2_GENB |= (PWM_Config->Channel_Action_CompBDowm << 10U);
        break;
    case PWM_Channel_6:
        PWMx->_3_CTL = 0; /* Disable Generator */
        PWMx->_3_GENA = 0; /* Clear Generator Controls */
        PWMx->_3_GENA |= PWM_Config->Channel_Action_Zero;
        PWMx->_3_GENA |= (PWM_Config->Channel_Action_Load << 2U);
        PWMx->_3_GENA |= (PWM_Config->Channel_Action_CompADowm << 6U);
        PWMx->_3_GENA |= (PWM_Config->Channel_Action_CompBDowm << 10U);
        break;
    case PWM_Channel_7:
        PWMx->_3_CTL = 0; /* Disable Generator */
        PWMx->_3_GENB = 0; /* Clear Generator Controls */
        PWMx->_3_GENB |= PWM_Config->Channel_Action_Zero;
        PWMx->_3_GENB |= (PWM_Config->Channel_Action_Load << 2U);
        PWMx->_3_GENB |= (PWM_Config->Channel_Action_CompADowm << 6U);
        PWMx->_3_GENB |= (PWM_Config->Channel_Action_CompBDowm << 10U);
        break;
    default:
        break;
    }
}

void PWM_Set_Load(PWM_RegDef_t *PWMx, uint8_t Generator, uint16_t Load_Value)
{
    switch (Generator)
    {
    case PWM_Generator0:
        PWMx->_0_LOAD = Load_Value; /* Set Load Value */
        break;
    case PWM_Generator1:
        PWMx->_1_LOAD = Load_Value; /* Set Load Value */
        break;
    case PWM_Generator2:
        PWMx->_2_LOAD = Load_Value; /* Set Load Value */
        break;
    case PWM_Generator3:
        PWMx->_3_LOAD = Load_Value; /* Set Load Value */
        break;
    default:
        break;
    }
}

void PWM_Set_Comp(PWM_RegDef_t *PWMx, uint8_t Channel, uint16_t Comp_Value)
{
    switch (Channel)
    {
    case PWM_Channel_0:
        PWMx->_0_CMPA = Comp_Value; /* Set Comparator Value */
        break;
    case PWM_Channel_1:
        PWMx->_0_CMPB = Comp_Value; /* Set Comparator Value */
        break;
    case PWM_Channel_2:
        PWMx->_1_CMPA = Comp_Value; /* Set Comparator Value */
        break;
    case PWM_Channel_3:
        PWMx->_1_CMPB = Comp_Value; /* Set Comparator Value */
        break;
    case PWM_Channel_4:
        PWMx->_2_CMPA = Comp_Value; /* Set Comparator Value */
        break;
    case PWM_Channel_5:
        PWMx->_2_CMPB = Comp_Value; /* Set Comparator Value */
        break;
    case PWM_Channel_6:
        PWMx->_3_CMPA = Comp_Value; /* Set Comparator Value */
        break;
    case PWM_Channel_7:
        PWMx->_3_CMPB = Comp_Value; /* Set Comparator Value */
        break;
    default:
        break;
    }
}

void PWM_Start(PWM_RegDef_t *PWMx, uint8_t Generator)
{
    switch (Generator)
    {
    case PWM_Generator0:
        PWMx->_0_CTL |= 0x01U; /* Start PWM Generator */
        break;
    case PWM_Generator1:
        PWMx->_1_CTL |= 0x01U; /* Start PWM Generator */
        break;
    case PWM_Generator2:
        PWMx->_2_CTL |= 0x01U; /* Start PWM Generator */
        break;
    case PWM_Generator3:
        PWMx->_3_CTL |= 0x01U; /* Start PWM Generator */
        break;
    default:
        break;
    }
}

void PWM_Output_Enable(PWM_RegDef_t *PWMx, uint8_t Channel)
{
    PWMx->ENABLE |= Channel;
}
