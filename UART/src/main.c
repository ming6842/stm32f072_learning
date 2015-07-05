
#include "main.h"


void SysTick_Handler(void) {
  static uint16_t tick = 0;

  switch (tick++) {
  	case 100:
  		tick = 0;

      //GPIOC->ODR ^= GPIO_Pin_6;
      //GPIOC->ODR ^= GPIO_Pin_7;
      //GPIOC->ODR ^= GPIO_Pin_8;
      GPIOC->ODR ^= GPIO_Pin_9;
      
    USART_SendData(USART1, 'Y');
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


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF ;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Connect USART pins to AF */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);   // USART1_TX
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);  // USART1_RX

}

void USART1_Initialization(void){

    USART_InitTypeDef USART_InitStructure;

    USART_DeInit(USART1);
    USART_InitStructure.USART_BaudRate = 57600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);

    USART_ClearFlag(USART1, USART_FLAG_TC);

    USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    /* NVIC Initialization */
    NVIC_InitTypeDef NVIC_InitStruct = {
      .NVIC_IRQChannel = USART1_IRQn,
      .NVIC_IRQChannelPriority = 0,
      .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&NVIC_InitStruct);



}

uint8_t uart1_data ;

int main(void)
{

  RCC_Configuration();
  GPIO_Configuration();
  USART1_Initialization();

  GPIOC->ODR ^= GPIO_Pin_6;
  GPIOC->ODR ^= GPIO_Pin_7;
  GPIOC->ODR ^= GPIO_Pin_8;
  GPIOC->ODR ^= GPIO_Pin_9;

	SysTick_Config(SystemCoreClock/1000);

	while(1){


  }

}

void USART1_IRQHandler(void)
{
  
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    uart1_data = USART_ReceiveData(USART1);

    //USART_SendData(USART1, uart1_data);

    GPIOC->ODR ^= GPIO_Pin_6;
  }

}