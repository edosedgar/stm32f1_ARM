#include <stm32f10x_gpio.h>
#include <InitPeripheral.c>
#include <stm32f10x_tim.h>
#include <stm32f10x.h>

#define   SetBit(reg, bit)          reg |= (1<<bit)
#define   ClrBit(reg, bit)          reg &= (~(1<<bit))

void TIM6_DAC_IRQHandler(void);
void TIM7_IRQHandler(void);

uint32_t Line, Point,Flag;

void Delay(volatile uint32_t nCount)
{
    for (; nCount > 0; nCount--);
};

//**************************************************************************************
int main(void)
{
	//Line=0; Point=0;
	RCC_Configuration();
	GPIO_Configuration();
	ADC_Configuration();
	DMA_Configuration();
	TIM_Configuration();
	//TIM7->ARR=0;
	while (1)
    {
		TIM3->CCR2=ADCBuffer[0]/400+10;
		if (TIM3->CCR2>10) {GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_SET);} else {GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_RESET);};
		Delay(100000);
    };
}
//**************************************************************************************
void TIM6_DAC_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	/*if ((Line==10) || (Line==11))
	{
		ClrBit(GPIOC->ODR,6);
	}
	else
	{
		SetBit(GPIOC->ODR,6);
	};
	if (Line>=45)
	{
		//TIM6->ARR=0;
		TIM7->ARR=1;
	}
	Line++;
	if (Line==525) {Line=0;TIM7->ARR=0;};*/
}
//**************************************************************************************
void TIM7_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	/*if ((Point>=1) && (Point<=6))
	{
		GPIOC->ODR=0b01000000;
	}
	else
	{
		GPIOC->ODR=0b11000000;
	};
	if (Point>=10)
	{
		GPIOB->ODR++;
	}
	Point++;
	if (Point==50)
	{
		GPIOB->ODR=0;
		Point=0;

	};*/
}
