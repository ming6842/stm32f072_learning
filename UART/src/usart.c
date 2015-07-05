#include "usart.h"


void USART1_Initialization(void){

    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF ;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Connect USART pins to AF */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);   // USART1_TX
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);  // USART1_RX


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


void USART3_Initialization(void){

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3 , ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF ;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* Connect USART pins to AF */
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_1);   // USART1_TX
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_1);  // USART1_RX

    USART_InitTypeDef USART_InitStructure;

    USART_DeInit(USART3);
    USART_InitStructure.USART_BaudRate = 57600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);
    USART_Cmd(USART3, ENABLE);

    USART_ClearFlag(USART3, USART_FLAG_TC);

    USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

    /* NVIC Initialization */
    NVIC_InitTypeDef NVIC_InitStruct = {
      .NVIC_IRQChannel = USART3_4_IRQn,
      .NVIC_IRQChannelPriority = 0,
      .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&NVIC_InitStruct);



}




uint8_t uart1_data , uart3_data ;

void USART1_IRQHandler(void)
{
  
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
    uart1_data = USART_ReceiveData(USART1);

    //USART_SendData(USART1, uart1_data);

    GPIOC->ODR ^= GPIO_Pin_6;
  }

}


void USART3_4_IRQHandler(void){

  if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
    uart3_data = USART_ReceiveData(USART3);

    GPIOC->ODR ^= GPIO_Pin_7;
  }

}