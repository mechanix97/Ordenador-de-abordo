#include "app.h"

int vueltas = 5;
int pwm1[] = {0, 10, 16, 22, 30, 45, 60, 80, 100, 140, 200};
int pwm2[] = {0, 10, 16, 22, 30, 45, 60, 80, 100, 140, 200};
uint8_t UART3_rxBuffer[200] = {0};
int i = 0;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;

void setup(){
    SSD1306_Init();
	SH1106_Init();
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    turnOnLedPWM1(5);
    turnOnLedPWM2(5);
    //HAL_UART_Receive (&huart1, UART3_rxBuffer, 200, 5000);
    
    char xd[]="Hola Mundo \r\n";
    HAL_UART_Transmit(&huart1, (uint8_t *) xd, 13, 10);
    HAL_UART_Receive_DMA (&huart3, UART3_rxBuffer, 200);
}

void loop(){
    //HAL_Delay(1000);

    //debugPrintln(&huart1, UART3_rxBuffer); 
    //char cadena[10];

    //sprintf(cadena, "%02d ", UART3_rxBuffer+6);
    //vueltas = 0;

   /* SH1106_GotoXY(0,0);
    SSD1306_GotoXY(0,0);
    
    SSD1306_Puts(UART3_rxBuffer+i, &Font_11x18, 1);
    SSD1306_GotoXY(0,20);
    SSD1306_Puts("RPM", &Font_11x18, 1);
    SSD1306_GotoXY(0,40);
    SSD1306_Puts("0", &Font_11x18, 1);

    SSD1306_UpdateScreen();
    SH1106_UpdateScreen();   
    i++;
    if( i == 100){
        i = 0;
    }*/
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if (GPIO_Pin == GPIO_PIN_8) {
        vueltas++;
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

