#include <stm32f10x_gpio.h>
#include <InitPeripheral.c>
#include <stm32f10x_tim.h>
#include <stm32f10x.h>
#include <SinSound.c>

void TIM6_DAC_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM7_IRQHandler(void);

void Delay(volatile uint32_t nCount)
{
    for (; nCount > 0; nCount--);
};

uint8_t Array[]="984-00,984-00,984-0-2-0-1-**,981-**,981-**,981-*-2-*-4-00%";
//**************************************************************************************
int main(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	ClearSoundBuffers();
	TIM_Configuration();
	uint8_t i=0;
			OutFrequence(20000,0,50,100,Sine);
		OutFrequence(10000,1,50,100,Saw);
		//OutFrequence(800,2,50,100,Square);
	while (1)
    {


		//OutFrequence(1000,0,250,250,Saw);
		//OutFrequence(200,1,250,250,Sine);
		//DoDTMFSound('1',100,50);
    };
}

void DoDTMFSound(uint8_t Symbol,uint8_t T1,uint8_t T2)
{
	switch (Symbol)
	{
	case '1': OutFrequence(697,0,T1,T2,Sine); OutFrequence(1209,1,T1,T2,Sine); break;
	case '2': OutFrequence(697,0,T1,T2,Sine); OutFrequence(1336,1,T1,T2,Sine);break;
	case '3': OutFrequence(697,0,T1,T2,Sine); OutFrequence(1477,1,T1,T2,Sine);break;
	case '4': OutFrequence(770,0,T1,T2,Sine); OutFrequence(1209,1,T1,T2,Sine);break;
	case '5': OutFrequence(770,0,T1,T2,Sine); OutFrequence(1336,1,T1,T2,Sine);break;
	case '6': OutFrequence(770,0,T1,T2,Sine); OutFrequence(1477,1,T1,T2,Sine);break;
	case '7': OutFrequence(852,0,T1,T2,Sine); OutFrequence(1209,1,T1,T2,Sine);break;
	case '8': OutFrequence(852,0,T1,T2,Sine); OutFrequence(1336,1,T1,T2,Sine);break;
	case '9': OutFrequence(852,0,T1,T2,Sine); OutFrequence(1477,1,T1,T2,Sine);break;
	case '*': OutFrequence(941,0,T1,T2,Sine); OutFrequence(1209,1,T1,T2,Sine);break;
	case '0': OutFrequence(941,0,T1,T2,Sine); OutFrequence(1366,1,T1,T2,Sine);break;
	case '#': OutFrequence(941,0,T1,T2,Sine); OutFrequence(1477,1,T1,T2,Sine);break;
	case '-': OutFrequence(20,0,T1,T2,Sine); OutFrequence(20,1,T1,T2,Sine);break;
	case ' ': OutFrequence(20,0,T1,T2,Sine); OutFrequence(20,1,T1,T2,Sine);break;
	}
}
//**************************************************************************************
void TIM2_IRQHandler(void)
{
	//Render Channel1
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	SoundRender(0);
}
//**************************************************************************************
void TIM3_IRQHandler(void)
{
	//Render Channel2
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	SoundRender(1);
}
//**************************************************************************************
void TIM4_IRQHandler(void)
{
	//Render Channel3
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	SoundRender(2);
}
//**************************************************************************************
void TIM7_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	DelayTimeRender();
}
//**************************************************************************************
void TIM6_DAC_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
}
