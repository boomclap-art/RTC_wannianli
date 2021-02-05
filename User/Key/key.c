#include "key.h"
static void NVIC_config(void)
{
	NVIC_InitTypeDef   NVIC_InitStruct;                        //NVIC配置结构体
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);             //配置为组1
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;               //选择中断线
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;      //主优先级为0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;             //子优先级为1
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;                //使能NVIC
	
	NVIC_Init(&NVIC_InitStruct); 
	
}

void KEY_GPIO_Config(void)
{		
		//定义结构体
		GPIO_InitTypeDef GPIO_InitStructure;
		EXTI_InitTypeDef  EXTI_InitStruct;
		
		NVIC_config();
		
		
		//开启相关的GPIOC外设时钟
		RCC_APB2PeriphClockCmd( KEY_GPIO_CLK, ENABLE); 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = C1_GPIO_PIN;	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  
		GPIO_Init(C1_GPIO_PORT, &GPIO_InitStructure);	
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource6);   //选择外部中断源
	  EXTI_InitStruct.EXTI_Line    = EXTI_Line6;                //
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;       //配置为中断
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;     
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	  EXTI_Init(&EXTI_InitStruct);                              
		GPIO_InitStructure.GPIO_Pin = C2_GPIO_PIN;
		GPIO_Init(C2_GPIO_PORT, &GPIO_InitStructure);		
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource7);   //选择外部中断源
	  EXTI_InitStruct.EXTI_Line    = EXTI_Line7; 
		EXTI_Init(&EXTI_InitStruct);                             
		GPIO_InitStructure.GPIO_Pin = C3_GPIO_PIN;
		GPIO_Init(C3_GPIO_PORT, &GPIO_InitStructure);	
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource8);   //选择外部中断源
	  EXTI_InitStruct.EXTI_Line    = EXTI_Line8; 
		EXTI_Init(&EXTI_InitStruct);                              //初始化函数
		GPIO_InitStructure.GPIO_Pin = C4_GPIO_PIN;
		GPIO_Init(C4_GPIO_PORT, &GPIO_InitStructure);
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource9);   //选择外部中断源
	  EXTI_InitStruct.EXTI_Line    = EXTI_Line9; 
		EXTI_Init(&EXTI_InitStruct);                              //初始化函数


		//设置引脚模式为通用推挽输出
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		//初始化列1
		GPIO_InitStructure.GPIO_Pin = R1_GPIO_PIN;
		GPIO_Init(R1_GPIO_PORT, &GPIO_InitStructure);
		GPIO_ResetBits ( R1_GPIO_PORT, R1_GPIO_PIN );



}

