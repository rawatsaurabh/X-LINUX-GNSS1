/**
  *******************************************************************************
  * @file    teseo_liv3f_queue.c
  * @author  SRA
  * @brief   Teseo III buffer queue manager.
  *
  *******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        www.st.com/software_license_agreement_liberty_v2
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ********************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "teseo_liv3f_queue.h"

/* Defines -------------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
#if (USE_FREE_RTOS_NATIVE_API)
  #ifndef configLIBRARY_LOWEST_INTERRUPT_PRIORITY
    #define configLIBRARY_LOWEST_INTERRUPT_PRIORITY 15
  #endif
  static BaseType_t b = configLIBRARY_LOWEST_INTERRUPT_PRIORITY;
#endif /* USE_FREE_RTOS_NATIVE_API */

/* Private functions ---------------------------------------------------------*/

/*
 * ffs - find first bit set
 * @x: the word to search
 *
 * This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 * Source: 
 * git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/include/asm-generic/bitops/ffs.h?h=v4.1.46
 */
static inline int32_t ffs(uint32_t _w)
{
  int32_t r = 1;
  uint32_t w = _w;
  
  if (w == 0U)
  {
    return 0;
  }
  if ((w & 0xffffU) == 0U)
  {
    w >>= 16;
    r += 16;
  }
  if ((w & 0xffU) == 0U)
  {
    w >>= 8;
    r += 8;
  }
  if ((w & 0xfU) == 0U)
  {
    w >>= 4;
    r += 4;
  }
  if ((w & 3U) == 0U)
  {
    w >>= 2;
    r += 2;
  }
  if ((w & 1U) == 0U)
  {
    r += 1;
  }
  return r;
}

#if (USE_FREE_RTOS_NATIVE_API)
/*
 * Wrapper for Semaphore lock.
 */
static inline void semaphore_lock(SemaphoreHandle_t sem)
{
  (void)xSemaphoreTake(sem, 5000);
}

/*
 * Wrapper for Semaphore release.
 */
static inline void semaphore_free(SemaphoreHandle_t sem)
{
  (void)xSemaphoreGive(sem);
}

/*
 * Wrapper for Semaphore lock from interrupt context.
 */
static inline BaseType_t semaphore_lock_irq(SemaphoreHandle_t sem)
{
  return xSemaphoreTakeFromISR(sem,  &b);
}

/*
 * Wrapper for Semaphore release from interrupt context.
 */
static inline void semaphore_free_irq(SemaphoreHandle_t sem)
{
  (void)xSemaphoreGiveFromISR(sem, &b);
}
#endif /* USE_FREE_RTOS_NATIVE_API */

#if (USE_AZRTOS_NATIVE_API)
/*
 * Wrapper for Semaphore lock.
 */
static inline void semaphore_lock(TX_SEMAPHORE sem)
{
  tx_semaphore_get(&sem, TX_WAIT_FOREVER);
}

/*
 * Wrapper for Semaphore release.
 */
static inline void semaphore_free(TX_SEMAPHORE sem)
{
  tx_semaphore_put(&sem);
}

/*
 * Wrapper for Semaphore lock from interrupt context.
 */
static inline UINT semaphore_lock_irq(TX_SEMAPHORE sem)
{
  return tx_semaphore_get(&sem, TX_NO_WAIT);
}

/*
 * Wrapper for Semaphore release from interrupt context.
 */
static inline void semaphore_free_irq(TX_SEMAPHORE sem)
{
  tx_semaphore_put(&sem);  
}
#endif /* USE_AZRTOS_NATIVE_API */

#if (osCMSIS)
#if (osCMSIS < 0x20000U)
/*
 * Wrapper for Semaphore lock.
 */
static inline uint32_t semaphore_lock(osSemaphoreId sem)
{
  return osSemaphoreWait(sem, osWaitForever);
}

/*
 * Wrapper for Semaphore release.
 */
static inline void semaphore_free(osSemaphoreId sem)
{
  osSemaphoreRelease(sem);
}

