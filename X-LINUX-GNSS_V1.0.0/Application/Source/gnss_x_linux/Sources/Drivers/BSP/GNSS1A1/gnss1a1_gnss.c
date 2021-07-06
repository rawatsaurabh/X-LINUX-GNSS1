/**
 ******************************************************************************
 * @file    gnss1a1_gnss.c
 * @author  SRA
 * @brief   This file provides a set of functions needed to manage the GNSS
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics International N.V.
 * All rights reserved.</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other
 *    contributors to this software may be used to endorse or promote products
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under
 *    this license is void and will automatically terminate your rights under
 *    this license.
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "gnss1a1_gnss.h"
#define SERIALTERMINAL      "/dev/ttySTM2"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/gpio.h>
#define INTERFACE_UART   1
#define INTERFACE_I2C   2
char *portname = SERIALTERMINAL;
int uart_fd , i2c_fd;
int wlen;

#define MAX_LEN 80

/** @addtogroup BSP BSP
 * @{
 */

/** @addtogroup GNSS1A1 GNSS1A1
 * @{
 */

/** @defgroup GNSS1A1_GNSS GNSS1A1 GNSS
 * @{
 */

/** @defgroup GNSS1A1_GNSS_Exported_Variables GNSS1A1 GNSS Exported Variables
 * @{
 */
extern void
*GNSSCompObj[GNSS1A1_GNSS_INSTANCES_NBR]; /* This "redundant" line is here to fulfil MISRA C-2012 rule 8.4 */
void *GNSSCompObj[GNSS1A1_GNSS_INSTANCES_NBR];

/**
 * @}
 */

/** @defgroup GNSS1A1_GNSS_Private_Variables GNSS1A1 GNSS Private Variables
 * @{
 */

static GNSS_Drv_t *GNSSDrv[GNSS1A1_GNSS_INSTANCES_NBR];

/**
 * @}
 */

/** @defgroup GNSS1A1_GNSS_Private_Function_Prototypes GNSS1A1 GNSS Private Function Prototypes
 * @{
 */

#if (USE_GNSS1A1_GNSS_TESEO_LIV3F == 1)
static int32_t TESEO_LIV3F_Probe(void);

static void GNSS1A1_GNSS_Rst(void);
#endif

/**
 * @}
 */

/** @defgroup GNSS1A1_GNSS_Exported_Functions GNSS1A1_GNSS Exported Functions
 * @{
 */

// __weak int32_t GNSS1A1_GNSS_GetTick(void)
//{
 // return BSP_ERROR_NONE;
//}


int32_t GNSS1A1_GNSS_Init(uint32_t Instance)
{
  int32_t ret = 0;

  return ret;
}

int32_t GNSS1A1_GNSS_DeInit(uint32_t Instance)
{
  int32_t ret;

  return ret;
}


int openDevice_uart()
{

    uart_fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (uart_fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }
    /*baudrate B9600, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(uart_fd, B9600);

    return 0;

}


int openDevice_i2c()
{

  char filename[40];
  const char *buffer;
  int addr = 0b00111010;        // The I2C address of the Tese-LIV3F Device
  int status = 0;
  int i;
  char read_buf[BUF_SIZE];
  sprintf(filename,"/dev/i2c-1");
  if ((i2c_fd = open(filename,O_RDWR)) < 0) {
      printf("Failed to open the bus.");
      /* ERROR HANDLING; you can check errno to see what went wrong */
      status = -1;
  }

  printf("I2C Device opened Successfully \n.");
  if (ioctl(i2c_fd,I2C_SLAVE,addr) < 0) {
      printf("Failed to acquire bus access and/or talk to slave.\n");
      /* ERROR HANDLING; you can check errno to see what went wrong */
      status = -1;
  }


  const char *gpgll_msg = "$PSTMNMEAREQUEST,100000,0\n\r";

  if (write(i2c_fd,gpgll_msg,1) != 1) {
      /* ERROR HANDLING: i2c transaction failed */
      printf("Failed to write to the i2c bus.\n");
     // buffer = g_strerror(errno);
      printf("\n\n");
  }

}


int GNSS1A1_GNSS_GetMessage(uint8_t *message , int interface)
{

int rdlen;
if(INTERFACE_UART == interface)
{
  rdlen = gnss_read_uart(message);

}
else if(INTERFACE_I2C == interface)
{
  rdlen = gnss_read_i2c(message);
}
else
{

  //for both interfaces paralelly
}
//printf(">%s",message);
return rdlen;
}


