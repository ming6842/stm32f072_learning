#include "main.h"


void SysTick_Handler(void) {
  static uint16_t tick = 0;

  switch (tick++) {
  	case 100:
  		tick = 0;

      GPIOC->ODR ^= GPIO_Pin_6;
      GPIOC->ODR ^= GPIO_Pin_7;
      GPIOC->ODR ^= GPIO_Pin_8;
      GPIOC->ODR ^= GPIO_Pin_9;

  		break;
  }
}

void RCC_Configuration(void)
{
      /* GPIOC clock enable */
       RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
       RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
       RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
}

void GPIO_Configuration(void){

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void DAC_Configuration(void){

    DAC_InitTypeDef DAC_InitStructure = {
       .DAC_Trigger = DAC_Trigger_Software,
       .DAC_Trigger = DAC_WaveGeneration_None,
       .DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1,
       .DAC_OutputBuffer = DAC_OutputBuffer_Enable,
    };
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
}

int main(void)
{

  RCC_Configuration();
  GPIO_Configuration();
  DAC_Configuration();
  SysTick_Config(SystemCoreClock/1000);

  uint16_t ctr = 0;

	while(1)
  {
     
     DAC_SetChannel1Data(DAC_Align_12b_R, ctr); // output "--00 0000 0000 1010" , e.g. 10 to register 
     ctr++;
     if (ctr >= 4096) ctr = 0;

  }

}
