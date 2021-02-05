#include "./systick/bsp_SysTick.h"

void Delay_s(unsigned int ms)
{ 
	unsigned char i;
	for(i=0;i<ms;ms--)
		{
		Delay_ms(1000);
		}
}

void Delay_us (uint32_t us)
{
	uint32_t i;
	SysTick_Config(72);  //一次计数循环的时间
	for(i=0;i<us;i++)
	{
		while( !((SysTick->CTRL) & (1<<16)));						//获取counter计数完成置1，读取后置0  控制及状态寄存器
		
	}
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;//计数完成后停止计数

}

void Delay_ms (uint32_t ms)
{
	uint32_t i;
	SysTick_Config(72000);  //一次计数循环的时间
	for(i=0;i<ms;i++)
	{
		while( !((SysTick->CTRL) & (1<<16)));						//获取counter计数完成置1，读取后置0  控制及状态寄存器
		
	}
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;//计数完成后停止计数

}

