#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./rtc/bsp_rtc.h"
#include "./systick/bsp_SysTick.h"
#include "OLED_I2C.h"
#include "key.h"
#include "bsp_TiMbase.h" 
#include "bsp_beep.h"
#include "./dht11/bsp_dht11.h"
// N = 2^32/365/24/60/60 = 136 Äê
uint8_t alarm_flag=0,stopwatch_flag=0;
uint8_t str_day[15]={0};
uint8_t str_time[15]={0};
uint8_t str_canderday[50]={0};
uint8_t str_time_set[15]={0};
uint8_t str_day_set[15]={0};
uint8_t str_alarm_time[15]={0};
uint8_t str_stop_time[15]={0};
uint8_t str_tempandhumidity[20]={0};
uint8_t page=0,page_before=0,next=0,back_page=0,alarm_ring=0;
uint32_t i=0;
struct rtc_time systmtime=
{
0,0,0,1,1,2021,0
};
struct rtc_time time_set=
{
0,0,0,1,1,2021,0
};
struct rtc_time alarm_time=
{
0,0,0,1,1,2021,0
};
struct rtc_time stopwatch_time=
{
0,0,0,1,1,2021,0
};

DHT11_Data_TypeDef dht11_data=
{
0,0,0,0,0	
};
extern __IO uint32_t TimeDisplay ;

int main()
{		
	  USART_Config();		
		I2C_Configuration();
		OLED_Init();
		KEY_GPIO_Config();
	  RTC_NVIC_Config();
		OLED_CLS();
	  RTC_CheckAndConfig(&systmtime);
		BASIC_TIM_Init();
		Beep_Config();
		DHT11_Init();
		DHT11_Read_TempAndHumidity(&dht11_data);
	  while (1)
	  {
			if(alarm_ring==1){
				play_failed();
				alarm_ring=0;
			}
		}
}