int gnss_read_uart(uint8_t *message)
{

  /* simple canonical input */
      u_int8_t buf[83];
      unsigned char *p;
      int rdlen;

      rdlen = read(uart_fd, buf, sizeof(buf) - 1);
      if (rdlen > 0) {
          buf[rdlen] = 0;
          /* first display as hex numbers then ASCII */
          #if 0
          for (p = buf; rdlen-- > 0; p++) {
              printf(" 0x%x", *p);
              if (*p < ' ')
                  *p = '.';   /* replace any control chars */
          }
          #endif

      } else if (rdlen < 0) {
          printf("Error from read: %d: %s\n", rdlen, strerror(errno));
      } else {  /* rdlen == 0 */
          printf("Nothing read. EOF?\n");
      }

    strcpy( message, buf);

}

int gnss_read_i2c(uint8_t *message)
{
    const char *buffer;
    int status = 0;
    int i;
    char read_buf[BUF_SIZE];

     //unsigned char reg = 0x10; // Device register to access
    //buf[0] = reg;
  //  const char *gpgll_msg = "$PSTMNMEAREQUEST,100000,0\n\r";

  //  if (write(i2c_fd,gpgll_msg,1) != 1) {
        /* ERROR HANDLING: i2c transaction failed */
    //    printf("Failed to write to the i2c bus.\n");
       // buffer = g_strerror(errno);
      //  printf("\n\n");
  //  }

  //  for(read_buf[BUF_SIZE] = 0;read_buf[180] != 0xff;) {
        // Using I2C Read
        if (read(i2c_fd,read_buf,BUF_SIZE) != 0) {
            /* ERROR HANDLING: i2c transaction failed */
            for(i = 0;i < BUF_SIZE ; i++)
            {
		    if(read_buf[i] != 0xff)
        {
          message[i] = read_buf[i];
		 //	printf("%c",read_buf[i]);
    }
  }
    message[i] = '\0';
  //  strcpy( message, read_buf);
        } else {
            printf("Read failed");
        	}
    //	}

  //  strcpy( message, read_buf);
    status = strlen(message);
  //  	printf("length == %d \n",strlen(message));
  //    printf("length == %d \n",strlen(read_buf));

  //  printf("\n");
    return status;
}


int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= CLOCAL | CREAD;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    tty.c_lflag |= ICANON | ISIG;  /* canonical input */
    tty.c_lflag &= ~(ECHO | ECHOE | ECHONL | IEXTEN);

    tty.c_iflag &= ~IGNCR;  /* preserve carriage return */
    tty.c_iflag &= ~INPCK;
    tty.c_iflag &= ~(INLCR | ICRNL | IUCLC | IMAXBEL);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);   /* no SW flowcontrol */

    tty.c_oflag &= ~OPOST;

    tty.c_cc[VEOL] = 0;
    tty.c_cc[VEOL2] = 0;
    tty.c_cc[VEOF] = 0x04;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}


int32_t GNSS1A1_GNSS_ReleaseMessage(uint32_t Instance, const GNSS1A1_GNSS_Msg_t *Message)
{
  int32_t ret;

  return ret;
}

int32_t GNSS1A1_GNSS_Send(uint32_t Instance, const GNSS1A1_GNSS_Msg_t *Message)
{
  int32_t ret;

  return ret;
}

int32_t GNSS1A1_GNSS_Wakeup_Status(uint32_t Instance, uint8_t *status)
{
  int32_t ret;

 (readgpio() == 0)?(*status = 0) : (*status = 1);

  return ret;
}




