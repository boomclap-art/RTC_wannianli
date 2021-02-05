/******************** (C) COPYRIGHT 2009 www.armjishu.com ************************
* File Name          : date.h
* Author             : www.armjishu.com Team
* Version            : V1.0
* Date               : 12/1/2009
* Description        : 日期相关函数
*******************************************************************************/
#ifndef __DATE_H
#define __DATE_H

#include "stm32f10x.h"

struct rtc_time {
	int tm_sec;		//秒
	int tm_min;		//分
	int tm_hour;	//时
	int tm_mday;	//天
	int tm_mon;		//月
	int tm_year;	//年
	int tm_wday;	//星期
};
    
void GregorianDay(struct rtc_time * tm);
uint32_t mktimev(struct rtc_time *tm);
void to_tm(uint32_t tim, struct rtc_time * tm);
#endif 
