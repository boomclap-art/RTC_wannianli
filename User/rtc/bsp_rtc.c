#include "./usart/bsp_usart.h"
#include "./rtc/bsp_rtc.h"
#include "OLED_I2C.h"
#include "./dht11/bsp_dht11.h"
#include "stdio.h"
extern uint8_t str_day[15];
extern uint8_t alarm_flag,stopwatch_flag;
extern uint8_t str_time[15];
extern uint8_t str_canderday[50];
extern uint8_t str_wday[15];
extern uint8_t str_zodiac_sign[15];
extern uint8_t str_time_set[15];
extern uint8_t str_day_set[15];
extern uint8_t str_alarm_time[15];
extern uint8_t str_stop_time[15];
extern uint8_t str_tempandhumidity[20];
extern struct rtc_time time_set;
extern struct rtc_time systmtime;
extern struct rtc_time alarm_time;
extern struct rtc_time stopwatch_time;
extern DHT11_Data_TypeDef dht11_data;

/*星期，生肖用文字ASCII码*/
char const *WEEK_STR[] = {"日", "一", "二", "三", "四", "五", "六"};
char const *zodiac_sign[] = {"猪", "鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗"};

///*英文，星期，生肖用文字ASCII码*/
//char const *en_WEEK_STR[] = { "Sunday","Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//char const *en_zodiac_sign[] = {"Pig", "Rat", "Ox", "Tiger", "Rabbit", "Dragon", "Snake", "Horse", "Goat", "Monkey", "Rooster", "Dog"};
void RTC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



void RTC_CheckAndConfig(struct rtc_time *tm)
{
   	/*在启动时检查备份寄存器BKP_DR1，如果内容不是0xA5A5,
	  则需重新配置时间并询问用户调整时间*/
	if (BKP_ReadBackupRegister(RTC_BKP_DRX) != RTC_BKP_DATA)
	{
		/* 使用tm的时间配置RTC寄存器 */
		Time_Adjust(tm);
		
		/*向BKP_DR1寄存器写入标志，说明RTC已在运行*/
		BKP_WriteBackupRegister(RTC_BKP_DRX, RTC_BKP_DATA);
	}
	else
	{
		
		/* 使能 PWR 和 Backup 时钟 */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
		/* 允许访问 Backup 区域 */
	  PWR_BackupAccessCmd(ENABLE);
		
	  /*LSE启动无需设置新时钟*/
		
#ifdef RTC_CLOCK_SOURCE_LSI		
			/* 使能 LSI */
			RCC_LSICmd(ENABLE);

			/* 等待 LSI 准备好 */
			while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
			{}
#endif

		/*检查是否掉电重启*/
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
		    printf("\r\n\r\n Power On Reset occurred....");
		}
		/*检查是否Reset复位*/
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			printf("\r\n\r\n External Reset occurred....");
		}
	
		printf("\r\n No need to configure RTC....");
		
		/*等待寄存器同步*/
		RTC_WaitForSynchro();
		
		/*允许RTC秒中断*/
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		
		/*等待上次RTC寄存器写操作完成*/
		RTC_WaitForLastTask();
	}
	   /*定义了时钟输出宏，则配置校正时钟输出到PC13*/
	#ifdef RTCClockOutput_Enable
	/* 使能 PWR 和 Backup 时钟 */
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* 允许访问 Backup 区域 */
	  PWR_BackupAccessCmd(ENABLE);
	
	  /* 禁止 Tamper 引脚 */
	  /* 要输出 RTCCLK/64 到 Tamper 引脚,  tamper 功能必须禁止 */	
	  BKP_TamperPinCmd(DISABLE); 
	
	  /* 使能 RTC 时钟输出到 Tamper 引脚 */
	  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
	#endif
	
	  /* 清除复位标志 flags */
	  RCC_ClearFlag();

}


