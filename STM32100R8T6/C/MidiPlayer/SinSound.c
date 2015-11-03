
typedef struct
{
	uint8_t DACData;
	uint8_t State;
	uint32_t Frequency;
	uint16_t StepTact;
	uint16_t StepStopTact;
	uint16_t Arr;
	uint16_t Cnt;
} TSoundChannel;

TSoundChannel SoundChannel [3];
uint8_t NumberChannel;

//**************************************************************************************

void OutFrequence(uint16_t Frequency, uint8_t NumberSoundChannel, uint16_t Duration, uint16_t StopDuration);
void ClearSoundBuffers(void);
void SoundRender(uint8_t NumberSoundChannel);
void DelayTimeRender(void);

//**************************************************************************************

void ClearSoundBuffers(void)
{
	uint8_t i;
	for (i=0;i<3;i++)
	{
		SoundChannel[i].DACData=0;
		SoundChannel[i].State=0;
		SoundChannel[i].Frequency=0;
		SoundChannel[i].StepTact=0;
		SoundChannel[i].StepStopTact=0;
		SoundChannel[i].Arr=0;
		SoundChannel[i].Cnt=0;
	}
	NumberChannel=0;
}

//**************************************************************************************

void OutFrequence(uint16_t Frequency, uint8_t NumberSoundChannel, uint16_t Duration, uint16_t StopDuration)
{
	while ((SoundChannel[NumberSoundChannel].State==1)||(SoundChannel[NumberSoundChannel].StepStopTact!=0)) {};
	SoundChannel[NumberSoundChannel].DACData=0;
	SoundChannel[NumberSoundChannel].Frequency=Frequency;
	SoundChannel[NumberSoundChannel].StepTact=Duration;
	SoundChannel[NumberSoundChannel].StepStopTact=StopDuration;
	SoundChannel[NumberSoundChannel].Arr=(SystemClk/48)/(Frequency*8);
	SoundChannel[NumberSoundChannel].Cnt=0;
	SoundChannel[NumberSoundChannel].State=1;
}

//**************************************************************************************

void SoundRender(uint8_t NumberSoundChannel)
{
	SoundChannel[NumberSoundChannel].DACData=0xFF-SoundChannel[NumberSoundChannel].DACData;
}

//**************************************************************************************

void DelayTimeRender(void)
{
	uint8_t i;
	for (i=0;i<3;i++)
	{
		if (SoundChannel[i].State==1)
		{
			if (SoundChannel[i].StepTact!=0) {SoundChannel[i].StepTact--;} else
			{
				SoundChannel[i].State=0;SoundChannel[i].DACData=0;
				SoundChannel[i].Arr=0;SoundChannel[i].Cnt=0;
			};
		} else {if (SoundChannel[i].StepStopTact!=0) {SoundChannel[i].StepStopTact--;};};
	};
}

