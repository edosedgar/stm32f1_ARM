#include <stm32f10x_gpio.h>
#include <InitPeripheral.c>
#include <stm32f10x_tim.h>
#include <stm32f10x.h>
#include <SinSound.c>

void TIM6_DAC_IRQHandler(void);
void TIM3_IRQHandler(void);

uint16_t Tick=0;

void Delay(volatile uint32_t nCount)
{
    for (; nCount > 0; nCount--);
};

//**************************************************************************************
int main(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	ClearSoundBuffers();
	TIM_Configuration();
	uint8_t i=0;
	while (1)
    {
		OutFrequence(200,1,500,100);
		OutFrequence(900,0,500,100);
		/*OutFrequence(277,0,500,750);
		OutFrequence(294,0,500,750);
		OutFrequence(311,0,500,750);
		OutFrequence(329,0,500,750);
		OutFrequence(349,0,500,750);
		OutFrequence(370,0,500,750);
		OutFrequence(392,0,500,750);
		OutFrequence(415,0,500,750);
		OutFrequence(440,0,500,750);
		OutFrequence(466,0,500,750);
		OutFrequence(494,0,500,750);*/
    };
}

//**************************************************************************************
void TIM3_IRQHandler(void)
{
	//Render Channel2

	uint8_t i;
	for (i=0;i<3;i++)
	{
		if (SoundChannel[i].State==1)
		{
			SoundChannel[i].Cnt++;
			if (SoundChannel[i].Cnt==SoundChannel[i].Arr)
			{
				SoundChannel[i].Cnt=0;
				SoundRender(i);
			}
		}
	}
	DAC->DHR12R1=(SoundChannel[0].DACData+SoundChannel[1].DACData+SoundChannel[2].DACData);

	if (Tick==499)
	{
		Tick=0;
		DelayTimeRender();
	};
	Tick++;
TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}
//**************************************************************************************
void TIM6_DAC_IRQHandler(void)
{

	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
}
//**************************************************************************************