/*
 * Wrapper for Semaphore lock from interrupt context.
 */
static inline uint32_t semaphore_lock_irq(osSemaphoreId sem)
{
  return osSemaphoreWait(sem, 0);
}

/*
 * Wrapper for Semaphore release from interrupt context.
 */
static inline void semaphore_free_irq(osSemaphoreId sem)
{
  osSemaphoreRelease(sem);
}
#elif (osCMSIS >= 0x20000U)
/*
 * Wrapper for Semaphore lock.
 */
static inline osStatus_t semaphore_lock(osSemaphoreId_t sem)
{
  return osSemaphoreAcquire(sem, osWaitForever);
}

/*
 * Wrapper for Semaphore release.
 */
static inline void semaphore_free(osSemaphoreId_t sem)
{
  osSemaphoreRelease(sem);
}

/*
 * Wrapper for Semaphore lock from interrupt context.
 */
static inline osStatus_t semaphore_lock_irq(osSemaphoreId_t sem)
{
  return osSemaphoreAcquire(sem, 0);
}

/*
 * Wrapper for Semaphore release from interrupt context.
 */
static inline void semaphore_free_irq(osSemaphoreId_t sem)
{
  osSemaphoreRelease(sem);
}
#endif /* (osCMSIS < 0x20000U) */
#endif /* osCMSIS */ 

/*
 * Returns the static instance of a Queue object.
 */
static TESEO_LIV3F_Queue_t *Teseo_Queue_static_alloc(void)
{
  static TESEO_LIV3F_Queue_t Teseo_Queue_Instance;
  return &Teseo_Queue_Instance;
}

/*
 * Returns the index of a message in the queue.
 */
static int32_t __find_msg_idx(const TESEO_LIV3F_Queue_t *pTeseoQueue, const TESEO_LIV3F_Msg_t *pTeseoMsg)
{
  int32_t i;
  
  for (i = 0; i < MAX_MSG_QUEUE; ++i)
  {
    if (&pTeseoQueue->nmea_queue[i] == pTeseoMsg)
    {
      return i;
    }
  }
  
  return -1;
}

/*
 * Checks for buffers not yet released.
 */
static void __check_unreleased_buffer(TESEO_LIV3F_Queue_t *pTeseoQueue)
{
#if ((USE_FREE_RTOS_NATIVE_API) || (USE_AZRTOS_NATIVE_API) || (osCMSIS))
  if (pTeseoQueue->bitmap_unreleased_buffer_irq == 0UL)
  {
    return;
  }
  
  PRINT_DBG("R\n\r");
  pTeseoQueue->bitmap_buffer_readable |= pTeseoQueue->bitmap_unreleased_buffer_irq;
  pTeseoQueue->bitmap_unreleased_buffer_irq = 0;
#endif /* USE_FREE_RTOS_NATIVE_API || USE_AZRTOS_NATIVE_API || osCMSIS */
}

/*
 * Checks for messages longer than MAX_MSG_BUF.
 */
static void check_longer_msg(TESEO_LIV3F_Queue_t *pTeseoQueue, const TESEO_LIV3F_Msg_t *pTeseoMsg, uint32_t idx, int8_t wr_or_rd)
{
  if (pTeseoMsg->len <= (uint32_t)MAX_MSG_BUF)
  {
    return;
  }
  
  uint32_t j;
  uint32_t extra_bufs = (pTeseoMsg->len / (uint32_t)MAX_MSG_BUF) + (((pTeseoMsg->len % (uint32_t)MAX_MSG_BUF) != 0U) ? 1U : 0U);
  
  for (j = 0; j < extra_bufs; j++)
  {
    uint32_t rhs = (idx + j);

    if (wr_or_rd == 1)
    {
      if(rhs < 32U)
      {
        pTeseoQueue->bitmap_buffer_writable &= ~(1UL << rhs);
      }
    }
    else
    {
      if(rhs < 32U)
      {
        pTeseoQueue->bitmap_buffer_writable |= (1UL << rhs);
      }
    }
  }
}

