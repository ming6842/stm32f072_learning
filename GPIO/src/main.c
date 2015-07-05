
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
       RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
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

  GPIOC->ODR ^= GPIO_Pin_6;
  GPIOC->ODR ^= GPIO_Pin_7;
  GPIOC->ODR ^= GPIO_Pin_8;
  GPIOC->ODR ^= GPIO_Pin_9;

	SysTick_Config(SystemCoreClock/1000);

	while(1);

}
