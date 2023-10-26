
#ifndef HAL_TEMPERATURE_H_
#define HAL_TEMPERATURE_H_

#include "../../LIB/Std_Types.h"
#include "../../MCAL/GPIO/GPIO.h"
#include "../../MCAL/RCC/RCC.h"
#include "../../MCAL/ADC/ADC.h"

void Temperature_Init(void);
uint32_t Temp_Send_Read(void);


#endif /* HAL_TEMPERATURE_H_ */
