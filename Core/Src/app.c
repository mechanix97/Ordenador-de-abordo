#include "app.h"

int vueltas = 5;
int pwm1[] = {0, 10, 16, 22, 30, 45, 60, 80, 100, 140, 200};
int pwm2[] = {0, 10, 16, 22, 30, 45, 60, 80, 100, 140, 200};

extern TIM_HandleTypeDef htim2;

void setup(){
    SSD1306_Init();
	SH1106_Init();
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    turnOnLedPWM1(5);
    turnOnLedPWM2(5);
}

void loop(){
    //HAL_Delay(100); 
    char cadena[10];

    sprintf(cadena, "%02d ", vueltas);
    //vueltas = 0;

    SH1106_GotoXY(0,0);
    SSD1306_GotoXY(0,0);

    SH1106_Puts("Audio de", &Font_11x18, 1);
    
    SSD1306_Puts("Audio de", &Font_11x18, 1);
    SSD1306_GotoXY(0,20);
    SSD1306_Puts("Francisca", &Font_11x18, 1);
    SSD1306_GotoXY(0,40);
    SSD1306_Puts("(4:50)", &Font_11x18, 1);

    SSD1306_UpdateScreen();
    SH1106_UpdateScreen();   
    
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

