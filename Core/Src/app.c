#include "app.h"

int pwm1[] = {0, 10, 16, 22, 30, 45, 60, 80, 100, 140, 200};
int pwm2[] = {0, 10, 16, 22, 30, 45, 60, 80, 100, 140, 200};

uint8_t UART3_rxBuffer[200] = {0};

int vueltas = 5;
int i = 0;

uint16_t AD_RES = 0;

refresh_function screen1_refresh_fp = & screen1_refresh_01;
refresh_function screen2_refresh_fp = & screen1_refresh_01;

char cadena[10];

extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

void setup(){
    SSD1306_Init();
	SH1106_Init();
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);   
    turnOnLedPWM1(5);
    turnOnLedPWM2(5);
    HAL_ADCEx_Calibration_Start(&hadc1);

    //HAL_UART_Receive (&huart1, UART3_rxBuffer, 200, 5000);
    
    //char xd[]="Hola Mundo \r\n";
    //HAL_UART_Transmit(&huart1, (uint8_t *) xd, 13, 10);
    //HAL_UART_Receive_DMA (&huart3, UART3_rxBuffer, 200);
}


void loop(){
    HAL_Delay(100);

    //HAL_ADC_Start_DMA(&hadc1,(uint32_t *) &AD_RES, 1);
   


    sprintf(cadena, "%05d ", i*60);
    //debugPrintln(&huart1, cadena); 
    i = 0;

    screen1_refresh_fp();
    screen2_refresh_fp();

   
    SSD1306_GotoXY(0,0);
   
    SH1106_Puts(cadena, &Font_11x18, 1);
    

//    turnOnLedPWM1(i);
    //turnOnLedPWM2(i);

    SSD1306_UpdateScreen();
    SH1106_UpdateScreen(); 
    /*i++;
    if( i == 11){
        i = 0;
    }*/

}

/* REFRESH_FUNCTIONS */
void screen1_refresh_01 () {
    static char srceen1_refresh_01_string[100] = "\0";
    SH1106_GotoXY(0,0);
    SSD1306_Puts(cadena, &Font_11x18, 1);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    // Conversion Complete & DMA Transfer Complete As Well
    // So The AD_RES Is Now Updated & Let's Move IT To The PWM CCR1
    // Update The PWM Duty Cycle With Latest ADC Conversion Result
    TIM2->CCR1 = (AD_RES<<4);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    HAL_UART_Transmit(&huart1, (uint8_t *) UART3_rxBuffer, 200, 10);
	char newline[2] = "\r\n";
	HAL_UART_Transmit(&huart1, (uint8_t *) newline, 2, 10);
    HAL_UART_Receive_DMA (&huart3, UART3_rxBuffer, 200);
}

void turnOnLedPWM1(uint16_t leds){
    if ( leds > 10 ){
        leds = 10;
    }
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm1[leds]);
}

void turnOnLedPWM2(uint16_t leds){
    if ( leds > 10 ){
        leds = 10;
    }
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pwm2[leds]);
}

void HAL_GPIO_EXTI_Cuback(uint16_t GPIO_Pin){
    if (GPIO_Pin == GPIO_PIN_8) {
        i++;
    } else if (GPIO_Pin == GPIO_PIN_0) {
        vueltas++;
        vueltas = vueltas % 11;
    } 
}

void debugPrint(UART_HandleTypeDef *huart, char _out[]){
	HAL_UART_Transmit(huart, (uint8_t *) _out, strlen(_out), 10);
} 

void debugPrintln(UART_HandleTypeDef *huart, char _out[]){
	HAL_UART_Transmit(huart, (uint8_t *) _out, strlen(_out), 10);
	char newline[2] = "\r\n";
	HAL_UART_Transmit(huart, (uint8_t *) newline, 2, 10);
}