static int readgpio()
{
   int val;
  	struct gpiohandle_request req;
  	struct gpiohandle_data data;
  	char chrdev_name[20];
  	int gpiochip_fd, ret;

  	strcpy(chrdev_name, "/dev/gpiochip0");

  	/*  Open device: gpiochip0 for GPIO bank A */
  	gpiochip_fd = open(chrdev_name, 0);
  	if (gpiochip_fd == -1) {
  		ret = -errno;
  		fprintf(stderr, "Failed to open %s\n", chrdev_name);

  		return ret;
  	}

  	/* request GPIO line: GPIO_A_5 */
  	req.lineoffsets[0] = 5;
  	req.flags = GPIOHANDLE_REQUEST_INPUT;
  	memcpy(req.default_values, &data, sizeof(req.default_values));
  	strcpy(req.consumer_label, "gnss_wakeup");
  	req.lines  = 1;

  	ret = ioctl(gpiochip_fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
  	if (ret == -1) {
  		ret = -errno;
  		fprintf(stderr, "Failed to issue GET LINEHANDLE IOCTL (%d)\n",
  			ret);
  	}
  	if (close(gpiochip_fd) == -1)
  		perror("Failed to close GPIO character device file");

  	ret = ioctl(req.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);
    val = data.values[0]?1:0;
  	/*  release line */
  	ret = close(req.fd);
  	if (ret == -1) {
  		perror("Failed to close GPIO LINEHANDLE device file");
  		ret = -errno;
  	}
  	return val;
  }


int32_t GNSS1A1_GNSS_Reset(uint32_t Instance)
{
  int32_t ret = 0;


  return ret;
}

void GNSS1A1_GNSS_BackgroundProcess(uint32_t Instance)
{
  (void)Instance;

  TESEO_LIV3F_I2C_BackgroundProcess();
#if (USE_FREE_RTOS_NATIVE_API)
  vTaskDelay(portTICK_PERIOD_MS * 5U); /* SO: not sure it is required */
#else
#if (USE_AZRTOS_NATIVE_API)
  tx_thread_sleep(((TX_TIMER_TICKS_PER_SECOND / 100)));  // wait 10mS /* SO: not sure it is required */
#endif /* USE_AZRTOS_NATIVE_API */
#endif /* USE_FREE_RTOS_NATIVE_API */
}

/**
 * @}
 */

/** @defgroup GNSS1A1_MOTION_SENSOR_Private_Functions IKS01A2 MOTION SENSOR Private Functions
 * @{
 */

#if (USE_GNSS1A1_GNSS_TESEO_LIV3F  == 1)
/**
 * @brief  Register Bus IOs for instance 0
 * @retval BSP status
 */
static int32_t TESEO_LIV3F_Probe(void)
{
  TESEO_LIV3F_IO_t            io_ctx;
  static TESEO_LIV3F_Object_t teseo_liv3f_obj;

  int32_t ret;
  GNSS1A1_GNSS_Rst();
  return ret;
}



static void GNSS1A1_GNSS_Rst(void)
{
  //printf("Caaling the gpiolib");
  //  gpiolib();
}
#if 0
static void GNSS1A1_GNSS_RegisterCallbacks(void)
{
#if (USE_I2C == 1)

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
  // FIXME: check the return value instead
  (void)GNSS1A1_RegisterRxCb(GNSS1A1_GNSS_I2C_RxCb);
  (void)GNSS1A1_RegisterErrorCb(GNSS1A1_GNSS_I2C_ErrorCb);
  (void)GNSS1A1_RegisterAbortCb(GNSS1A1_GNSS_I2C_AbortCb);
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

#else

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
  // FIXME: check the return value instead
 // (void)GNSS1A1_RegisterRxCb(GNSS1A1_GNSS_UART_RxCb);
// (void)GNSS1A1_RegisterErrorCb(GNSS1A1_GNSS_UART_ErrorCb);
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */

#endif /* USE_I2C */
}

#if (USE_I2C == 1)

#if (USE_HAL_I2C_REGISTER_CALLBACKS == 1)
void GNSS1A1_GNSS_I2C_RxCb(I2C_HandleTypeDef *hi2c)
{
  (void)(hi2c);
  TESEO_LIV3F_I2C_RxCb();
}

void GNSS1A1_GNSS_I2C_ErrorCb(I2C_HandleTypeDef *hi2c)
{
  (void)(hi2c);
  TESEO_LIV3F_I2C_ErrorCb();
}

void GNSS1A1_GNSS_I2C_AbortCb(I2C_HandleTypeDef *hi2c)
{
  (void)(hi2c);
  TESEO_LIV3F_I2C_AbortCb();
}
#endif /* USE_HAL_I2C_REGISTER_CALLBACKS */

#else

#if (USE_HAL_UART_REGISTER_CALLBACKS == 1)
void GNSS1A1_GNSS_UART_RxCb(UART_HandleTypeDef *huart)
{
  (void)(huart);
  TESEO_LIV3F_UART_RxCb();
}

void GNSS1A1_GNSS_UART_ErrorCb(UART_HandleTypeDef *huart)
{
  (void)(huart);
  TESEO_LIV3F_UART_ErrorCb();
}
#endif /* USE_HAL_UART_REGISTER_CALLBACKS */
#endif
#endif /* USE_I2C */

#endif

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
