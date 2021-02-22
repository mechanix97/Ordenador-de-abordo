#include "app.h"

int vueltas = 0;

void setup(){
    SSD1306_Init();
	SH1106_Init();
}

void loop(){
    char cadena[10];
    sprintf(cadena, "%d", vueltas);
    //vueltas = 0;

    SH1106_GotoXY(0,0);
    SSD1306_GotoXY(0,0);

    SH1106_Puts(cadena, &Font_11x18, 1);
    SSD1306_Puts(cadena, &Font_11x18, 1);

    SSD1306_UpdateScreen();
    SH1106_UpdateScreen();
    //HAL_Delay(1000);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if(GPIO_Pin == GPIO_PIN_8) // If The INT Source Is EXTI Line9 (A9 Pin)
    {
      vueltas++;
    }
}

