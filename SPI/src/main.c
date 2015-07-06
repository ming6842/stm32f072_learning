
#include "main.h"

 volatile uint8_t damn_just_a_tick;
 volatile uint16_t tick = 0;

void SysTick_Handler(void) {

  tick++;

  switch (tick) {
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


static inline void Delay_1us(uint32_t);
static inline void Delay_1us(uint32_t nCnt_1us)
{
  volatile uint32_t nCnt;

  for (; nCnt_1us != 0; nCnt_1us--)
    for (nCnt = 45; nCnt != 0; nCnt--);
}

void LED_Configuration(void)
{
      /* GPIOC clock enable */
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

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

  LED_Configuration();
  USART1_Initialization();
  USART3_Initialization();
  USART3_dma_init();
  SPI1_Configuration();

  GPIOC->ODR ^= GPIO_Pin_6;
  GPIOC->ODR ^= GPIO_Pin_7;
  GPIOC->ODR ^= GPIO_Pin_8;
  GPIOC->ODR ^= GPIO_Pin_9;

	SysTick_Config(SystemCoreClock/500);

      /* Initial transmission */
      spi1_dma_enable_receive_fast(spi_buffer);
      spi1_dma_transmit_fast(testspi);

	while(1){

      if(dma_complete_flag==1){

        dma_complete_flag=0;

        USART_SendData(USART3, '\n');
        usart3_dma_send_text(uart_from_spi_buffer);

      }

  }

}
