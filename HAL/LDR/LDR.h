#ifndef HAL_LDR_H_
#define HAL_LDR_H_
#include "../../LIB/Std_Types.h"
#include "../../MCAL/GPIO/GPIO.h"
#include "../../MCAL/RCC/RCC.h"
#include "../../MCAL/ADC/ADC.h"

void LDR_Init(void);
void LDR_Read(uint32_t *arr);

#endif /* HAL_LDR_H_ */
