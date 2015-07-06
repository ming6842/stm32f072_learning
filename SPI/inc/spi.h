#ifndef FILE_SPI_H
#define FILE_SPI_H


extern uint8_t spi_buffer[25];
extern uint8_t uart_from_spi_buffer[25];
extern uint8_t dma_complete_flag;
extern uint8_t testspi[];
void SPI1_Configuration(void);
uint8_t SPI_xfer(SPI_TypeDef *SPIx, uint8_t  WriteByte);
void spi1_dma_enable_receive_fast(uint8_t *buffer_addr);
void spi1_dma_enable_receive(uint8_t *buffer_addr);
void spi1_dma_transmit_fast(uint8_t *buffer_addr);
void spi1_dma_transmit(uint8_t *buffer_addr);

void DMA1_Channel2_3_IRQHandler(void);

#endif