/* Exported functions --------------------------------------------------------*/
TESEO_LIV3F_Queue_t *teseo_queue_init(void)
{
  TESEO_LIV3F_Queue_t *pTeseoQueue = Teseo_Queue_static_alloc();
  int8_t i;
  TESEO_LIV3F_Msg_t *pTeseoMsg;
  
  PRINT_DBG("teseo_queue_init...\n\r" );
  pTeseoQueue->bitmap_buffer_writable = 0;
  for (i = 0;  i < MAX_MSG_QUEUE; ++i)
  {
    pTeseoMsg = &pTeseoQueue->nmea_queue[i];

    pTeseoMsg->buf = &pTeseoQueue->single_message_buffer[i * MAX_MSG_BUF];
    pTeseoMsg->len = 0;
    pTeseoQueue->bitmap_buffer_writable |=  (1UL << (uint8_t)i);
  }
  pTeseoQueue->bitmap_buffer_readable = 0;

#if (USE_FREE_RTOS_NATIVE_API)
  pTeseoQueue->bitmap_unreleased_buffer_irq = 0;
  pTeseoQueue->semaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(pTeseoQueue->semaphore);
#endif /* USE_FREE_RTOS_NATIVE_API */
  
#if (USE_AZRTOS_NATIVE_API  )
  pTeseoQueue->bitmap_unreleased_buffer_irq = 0;  
  tx_semaphore_create(&pTeseoQueue->semaphore, "TeseoQueueSemaphore", 0);  
  tx_semaphore_put(&pTeseoQueue->semaphore);
#endif /* USE_AZRTOS_NATIVE_API */

#if (osCMSIS)
#if (osCMSIS < 0x20000U)
  pTeseoQueue->bitmap_unreleased_buffer_irq = 0;
  osSemaphoreDef(TeseoQueue_SEM);
  pTeseoQueue->semaphore = osSemaphoreCreate(osSemaphore(TeseoQueue_SEM), 1);  
#elif (osCMSIS >= 0x20000U)
  pTeseoQueue->bitmap_unreleased_buffer_irq = 0;
  pTeseoQueue->semaphore = osSemaphoreNew(1, 1, NULL);  
#endif /* (osCMSIS < 0x20000U) */
#endif /* osCMSIS */

  PRINT_DBG("teseo_queue_init: Done\n\r");
  
  return pTeseoQueue;
}

