#include "bsp_beep.h"


void Beep_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BEEP_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_ResetBits(BEEP_GPIO_PORT, GPIO_Pin_8);


}

int melody[] = {50, 50, 50, 50, 200, 200, 200, 400, 400, 500, 500, 500};

void Sound(u16 frq)
{
	u32 time;
	if(frq != 1000)
	{
		time = 500000/((u32)frq);
		BEEP_ON;
		Delay_us(time);
		BEEP_OFF;
		Delay_us(time);
	}else
		Delay_ms(1000);
}
void Sound2(u16 time)
{
    BEEP_ON;
    Delay_ms(time);
    BEEP_OFF;
    Delay_ms(time);
}
void play_successful(void)
{
    int id=0;
    for(id = 0 ;id < 12 ;id++)
    {
        Sound2(melody[id]);
    }
}
void play_failed(void)
{
    int id=0,i=0;
		for(i=0;i<6;i++)
		{
			for(id = 11 ;id >=0 ;id--)
			{
					Sound2(melody[id]);
			}
		}
}


