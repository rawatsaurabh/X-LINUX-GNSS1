/**
 ******************************************************************************
 * @file    gnss1a1_conf.h
 * @author  SRA
 * @brief   This file contains definitions for the GNSS components bus interfaces
 ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef GNSS1A1_CONF_H
#define GNSS1A1_CONF_H

#ifdef __cplusplus
extern "C" {
#endif


#define USE_I2C 0U

#define USE_GNSS1A1_GNSS_TESEO_LIV3F	1U


#define GNSS1A1_GNSS_UART_ClearOREF   BSP_USART1_ClearOREF


#define GPIOA    1
#define GPIO_PIN_8 1
#define GPIO_PIN_5 1

#define GNSS1A1_RST_PORT                        GPIOA
#define GNSS1A1_RST_PIN                         GPIO_PIN_8

#define GNSS1A1_WAKEUP_PORT                     GPIOA
#define GNSS1A1_WAKEUP_PIN                      GPIO_PIN_5


/* To be checked */
#define GNSS1A1_UART_IRQHanlder                 BSP_USART1_IRQHanlder

#ifdef __cplusplus
}
#endif

#endif /* GNSS1A1_CONF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