TESEO_LIV3F_Msg_t *teseo_queue_claim_wr_buffer(TESEO_LIV3F_Queue_t *pTeseoQueue)
{
  int32_t i;
  TESEO_LIV3F_Msg_t *pTeseoMsg;
  
#if (USE_FREE_RTOS_NATIVE_API)
  BaseType_t ret;
#endif /* USE_FREE_RTOS_NATIVE_API */

#if (USE_AZRTOS_NATIVE_API)
  UINT ret;
#endif /* USE_AZRTOS_NATIVE_API */

#if (osCMSIS)
#if (osCMSIS < 0x20000U)
  uint32_t ret;
#elif (osCMSIS >= 0x20000U)
  osStatus_t ret;
#endif /* (osCMSIS < 0x20000U) */
#endif /* osCMSIS */
  
#if ((USE_FREE_RTOS_NATIVE_API) || (USE_AZRTOS_NATIVE_API) || (osCMSIS))
  ret = semaphore_lock_irq(pTeseoQueue->semaphore);

  /* semaphore already taken.... */
#if (USE_FREE_RTOS_NATIVE_API)
  if (ret == pdFAIL) {
#endif /* USE_FREE_RTOS_NATIVE_API */
#if (USE_AZRTOS_NATIVE_API)
  if (ret != TX_SUCCESS) {
#endif /* USE_AZRTOS_NATIVE_API */
#if (osCMSIS)
  if (ret != osOK) {
#endif /* osCMSIS */
    return NULL;
  }    
#endif /* USE_FREE_RTOS_NATIVE_API || USE_AZRTOS_NATIVE_API || osCMSIS */  

  /* first available buffer writable */
  i = ffs(pTeseoQueue->bitmap_buffer_writable);
  
  if (i == 0) {
#if ((USE_FREE_RTOS_NATIVE_API) || (USE_AZRTOS_NATIVE_API) || (osCMSIS))
    /* release the semaphore */
    semaphore_free_irq(pTeseoQueue->semaphore);
#endif /* USE_FREE_RTOS_NATIVE_API || USE_AZRTOS_NATIVE_API || osCMSIS */
    PRINT_DBG("-\n\r");
    return NULL;
  }
  
  i--; /* first available buffer writable */
  
  /* buffer no more writable */
  pTeseoQueue->bitmap_buffer_writable &= ~(1U << (uint32_t)i);
  
  __check_unreleased_buffer(pTeseoQueue);

#if ((USE_FREE_RTOS_NATIVE_API) || (USE_AZRTOS_NATIVE_API) || (osCMSIS))
  semaphore_free_irq(pTeseoQueue->semaphore);
#endif /* USE_FREE_RTOS_NATIVE_API || USE_AZRTOS_NATIVE_API || osCMSIS */

  pTeseoMsg = &pTeseoQueue->nmea_queue[i];
  pTeseoMsg->len = 0;
  
  return pTeseoMsg;
}

void teseo_queue_release_wr_buffer(TESEO_LIV3F_Queue_t *pTeseoQueue, TESEO_LIV3F_Msg_t *pTeseoMsg)
{
  int32_t i;

#if (USE_FREE_RTOS_NATIVE_API)
  BaseType_t ret;
#endif /* USE_FREE_RTOS_NATIVE_API */
  
#if (USE_AZRTOS_NATIVE_API)
  UINT ret;
#endif /* USE_AZRTOS_NATIVE_API */

#if (osCMSIS)
#if (osCMSIS < 0x20000U)
  uint32_t ret;
#elif (osCMSIS >= 0x20000U)
  osStatus_t ret;
#endif /* (osCMSIS < 0x20000U) */
#endif /* osCMSIS */
  
  if (pTeseoMsg == NULL) {
    return;
  }
  
  i = __find_msg_idx(pTeseoQueue, pTeseoMsg);
  if (i < 0) {
    /* this should NEVER happen... this means an external buffer was provided */
    return;
  }
  
#if ((USE_FREE_RTOS_NATIVE_API) || (USE_AZRTOS_NATIVE_API) || (osCMSIS))
  ret = semaphore_lock_irq(pTeseoQueue->semaphore);
  
  /* semaphore already taken.... */
#if (USE_FREE_RTOS_NATIVE_API)
  if (ret == pdFAIL) {
#endif /* USE_FREE_RTOS_NATIVE_API */
#if (USE_AZRTOS_NATIVE_API)
  if (ret != TX_SUCCESS) {
#endif /* USE_AZRTOS_NATIVE_API */
#if (osCMSIS)
  if (ret != osOK) {
#endif /* osCMSIS */
    PRINT_DBG("+\n\r" );
    /* mark the buffer as 'un-released'... and go-ahead... */
    pTeseoQueue->bitmap_unreleased_buffer_irq |= (1UL << (uint32_t)i);
    return;
  }

#endif /* USE_FREE_RTOS_NATIVE_API || USE_AZRTOS_NATIVE_API || osCMSIS */
  
  /* mark the buffer as readable */
  pTeseoQueue->bitmap_buffer_readable |= (1UL << (uint32_t)i);

  /* check for longer message */
  check_longer_msg(pTeseoQueue, pTeseoMsg, (uint32_t)i, 1);

#if ((USE_FREE_RTOS_NATIVE_API) || (USE_AZRTOS_NATIVE_API) || (osCMSIS))
  __check_unreleased_buffer(pTeseoQueue);
  semaphore_free_irq(pTeseoQueue->semaphore);
#endif /* USE_FREE_RTOS_NATIVE_API || USE_AZRTOS_NATIVE_API || osCMSIS */
}

const TESEO_LIV3F_Msg_t *teseo_queue_claim_rd_buffer(TESEO_LIV3F_Queue_t *pTeseoQueue)
{
  const TESEO_LIV3F_Msg_t *pTeseoMsg = NULL;
  int32_t i;

  uint8_t rd_buffer_re_try = 1;
  
  while(rd_buffer_re_try == 1U) {
#if ((USE_FREE_RTOS_NATIVE_API) || (USE_AZRTOS_NATIVE_API) || (osCMSIS))
    semaphore_lock(pTeseoQueue->semaphore);
#endif /* USE_FREE_RTOS_NATIVE_API || USE_AZRTOS_NATIVE_API || osCMSIS */

    /* first available readable buffer */
    i = ffs(pTeseoQueue->bitmap_buffer_readable);
    
    if (i == 0) {
#if ((USE_FREE_RTOS_NATIVE_API) || (USE_AZRTOS_NATIVE_API) || (osCMSIS))
      /* release the semaphore */
      semaphore_free(pTeseoQueue->semaphore);
      //PRINT_DBG("No read buffer available... going to sleep...\n\r");
#if (USE_FREE_RTOS_NATIVE_API)
      vTaskDelay(portTICK_PERIOD_MS * 5U);
#endif /* USE_FREE_RTOS_NATIVE_API */
#if (USE_AZRTOS_NATIVE_API)
      tx_thread_sleep(((TX_TIMER_TICKS_PER_SECOND / 100)));  // wait 10mS
#endif /* USE_AZRTOS_NATIVE_API */
#if (osCMSIS)
      osDelay(500U);
#endif /* osCMSIS */     
      continue;
#else   /* no RTOS: bare metal */
      break;
#endif /* USE_FREE_RTOS_NATIVE_API || USE_AZRTOS_NATIVE_API || osCMSIS */
    }

    /* first available readable buffer */
    i -= 1;
    /* buffer no more readable */
    pTeseoQueue->bitmap_buffer_readable &= ~(1UL << (uint32_t)i);
    pTeseoMsg = &pTeseoQueue->nmea_queue[i];

#if ((USE_FREE_RTOS_NATIVE_API) || (USE_AZRTOS_NATIVE_API) || (osCMSIS))
    semaphore_free(pTeseoQueue->semaphore);
#endif /* USE_FREE_RTOS_NATIVE_API || USE_AZRTOS_NATIVE_API || osCMSIS */

    rd_buffer_re_try = 0;
  }
  return pTeseoMsg;
}

void teseo_queue_release_rd_buffer(TESEO_LIV3F_Queue_t *pTeseoQueue, const TESEO_LIV3F_Msg_t *pTeseoMsg)
{ 
  int32_t i;
  
  i = __find_msg_idx(pTeseoQueue, pTeseoMsg);
  if (i < 0) {
    /* this should NEVER happen... this means an external buffer was provided */
    return;
  }
#if ((USE_FREE_RTOS_NATIVE_API) || (USE_AZRTOS_NATIVE_API) || (osCMSIS))
  semaphore_lock(pTeseoQueue->semaphore);
#endif /* USE_FREE_RTOS_NATIVE_API || USE_AZRTOS_NATIVE_API || osCMSIS */

  /* mark the buffer as writeable */
  pTeseoQueue->bitmap_buffer_writable |= (1UL << (uint32_t)i);

  /* check for longer message */
  check_longer_msg(pTeseoQueue, pTeseoMsg, (uint32_t)i, 0);

#if ((USE_FREE_RTOS_NATIVE_API) || (USE_AZRTOS_NATIVE_API) || (osCMSIS))
  semaphore_free(pTeseoQueue->semaphore);
#endif /* USE_FREE_RTOS_NATIVE_API || USE_AZRTOS_NATIVE_API || osCMSIS */
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
