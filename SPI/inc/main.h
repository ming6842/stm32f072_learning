#ifndef FILE__MAIN_H
#define FILE__MAIN_H

#include "stm32f0xx_conf.h"
#include "usart.h"
#include "spi.h"

void SysTick_Handler(void);
void LED_Configuration(void);
void USART1_Initialization(void);

#endif
