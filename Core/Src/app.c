#include "app.h"

int pwm1[] = {0, 7, 10, 16, 30, 42, 60, 80, 100, 140, 200};
int pwm2[] = {0, 7, 10, 16, 30, 45, 60, 80, 100, 140, 200};

char UART3_rxBuffer[200] = {0};

int vueltas = 0;
int i = 0;
uint32_t tick = 0;
uint32_t tick_anterior = 0;
uint32_t tick_actual = 0;

gps_t gps = {{0,0,0,0,0,0}, 'U', 0,0,'U',0,0,'U'};

uint32_t RPM = 0;
uint32_t GAS = 0;

refresh_function screen1_refresh_fp = & screen1_refresh_01;
refresh_function screen2_refresh_fp = & screen1_refresh_01;

char cadena[50] = {0};

extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;

void setup(){
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);   
    
    RPM_init();
    GAS_init();

    HAL_Delay(500);

    SSD1306_Init();
	SH1106_Init();
    
//    welcome();
    
    HAL_UART_Receive_DMA (&huart3, (uint8_t*) UART3_rxBuffer, 200);
}


void loop(){
    //screen1_refresh_fp();
    //screen2_refresh_fp();
    
//    HAL_Delay(100);

    

    //HAL_ADC_Start_DMA(&hadc1, &AD_RES, 1); 
    GAS = GAS_read();
    RPM = RPM_read();
    

    tick_actual = HAL_GetTick();
    tick =  tick_actual - tick_anterior;
    tick_anterior = tick_actual;
    sprintf(cadena, "%02lu%02lu%02lu-%lu",  gps.date.hour, gps.date.minute, gps.date.second, tick);
    debugPrintln(&huart1, cadena);

    sprintf(cadena, "VEL: %05d ", i);
    SH1106_GotoXY(0,0);
    SH1106_Puts(cadena, &Font_11x18, 1);
    
    sprintf(cadena, "RPM: %05lu ", RPM);   
    SH1106_GotoXY(0,20);
    SH1106_Puts(cadena, &Font_11x18, 1);
    
    sprintf(cadena, "GAS: %05lu ", GAS);   
    SH1106_GotoXY(0,40);
    SH1106_Puts(cadena, &Font_11x18, 1);

    sprintf(cadena, "BUT: %02d ", vueltas);
    SSD1306_GotoXY(0,0);    
    SSD1306_Puts(cadena, &Font_11x18, 1);

    sprintf(cadena, "LAT: %02lu.%05lu %c", gps.lat_int, gps.lat_dec, gps.lat);
    SSD1306_GotoXY(0,19);    
    SSD1306_Puts(cadena, &Font_7x10, 1);

    sprintf(cadena, "LON: %02lu.%05lu %c", gps.lon_int, gps.lon_dec, gps.lon);
    SSD1306_GotoXY(0,30);    
    SSD1306_Puts(cadena, &Font_7x10, 1);

    sprintf(cadena, "DAT: %02u/%02u/%04u", gps.date.day, gps.date.month, gps.date.year);
    SSD1306_GotoXY(0,41);    
    SSD1306_Puts(cadena, &Font_7x10, 1);

    sprintf(cadena, "TIME: %02u:%02u", gps.date.hour, gps.date.minute);
    SSD1306_GotoXY(0,52);    
    SSD1306_Puts(cadena, &Font_7x10, 1);

    turnOnLedPWM1(0);
    turnOnLedPWM2(0);

    SSD1306_UpdateScreen();
    SH1106_UpdateScreen(); 
}

void welcome() {
    for(int j = 0; j<=10; j++) {
        turnOnLedPWM1(j);
        turnOnLedPWM2(j);
        HAL_Delay(100);
    }

    char zanella[] = "Zanella     ";
    for(int j = 130; j >= 20; j-=5) {
        SSD1306_GotoXY(j, 10);
        SH1106_GotoXY(j, 10);
        
        SSD1306_Puts(zanella, &Font_11x18, 1);
        SH1106_Puts(zanella, &Font_11x18, 1);
        
        SSD1306_UpdateScreen();
        SH1106_UpdateScreen(); 
    }

     char rx[] = "RX 150       ";
    for(int j = 130; j >= 30; j-=5) {
        SSD1306_GotoXY(j,35);
        SH1106_GotoXY(j,35);
        

        SSD1306_Puts(rx, &Font_11x18, 1);
        SH1106_Puts(rx, &Font_11x18, 1);
        
        SSD1306_UpdateScreen();
        SH1106_UpdateScreen(); 
    }

    for(int j = 10; j >= 0; j--) {
        turnOnLedPWM1(j);
        turnOnLedPWM2(j);
        HAL_Delay(100);
    }

    HAL_Delay(3000); 
    SSD1306_Clear();
    SH1106_Clear();
}

/* REFRESH_FUNCTIONS */
void screen1_refresh_01 () {
    static char srceen1_refresh_01_string[100] = "\0";
    SH1106_GotoXY(0,0);
    SSD1306_Puts(cadena, &Font_11x18, 1);
}

/*
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    // Conversion Complete & DMA Transfer Complete As Well
}*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    gps_t mygps = parseGPSRead(UART3_rxBuffer);
    if( isValid(mygps) ) {
        gps = mygps;
    }    
    HAL_UART_Receive_DMA (&huart3, (uint8_t *)UART3_rxBuffer, 200);
}

void turnOnLedPWM1(uint16_t leds){
    if ( leds > 10 ){
        leds = 10;
    }
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pwm1[leds]);
}

void turnOnLedPWM2(uint16_t leds){
    if ( leds > 10 ){
        leds = 10;
    }
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm2[leds]);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    if (GPIO_Pin == GPIO_PIN_8) {
        i++;
    } else if (GPIO_Pin == GPIO_PIN_0) {
        vueltas++;
        vueltas = vueltas % 11;
    } else {
        vueltas = GPIO_Pin;
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

