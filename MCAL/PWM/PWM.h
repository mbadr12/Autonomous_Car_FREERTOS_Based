#ifndef MCAL_PWM_DRIVERS_PWM_H_
#define MCAL_PWM_DRIVERS_PWM_H_

/* Includes */

#include "LIB/TM4C123xx.h"
#include <stdint.h>

/* Config Defines */

#define PWM_Generator0                  0U
#define PWM_Generator1                  1U
#define PWM_Generator2                  2U
#define PWM_Generator3                  3U

#define PWM_Channel_0                   0x01U
#define PWM_Channel_1                   0x02U
#define PWM_Channel_2                   0x04U
#define PWM_Channel_3                   0x08U
#define PWM_Channel_4                   0x10U
#define PWM_Channel_5                   0x20U
#define PWM_Channel_6                   0x40U
#define PWM_Channel_7                   0x80U

#define PWM_DO_NOTHING                  0x00U
#define PWM_INVERT                      0x01U
#define PWM_DRIVE_LOW                   0x02U
#define PWM_DRIVE_HIGH                  0x03U

/* Config Struct */

typedef struct
{
    uint8_t Channel;
    uint8_t Channel_Action_CompBDowm;
    uint8_t Channel_Action_CompADowm;
    uint8_t Channel_Action_Load;
    uint8_t Channel_Action_Zero;
} PWM_InitTypeDef;

/* Function Prototypes */

void PWM_Init(PWM_RegDef_t *PWMx, PWM_InitTypeDef *PWM_Config);
void PWM_Set_Load(PWM_RegDef_t *PWMx, uint8_t Generator, uint16_t Load_Value);
void PWM_Set_Comp(PWM_RegDef_t *PWMx, uint8_t Channel, uint16_t Comp_Value);
void PWM_Start(PWM_RegDef_t *PWMx, uint8_t Generator);
void PWM_Output_Enable(PWM_RegDef_t *PWMx, uint8_t Channel);

#endif /* MCAL_PWM_DRIVERS_PWM_H_ */
