#ifndef FILE_USART_H
#define FILE_USART_H

#include <string.h>

void USART1_Initialization(void);
void USART3_Initialization(void);
void USART1_IRQHandler(void);
void USART3_dma_init(void);
void usart3_dma_send_text(uint8_t *s);
void USART3_4_IRQHandler(void);

#endif