void RTC_Configuration(void)
{
	/* 使能 PWR 和 Backup 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* 允许访问 Backup 区域 */
	PWR_BackupAccessCmd(ENABLE);
	
	/* 复位 Backup 区域 */
	BKP_DeInit();
	
//使用外部时钟还是内部时钟（在bsp_rtc.h文件定义）	
//使用外部时钟时，在有些情况下晶振不起振
//批量产品的时候，很容易出现外部晶振不起振的情况，不太可靠	
#ifdef 	RTC_CLOCK_SOURCE_LSE
	/* 使能 LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	
	/* 等待 LSE 准备好 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* 选择 LSE 作为 RTC 时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* 使能 RTC 时钟 */
	RCC_RTCCLKCmd(ENABLE);
	
	/* 等待 RTC 寄存器 同步
	 * 因为RTC时钟是低速的，内环时钟是高速的，所以要同步
	 */
	RTC_WaitForSynchro();
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
	/* 使能 RTC 秒中断 */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
	/* 设置 RTC 分频: 使 RTC 周期为1s  */
	/* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
	RTC_SetPrescaler(32767); 
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
#else

	/* 使能 LSI */
	RCC_LSICmd(ENABLE);

	/* 等待 LSI 准备好 */
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{}
	
	/* 选择 LSI 作为 RTC 时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	/* 使能 RTC 时钟 */
	RCC_RTCCLKCmd(ENABLE);
	
	/* 等待 RTC 寄存器 同步
	 * 因为RTC时钟是低速的，内环时钟是高速的，所以要同步
	 */
	RTC_WaitForSynchro();
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
	/* 使能 RTC 秒中断 */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
	
	/* 设置 RTC 分频: 使 RTC 周期为1s ,LSI约为40KHz */
	/* RTC period = RTCCLK/RTC_PR = (40 KHz)/(40000-1+1) = 1HZ */	
	RTC_SetPrescaler(40000-1); 
	
	/* 确保上一次 RTC 的操作完成 */
	RTC_WaitForLastTask();
#endif
	
}


void Time_Regulate_Get(struct rtc_time *tm)
{
	  uint32_t temp_num = 0;
		uint8_t day_max=0 ;

	  printf("\r\n=========================设置时间==================");
		
	  do 
	  {
			printf("\r\n  请输入年份(Please Set Years),范围[1970~2038]，输入字符后请加回车:");
			scanf("%d",&temp_num);
			if(temp_num <1970 || temp_num >2038)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
					  
			}
			else
			{	  
				printf("\n\r  年份被设置为: %d\n\r", temp_num);

				tm->tm_year = temp_num;
				break;
			}
	  }while(1);


	 do 
	  {
			printf("\r\n  请输入月份(Please Set Months):范围[1~12]，输入字符后请加回车:");
			scanf("%d",&temp_num);
			if(temp_num <1 || temp_num >12)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
					  
			}
			else
			{	  
				printf("\n\r  月份被设置为: %d\n\r", temp_num);

				tm->tm_mon = temp_num;
				break;
			}
	  }while(1);
		
		/*根据月份计算最大日期*/
		switch(tm->tm_mon)
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:					
						day_max = 31;
					break;
				
				case 4:
				case 6:
				case 9:
				case 11:
						day_max = 30;
					break;
				
				case 2:					
				     /*计算闰年*/
						if((tm->tm_year%4==0) &&
							 ((tm->tm_year%100!=0) || (tm->tm_year%400==0)) &&
							 (tm->tm_mon>2)) 
								{
									day_max = 29;
								} else 
								{
									day_max = 28;
								}
					break;			
			}

		do 
	  {				
			printf("\r\n  请输入日期(Please Set Months),范围[1~%d]，输入字符后请加回车:",day_max);
			scanf("%d",&temp_num);
			
			if(temp_num <1 || temp_num >day_max)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
			}
			else
			{
				printf("\n\r  日期被设置为: %d\n\r", temp_num);

				tm->tm_mday = temp_num;
				break;
			}
	  }while(1);
		
		do 
	  {				
			printf("\r\n  请输入时钟(Please Set Hours),范围[0~23]，输入字符后请加回车:");
			scanf("%d",&temp_num);
			
			if( temp_num >23)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
			}
			else
			{
				printf("\n\r  时钟被设置为: %d\n\r", temp_num);

				tm->tm_hour = temp_num;
				break;
			}
	  }while(1);

		do 
	  {				
			printf("\r\n  请输入分钟(Please Set Minutes),范围[0~59]，输入字符后请加回车:");
			scanf("%d",&temp_num);
			
			if( temp_num >59)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
			}
			else
			{
				printf("\n\r  分钟被设置为: %d\n\r", temp_num);

				tm->tm_min = temp_num;
				break;
			}
	  }while(1);

		do 
	  {				
			printf("\r\n  请输入秒钟(Please Set Seconds),范围[0~59]，输入字符后请加回车:");
			scanf("%d",&temp_num);
			
			if( temp_num >59)
			{
				printf("\r\n 您输入的数字是:%d，不符合要求",temp_num);
			}
			else
			{
				printf("\n\r  秒钟被设置为: %d\n\r", temp_num);

				tm->tm_sec = temp_num;
				break;
			}
	  }while(1);

}

