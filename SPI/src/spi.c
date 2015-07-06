#include "spi.h"


uint8_t SPI_xfer(SPI_TypeDef *SPIx, uint8_t  WriteByte)
{
	uint8_t rxdata;

	SPI_SendData8(SPIx, (uint16_t) WriteByte);

	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	//while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
	rxdata = SPI_ReceiveData8(SPIx);


	return rxdata;
}




void SPI1_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	/* CSN PB12 */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 |
				   GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	// GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	// GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	// GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_0);


	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // 雙線全雙工
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master; // 主模式
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // 數據大小8位
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High; // 時鐘極性，空閒時為低
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge; // 第1個邊沿有效，上升沿為采樣時刻
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft; // NSS信號由軟件產生
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // 8分頻，9MHz
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; // 高位在前
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStruct);

  	SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
	SPI_Cmd(SPI1, ENABLE);

    /* NVIC Initialization */
    NVIC_InitTypeDef NVIC_InitStruct = {
      .NVIC_IRQChannel = DMA1_Channel2_3_IRQn ,
      .NVIC_IRQChannelPriority = 0,
      .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&NVIC_InitStruct);

    
    DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, DISABLE);
    /* Enable DMA RX finish */
    DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);


    
}


void spi1_dma_transmit(uint8_t *buffer_addr){

    // while (DMA_GetFlagStatus( DMA1_FLAG_TC7) == RESET);

    // DMA_ClearFlag(DMA1_FLAG_TC7);
    // DMA_ClearFlag(DMA1_FLAG_GL7);
    // DMA_ClearFlag(DMA1_FLAG_TE7);

    DMA_DeInit(DMA1_Channel3);

    DMA_InitTypeDef DMA_InitStructure = {
    /* Configure DMA Initialization Structure */
        .DMA_PeripheralBaseAddr = (uint32_t)(&(SPI1->DR)),
        .DMA_MemoryBaseAddr = (uint32_t)buffer_addr,
        .DMA_DIR = DMA_DIR_PeripheralDST,
        .DMA_BufferSize =  (uint32_t)8,
        .DMA_PeripheralInc = DMA_PeripheralInc_Disable,
        .DMA_MemoryInc = DMA_MemoryInc_Enable,
        .DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
        .DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
        .DMA_Mode = DMA_Mode_Normal,
        .DMA_Priority = DMA_Priority_Medium,
        .DMA_M2M = DMA_M2M_Disable

    };
  
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel3, ENABLE);
      GPIOC->ODR ^= GPIO_Pin_8;
    

    /* NVIC Initialization */
    NVIC_InitTypeDef NVIC_InitStruct = {
      .NVIC_IRQChannel = DMA1_Ch2_3_DMA2_Ch1_2_IRQn,
      .NVIC_IRQChannelPriority = 0,
      .NVIC_IRQChannelCmd = ENABLE
    };
    NVIC_Init(&NVIC_InitStruct);

    DMA_ITConfig(DMA1_Channel3, DMA1_IT_TC3, ENABLE);

	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
      GPIOC->ODR ^= GPIO_Pin_8;

}



void spi1_dma_transmit_fast(uint8_t *buffer_addr){

	/* Disable DMA1_Channel3 */
	DMA1_Channel3->CCR &= (uint16_t)(~DMA_CCR_EN);


    DMA_InitTypeDef DMA_InitStructure = {
    /* Configure DMA Initialization Structure */
        .DMA_PeripheralBaseAddr = (uint32_t)(&(SPI1->DR)),
        .DMA_MemoryBaseAddr = (uint32_t)buffer_addr,
        .DMA_DIR = DMA_DIR_PeripheralDST,
        .DMA_BufferSize =  (uint32_t)10,
        .DMA_PeripheralInc = DMA_PeripheralInc_Disable,
        .DMA_MemoryInc = DMA_MemoryInc_Enable,
        .DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
        .DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
        .DMA_Mode = DMA_Mode_Normal,
        .DMA_Priority = DMA_Priority_Medium,
        .DMA_M2M = DMA_M2M_Disable

    };
  
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);

    /* Enable DMA1_Channel3 */
    DMA1_Channel3->CCR |= DMA_CCR_EN;

    /* Enable SPI DMA Tx Request */
    SPI1->CR2 |= SPI_I2S_DMAReq_Tx;

}



void spi1_dma_enable_receive(uint8_t *buffer_addr){


    DMA_DeInit(DMA1_Channel2);
    DMA_Cmd(DMA1_Channel2, DISABLE);

    DMA_InitTypeDef DMA_InitStructure = {
    /* Configure DMA Initialization Structure */
        .DMA_PeripheralBaseAddr = (uint32_t)(&(SPI1->DR)),
        .DMA_MemoryBaseAddr = (uint32_t)buffer_addr,
        .DMA_DIR = DMA_DIR_PeripheralSRC,
        .DMA_BufferSize =  (uint32_t)10,
        .DMA_PeripheralInc = DMA_PeripheralInc_Disable,
        .DMA_MemoryInc = DMA_MemoryInc_Enable,
        .DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
        .DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
        .DMA_Mode = DMA_Mode_Normal,
        .DMA_Priority = DMA_Priority_VeryHigh,
        .DMA_M2M = DMA_M2M_Disable

    };
  
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel2, ENABLE);

  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);

}

void spi1_dma_enable_receive_fast(uint8_t *buffer_addr){

    /* Disable DMA1_Channel2 */
    DMA1_Channel2->CCR &= (uint16_t)(~DMA_CCR_EN);

    DMA_InitTypeDef DMA_InitStructure = {
    /* Configure DMA Initialization Structure */
        .DMA_PeripheralBaseAddr = (uint32_t)(&(SPI1->DR)),
        .DMA_MemoryBaseAddr = (uint32_t)buffer_addr,
        .DMA_DIR = DMA_DIR_PeripheralSRC,
        .DMA_BufferSize =  (uint32_t)10,
        .DMA_PeripheralInc = DMA_PeripheralInc_Disable,
        .DMA_MemoryInc = DMA_MemoryInc_Enable,
        .DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte,
        .DMA_MemoryDataSize = DMA_MemoryDataSize_Byte,
        .DMA_Mode = DMA_Mode_Normal,
        .DMA_Priority = DMA_Priority_Medium,
        .DMA_M2M = DMA_M2M_Disable

    };
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);

    /* Enable SPI DMA Tx Request */
    SPI1->CR2 |= SPI_I2S_DMAReq_Rx;
    /* Enable DMA1_Channel2 */
    DMA1_Channel2->CCR |= DMA_CCR_EN;


}


uint8_t spi_buffer[25];
uint8_t uart_from_spi_buffer[25];
uint8_t dma_complete_flag =0;
uint8_t testspi[]= "12345678X01234567890";

void DMA1_Channel2_3_IRQHandler(void){

    if( DMA_GetITStatus(DMA1_IT_TC3) == SET){
      DMA_ClearITPendingBit(DMA1_IT_TC3);
       GPIOC->ODR ^= GPIO_Pin_8;
    }

    if(DMA_GetITStatus(DMA1_IT_TC2) == SET){

      DMA_ClearITPendingBit(DMA1_IT_TC2);
       GPIOC->ODR ^= GPIO_Pin_7;

      for(int i = 0;i<10;i++)
           uart_from_spi_buffer[i] = spi_buffer[i];

         dma_complete_flag=1;


      /* Re-transmission */
      spi1_dma_enable_receive_fast(spi_buffer);
      spi1_dma_transmit_fast(testspi);
       GPIOC->ODR ^= GPIO_Pin_8;

    }


}