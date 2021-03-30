#include "rpm.h"

void RPM_init() {
    HAL_ADCEx_Calibration_Start(&hadc1);
}

uint32_t RPM_read() {
    HAL_ADC_Start(&hadc1);    
    HAL_ADC_PollForConversion(&hadc1, 1); 
    uint32_t AD_RES = HAL_ADC_GetValue(&hadc1);
    /* do something */
    return AD_RES;
}
