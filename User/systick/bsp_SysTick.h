#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void Delay_us (uint32_t us);
void Delay_ms(__IO u32 nTime);
void Delay_s(unsigned int ms);




#endif /* __SYSTICK_H */
