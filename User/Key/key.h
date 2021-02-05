#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"


#define C1_GPIO_PORT    	GPIOC			              //列C1
#define KEY_GPIO_CLK 	         RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC
#define C1_GPIO_PIN		   GPIO_Pin_6			     

#define C2_GPIO_PORT    	GPIOC			               //列C2
#define C2_GPIO_PIN		    GPIO_Pin_7		

#define C3_GPIO_PORT    	GPIOC			              //列C3
#define C3_GPIO_PIN		    GPIO_Pin_8		

#define C4_GPIO_PORT    	GPIOC			               //列C4
#define C4_GPIO_PIN		    GPIO_Pin_9		

#define R1_GPIO_PORT    	GPIOC			               //行R1 
#define R1_GPIO_PIN		  	GPIO_Pin_10		


void KEY_GPIO_Config(void);
#endif
