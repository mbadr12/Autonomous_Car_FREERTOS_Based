#include "LIB/Std_Types.h"
#include "MCAL/RCC/RCC.h"
#include "APP/RC_Car.h"
#include "HAL/Motor/Motor.h"
#include "FreeRTOS.h"

int main(void)
{
    RCC_ConfigureClock(16U ,(RCC_PWMDIV_64|RCC_USE_PWM));
    CAR_Init();
    while(1);
}
