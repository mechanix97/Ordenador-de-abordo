#include "gas.h"

void GAS_init(){
    HAL_ADCEx_Calibration_Start(&hadc2);
}

uint32_t GAS_read(){
    HAL_ADC_Start(&hadc2);    
    HAL_ADC_PollForConversion(&hadc2, 1); 
    uint32_t AD_RES2 = HAL_ADC_GetValue(&hadc2);
    /* do something */
    return AD_RES2;
}
