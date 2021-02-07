
#include "stm32f10x_it.h"
#include "./usart/bsp_usart.h"
#include "./systick/bsp_SysTick.h"
#include "./rtc/bsp_rtc.h"
#include "key.h"
#include "bsp_TiMbase.h" 
#include "OLED_I2C.h"
#include "bsp_beep.h"
#include "./dht11/bsp_dht11.h"
uint8_t first_in=0,set_select=0;
extern uint32_t TimeDisplay;
extern struct rtc_time systmtime;
extern struct rtc_time time_set;
extern struct rtc_time alarm_time;
extern struct rtc_time stopwatch_time;
extern DHT11_Data_TypeDef dht11_data;
extern uint8_t alarm_flag,stopwatch_flag,alarm_ring;
extern uint8_t page,page_before,next,back_page;
extern uint32_t i;
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */

void RTC_IRQHandler(void)
{
	  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
	  {
	    /* Clear the RTC Second interrupt */
	    RTC_ClearITPendingBit(RTC_IT_SEC);
			Time_Display( RTC_GetCounter(),&systmtime);
			if(stopwatch_flag==1)
			{
				stopwatch_time.tm_sec++;
				if(stopwatch_time.tm_sec==60){
					stopwatch_time.tm_sec=0;
					stopwatch_time.tm_min++;
				}
				if(stopwatch_time.tm_min==60){
					stopwatch_time.tm_min=0;
					stopwatch_time.tm_hour++;
				}
				if(stopwatch_time.tm_hour==24){
					stopwatch_time.tm_hour=0;
				}
			}
			if(alarm_flag==1&&systmtime.tm_hour==alarm_time.tm_hour&&systmtime.tm_min==alarm_time.tm_min&&systmtime.tm_sec==alarm_time.tm_sec)
			{	
				alarm_ring=1;
			}
			oled_page_display(page);
	    /* Wait until last write operation on RTC registers has finished */
	    RTC_WaitForLastTask();
	  }
}
void BASIC_TIM_IRQHandler(void)
{
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{
			i=0;
			//page0
			if(page==0&&next%2==0){
				OLED_ShowStr(0, 6, "      ", 1);
				OLED_ShowStr(0, 7, "      ", 1);
			}
			if(page==0&&next%2==1){
				OLED_ShowStr(90, 6, "      ", 1);
				OLED_ShowStr(90, 7, "      ", 1);
			}	
			//page1
			if(page==1&&next%2==0){
				OLED_ShowStr(0, 6, "      ", 1);
				OLED_ShowStr(0, 7, "      ", 1);
				DHT11_Read_TempAndHumidity(&dht11_data);
			}
			if(page==1&&next%2==1){
				OLED_ShowStr(90, 6, "      ", 1);
				OLED_ShowStr(90, 7, "      ", 1);
				DHT11_Read_TempAndHumidity(&dht11_data);
			}
			//page2
			if(page==2&&next%4==0){
				OLED_ShowStr(0, 0, "           ", 1);
				OLED_ShowStr(0, 1, "           ", 1);
			}
			if(page==2&&next%4==1){
				OLED_ShowStr(0, 2, "      ", 1);
				OLED_ShowStr(0, 3, "      ", 1);
			}
			if(page==2&&next%4==2){
				OLED_ShowStr(0, 4, "          ", 1);
				OLED_ShowStr(0, 5, "          ", 1);
			}
			if(page==2&&next%4==3){
				OLED_ShowStr(0 , 6,"        ", 1);
				OLED_ShowStr(0 , 7,"        ", 1);
			}			
			
			//page3
			if(page==3&&next%2==0){
				OLED_ShowStr(0, 0, "                ", 1);
				OLED_ShowStr(0, 1, "                ", 1);
			}
			if(page==3&&next%2==1){
				OLED_ShowStr(0, 2, "               ", 1);
				OLED_ShowStr(0, 3, "               ", 1);
			}
			//page4
			if(page==4&&next%3==0){
				OLED_ShowStr(0, 7, " ", 1);
			}
			if(page==4&&next%3==1){
				OLED_ShowStr(40, 7, " ", 1);
			}
			if(page==4&&next%3==2){
				OLED_ShowStr(70, 6, "          ", 1);
				OLED_ShowStr(70, 7, "          ", 1);
			}
			//page5
			if(page==5&&next%3==0){
				OLED_ShowStr(0, 7, " ", 1);
			}
			if(page==5&&next%3==1){
				OLED_ShowStr(40, 7, " ", 1);
			}
			if(page==5&&next%3==2){
				OLED_ShowStr(70, 6, "          ", 1);
				OLED_ShowStr(70, 7, "          ", 1);
			}
			//page6
			if(page==6&&next%2==0){
				OLED_ShowStr(0, 6, "   ", 1);
				OLED_ShowStr(0, 7, "   ", 1);
			}
			if(page==6&&next%2==1){
				OLED_ShowStr(70, 6, "        ", 1);
				OLED_ShowStr(70, 7, "        ", 1);
			}
			//page7
			if(page==7&&next%3==0){
				OLED_ShowStr(0, 7, " ", 1);
			}
			if(page==7&&next%3==1){
				OLED_ShowStr(40, 7, " ", 1);
			}
			if(page==7&&next%3==2){
				OLED_ShowStr(70, 6, "          ", 1);
				OLED_ShowStr(70, 7, "          ", 1);
			}
			//page8
			if(page==8&&next%2==0){
				OLED_ShowStr(0, 6, "     ", 1);
				OLED_ShowStr(0, 7, "     ", 1);
			}
			if(page==8&&next%2==1){
				OLED_ShowStr(70, 6, "      ", 1);
				OLED_ShowStr(70, 7, "      ", 1);
			}					
		}
		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);
}
void EXTI9_5_IRQHandler(void)                           //外部中断中断函数
{
		//next键
		if(EXTI_GetFlagStatus(EXTI_Line6) != RESET)           //检测中断标志位
		{			
			Delay_ms(200);
			while(GPIO_ReadInputDataBit(C1_GPIO_PORT,C1_GPIO_PIN)==RESET);
			printf("1");
			next++;
			EXTI_ClearFlag(EXTI_Line6);                         //清除标志位	

		}		
		
		//OK键
		if(EXTI_GetFlagStatus(EXTI_Line7) != RESET)           //检测中断标志位
		{
			Delay_ms(200);
			while(GPIO_ReadInputDataBit(C2_GPIO_PORT,C2_GPIO_PIN)==RESET);
			printf("2");
			OLED_CLS();
			//page0
			if(first_in==0&&page==0&&next%2==0){
				page_before =page;
				first_in =1;
				page=2;
				next=0;
			}
			if(first_in==0&&page==0&&next%2==1){
				page_before =page;
				first_in =1;
				page=1;
				next=0;
			}
			//page1
			if(first_in==0&&page==1&&next%2==0){
				page_before =page;
				first_in =1;
				page=2;
				next=0;
			}
			if(first_in==0&&page==1&&next%2==1){
				page_before =page;
				first_in =1;
				page=0;
				next=0;
			}
			//page2
			if(first_in==0&&page==2&&next%4==0){
				page_before =page;
				first_in =1;
				page=3;
				next=0;
			}
			if(first_in==0&&page==2&&next%4==1){
				page_before =page;
				first_in =1;
				page=6;
				next=0;
			}
			if(first_in==0&&page==2&&next%4==2){
				page_before =page;
				first_in =1;
				page=8;
				next=0;
			}
			if(first_in==0&&page==2&&next%4==3){
				page_before =page;
				first_in =1;
				page=9;
				next=0;
			}
			//page3
			if(first_in==0&&page==3&&next%2==0){
				page_before =page;
				first_in =1;
				page=4;
				set_select=0;
				next=0;
				time_set.tm_hour=systmtime.tm_hour;
				time_set.tm_min=systmtime.tm_min;
				time_set.tm_sec=systmtime.tm_sec;
			}
			if(first_in==0&&page==3&&next%2==1){
				page_before =page;
				first_in =1;
				page=5;
				set_select=0;
				next=0;
				time_set.tm_year=systmtime.tm_year;
				time_set.tm_mon=systmtime.tm_mon;
				time_set.tm_mday=systmtime.tm_mday;
			}
			//page4
			if(first_in==0&&page==4&&set_select%3==0)
			{
				switch(next%3)
				{
					case 0:time_set.tm_hour++;break;
					case 1:time_set.tm_hour--;break;
				}
			}
			if(first_in==0&&page==4&&set_select%3==1)
			{
				switch(next%3)
				{
					case 0:time_set.tm_min++;break;
					case 1:time_set.tm_min--;break;
				}
			}
			if(first_in==0&&page==4&&set_select%3==2)
			{
				switch(next%3)
				{
					case 0:time_set.tm_sec++;break;
					case 1:time_set.tm_sec--;break;
				}
			}
			if(first_in==0&&page==4&&next%3==2)		
			{
				set_select++;
			}		
			//page5
			if(first_in==0&&page==5&&set_select%3==0)
			{
				switch(next%3)
				{
					case 0:time_set.tm_year++;break;
					case 1:time_set.tm_year--;break;
				}
			}
			if(first_in==0&&page==5&&set_select%3==1)
			{
				switch(next%3)
				{
					case 0:time_set.tm_mon++;break;
					case 1:time_set.tm_mon--;break;
				}
			}
			if(first_in==0&&page==5&&set_select%3==2)
			{
				switch(next%3)
				{
					case 0:time_set.tm_mday++;break;
					case 1:time_set.tm_mday--;break;
				}
			}
			if(first_in==0&&page==5&&next%3==2)		
			{
				set_select++;
			}				
			//page6
			if(first_in==0&&page==6&&next%2==0)
			{
				if(alarm_flag ==0)
					alarm_flag =1;
				else alarm_flag =0;
			}
			if(first_in==0&&page==6&&next%2==1)
			{
				page_before =page;
				first_in =1;
				page=7;
				next=0;
			}
			//page7
			if(first_in==0&&page==7&&set_select%3==0)
			{
				switch(next%3)
				{
					case 0:alarm_time.tm_hour++;break;
					case 1:alarm_time.tm_hour--;break;
				}
			}
			if(first_in==0&&page==7&&set_select%3==1)
			{
				switch(next%3)
				{
					case 0:alarm_time.tm_min++;break;
					case 1:alarm_time.tm_min--;break;
				}
			}
			if(first_in==0&&page==7&&set_select%3==2)
			{
				switch(next%3)
				{
					case 0:alarm_time.tm_sec++;break;
					case 1:alarm_time.tm_sec--;break;
				}
			}
			if(first_in==0&&page==7&&next%3==2)		
			{
				set_select++;
			}	
			
			//page8
			if(first_in==0&&page==8&&next%2==0)
			{
				first_in =1;
				if(stopwatch_flag ==0){
					stopwatch_flag =1;			
				}
				else{ 
					stopwatch_flag =0;
				}
			}
			if(first_in==0&&page==8&&next%2==1)
			{
				first_in =1;
				next=0;
				stopwatch_time.tm_hour=0;
				stopwatch_time.tm_min=0;
				stopwatch_time.tm_sec=0;
				stopwatch_flag =0;
			}
			
			
			
			first_in =0;
			EXTI_ClearFlag(EXTI_Line7);                         //清除标志位
		}
		
		
		
		
		//back键
		if(EXTI_GetFlagStatus(EXTI_Line8) != RESET)           //检测中断标志位
		{
			Delay_ms(200);
			while(GPIO_ReadInputDataBit(C3_GPIO_PORT,C3_GPIO_PIN)==RESET);
			printf("3");
			OLED_CLS();
			//page4,5修改时间和日期需要重新写RTC寄存器
			if(page==4||page==5)
			{
				if(page==4){
					systmtime.tm_hour=time_set.tm_hour;
					systmtime.tm_min=time_set.tm_min;
					systmtime.tm_sec=time_set.tm_sec;
				}
				else{
					systmtime.tm_year=time_set.tm_year;
					systmtime.tm_mon=time_set.tm_mon;
					systmtime.tm_mday=time_set.tm_mday;				
				}
				Time_Adjust(&systmtime);/*配置RTC寄存器 */
				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);/*向BKP_DR1寄存器写入标志，说明RTC已在运行*/
			}
			if(page==3||page==6)
				page_before=2;
			if(page==2)
				page_before =0;
			page=page_before;
			next=0;
			EXTI_ClearFlag(EXTI_Line8);                         //清除标志位
		}
		
		if(EXTI_GetFlagStatus(EXTI_Line9) != RESET)           //检测中断标志位
		{
			struct rtc_time debug_time_set;
			Delay_ms(200);
			while(GPIO_ReadInputDataBit(C4_GPIO_PORT,C4_GPIO_PIN)==RESET);
			//按下按键，通过串口修改时间
				/*使用串口接收设置的时间，输入数字时注意末尾要加回车*/
				Time_Regulate_Get(&debug_time_set);
				/*用接收到的时间设置RTC*/
				Time_Adjust(&debug_time_set);
				//向备份寄存器写入标志
				BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
				printf("4");
				EXTI_ClearFlag(EXTI_Line9); 

			} 


		
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
