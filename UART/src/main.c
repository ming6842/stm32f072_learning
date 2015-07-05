
#include "main.h"

 volatile uint8_t damn_just_a_tick;
  static uint16_t tick = 0;

void SysTick_Handler(void) {

  switch (tick++) {
  	case 100:
      damn_just_a_tick = 1;
  		tick = 0;

      //GPIOC->ODR ^= GPIO_Pin_6;
      //GPIOC->ODR ^= GPIO_Pin_7;
      //GPIOC->ODR ^= GPIO_Pin_8;
      GPIOC->ODR ^= GPIO_Pin_9;
  		break;
  }
}

void RCC_Configuration(void)
{
      /* GPIOC clock enable */
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}

void GPIO_Configuration(void){


    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

}


int main(void)
{

  RCC_Configuration();
  GPIO_Configuration();
  USART1_Initialization();
  USART3_Initialization();

  GPIOC->ODR ^= GPIO_Pin_6;
  GPIOC->ODR ^= GPIO_Pin_7;
  GPIOC->ODR ^= GPIO_Pin_8;
  GPIOC->ODR ^= GPIO_Pin_9;

	SysTick_Config(SystemCoreClock/5000);

	while(1){

    if(damn_just_a_tick==1){

      damn_just_a_tick =0;
      GPIOC->ODR ^= GPIO_Pin_8;

      USART_SendData(USART1, 'Y');
      USART_SendData(USART3, 'Y');


    }

  }

}