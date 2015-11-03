
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_flash.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_dma.h>
#include <misc.h>

uint16_t ADCBuffer[1];
//===============================================
void RCC_Configuration(void);
void GPIO_Configuration(void);
void TIM_Configuration(void);
void SPI_Configuration(void);
void ADC_Configuration(void);
void DMA_Configuration(void);
//===============================================
void RCC_Configuration(void)
{
	FLASH_PrefetchBufferCmd(ENABLE);
	FLASH_SetLatency(FLASH_ACR_LATENCY_1);
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    if (RCC_WaitForHSEStartUp()==SUCCESS)
    {
    	RCC_HCLKConfig(RCC_SYSCLK_Div1);
    	RCC_PCLK1Config(RCC_HCLK_Div1);
    	RCC_PCLK2Config(RCC_HCLK_Div1);
    	RCC_ADCCLKConfig(RCC_PCLK2_Div4);
    	RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);
    	RCC_PLLCmd(ENABLE);
    	/* Wait till PLL is ready */
    	while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}
    	/* Select PLL as system clock source */
    	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    	/* Wait till PLL is used as system clock source */
    	while (RCC_GetSYSCLKSource() != 0x08) {}
    	RCC_HSICmd(DISABLE);
    }
};

void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM6 | RCC_APB1Periph_TIM7, ENABLE);
	/* Enable the TIM6 global Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

//==============================================================================

	TIM_DeInit(TIM3);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 200;
	TIM_TimeBaseStructure.TIM_Prescaler = 4800;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x00;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* Channel 1, 2, 3 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 10;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	/* TIM3 Enable */
	TIM_Cmd(TIM3, ENABLE);
	TIM_CtrlPWMOutputs(TIM3, ENABLE);

//==============================================================================

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 48000;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	/* Clear TIM2 update pending flags */
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	/* TIM6 enable counter */
    TIM_Cmd(TIM6, ENABLE);

//==============================================================================

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 243;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	/* Clear TIM2 update pending flags */
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	/* TIM Interrupts enable */
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	/* TIM6 enable counter */
	//TIM_Cmd(TIM7, ENABLE);

//==============================================================================
};

void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_DeInit(ADC1);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1, &ADC_InitStructure);
	/* Convert the ADC1 Channel 10 with 239.5 Cycles as sampling time */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	/* Enable ADCperipheral */
	ADC_Cmd(ADC1, ENABLE);
	ADC_DMACmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	//while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	//while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
};

void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE) ;
	DMA_InitStructure.DMA_BufferSize= 1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCBuffer;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}


void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd (RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig (GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_PinRemapConfig (GPIO_FullRemap_TIM3, ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
};

void SPI_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);
}