void Time_Adjust(struct rtc_time *tm)
{
	
			/* RTC 配置 */
		RTC_Configuration();

	  /* 等待确保上一次操作完成 */
	  RTC_WaitForLastTask();
		  
	  /* 计算星期 */
	  GregorianDay(tm);

	  /* 由日期计算时间戳并写入到RTC计数寄存器 */
	  RTC_SetCounter(mktimev(tm)-TIME_ZOOM);

	  /* 等待确保上一次操作完成 */
	  RTC_WaitForLastTask();
}
void Alarm_bkp_Read(void)
{
	uint16_t temp;
	temp=BKP_ReadBackupRegister(BKP_DR2);
	alarm_time.tm_hour=(temp)>>8;
	alarm_time.tm_min=(temp&0x0f);
	alarm_flag=BKP_ReadBackupRegister(BKP_DR3);
}

void Alarm_bkp_Write(void)
{	
	BKP_WriteBackupRegister(BKP_DR2,(alarm_time.tm_hour<<8)|(alarm_time.tm_min&0x0f));
//	BKP_WriteBackupRegister(BKP_DR2,(alarm_time.tm_hour<<8)|(alarm_time.tm_min&0x0f));
	BKP_WriteBackupRegister(BKP_DR3,alarm_flag);
}

void Time_Display(uint32_t TimeVar,struct rtc_time *tm)
{
	   static uint32_t FirstDisplay = 1;
	   uint32_t BJ_TimeVar;
	   uint8_t str[200]; // 字符串暂存  	
	   /*  把标准时间转换为北京时间*/
	   BJ_TimeVar =TimeVar + TIME_ZOOM;

	   to_tm(BJ_TimeVar, tm);/*把定时器的值转换为北京时间*/	
	
	  if((!tm->tm_hour && !tm->tm_min && !tm->tm_sec)  || (FirstDisplay))
	  {
	      
	      GetChinaCalendar((u16)tm->tm_year, (u8)tm->tm_mon, (u8)tm->tm_mday, str);	
	      FirstDisplay = 0;
	  }	 	  	
		sprintf((char*)str_day,"%0.4d-%0.2d-%0.2d", tm->tm_year, tm->tm_mon, tm->tm_mday);
		str_day[12]=0;
		sprintf((char*)str_time,"%0.2d:%0.2d:%0.2d",tm->tm_hour, tm->tm_min, tm->tm_sec);
}
void oled_page_display(uint8_t page)
{
	if(page==0)
	{
		OLED_ShowStr(30, 0, str_day, 1);
		OLED_Show_Numphoto(0,2,systmtime.tm_hour/10);
		OLED_Show_Numphoto(26,2,systmtime.tm_hour%10);
		OLED_Show_ClockPots(56, 2);
		OLED_Show_Numphoto(74,2,systmtime.tm_min/10);
		OLED_Show_Numphoto(100,2,systmtime.tm_min%10);
		OLED_DisString_CH(0, 6, "菜单");OLED_DisString_CH(90, 6, "农历");
	}
	if(page==1)
	{
		GetChinaCalendarStr((u16)systmtime.tm_year,(u8)systmtime.tm_mon,(u8)systmtime.tm_mday,str_canderday);
		OLED_DisString_CH(0,2,(char *)str_canderday);
		
		sprintf((char*)str_wday,"星期%s",WEEK_STR[systmtime.tm_wday]);								//星期
		OLED_DisString_CH(0,4,(char *)str_wday);

		sprintf((char*)str_zodiac_sign,"%s年",zodiac_sign[systmtime.tm_year%12-3]);		//调整为相应的农历年份
		OLED_DisString_CH(80,4,(char *)str_zodiac_sign);
		
		OLED_DisString_CH(0,0,"湿");
		sprintf((char*)str_tempandhumidity,":%0.2d.%0.2d",dht11_data.humi_int,dht11_data.humi_deci);
		OLED_ShowStr(16, 0, str_tempandhumidity, 1);
		
		OLED_DisString_CH(60,0,"温");
		sprintf((char*)str_tempandhumidity,":%0.2d.%0.2d",dht11_data.temp_int,dht11_data.temp_deci);
		OLED_ShowStr(76, 0, str_tempandhumidity, 1);
		
		OLED_DisString_CH(0, 6, "菜单");OLED_DisString_CH(90, 6, "公历");
	}
	if(page==2)
	{
		OLED_DisString_CH(0, 0, "校准");
		OLED_DisString_CH(0 , 2,"闹钟");
		OLED_DisString_CH(0 , 4,"计时器");
		OLED_DisString_CH(0 , 6,"图形时钟");
	}
	if(page==3)
	{
		OLED_DisString_CH(0, 0, "时间校准");
		OLED_DisString_CH(0 , 2,"日期校准");
	
	}
	if(page==4)
	{
		sprintf((char*)str_time_set,"%0.2d:%0.2d:%0.2d",time_set.tm_hour, time_set.tm_min, time_set.tm_sec);
		OLED_ShowStr(35 , 3, str_time_set, 1);
		OLED_ShowStr(0, 7,"+", 1);OLED_ShowStr(40, 7, "-", 1);OLED_DisString_CH(70, 6, "下一个");
	}
	if(page==5)
	{
		sprintf((char*)str_day_set,"%0.4d-%0.2d-%0.2d", time_set.tm_year, time_set.tm_mon, time_set.tm_mday);
		OLED_ShowStr(30, 0, str_day_set, 1);
		OLED_ShowStr(0, 7,"+", 1);OLED_ShowStr(40, 7, "-", 1);OLED_DisString_CH(70, 6, "下一个");
	}
	if(page==6)
	{
		sprintf((char*)str_alarm_time,"%0.2d:%0.2d:%0.2d",alarm_time.tm_hour, alarm_time.tm_min, alarm_time.tm_sec);
		OLED_ShowStr(35 , 3, str_alarm_time, 1);
		OLED_DisString_CH(70 , 6, "设置");
		if(alarm_flag==1)		OLED_DisString_CH(0 , 6, "开");
		else		OLED_DisString_CH(0 , 6, "关");
	}
	if(page==7)
	{
		sprintf((char*)str_alarm_time,"%0.2d:%0.2d:%0.2d",alarm_time.tm_hour, alarm_time.tm_min, alarm_time.tm_sec);
		OLED_ShowStr(35 , 3, str_alarm_time, 1);
		OLED_ShowStr(0, 7,"+", 1);OLED_ShowStr(40, 7, "-", 1);OLED_DisString_CH(70, 6, "下一个");
	}
	if(page==8)
	{
		sprintf((char*)str_stop_time,"%0.2d:%0.2d:%0.2d",stopwatch_time.tm_hour, stopwatch_time.tm_min, stopwatch_time.tm_sec);
		OLED_ShowStr(35 , 3, str_stop_time, 1);
		if(stopwatch_flag==0)OLED_DisString_CH(0, 6,"开");
		else	OLED_DisString_CH(0, 6, "停");
		OLED_DisString_CH(70, 6, "复位");
	}
	if(page==9)
	{
		Draw_CircleClock(systmtime.tm_hour,systmtime.tm_min ,systmtime.tm_sec);
//		OLED_ShowStr(35 , 3, "2019214145", 1);
//		OLED_DisString_CH(35 , 5, "杨建");
		
	}


}

