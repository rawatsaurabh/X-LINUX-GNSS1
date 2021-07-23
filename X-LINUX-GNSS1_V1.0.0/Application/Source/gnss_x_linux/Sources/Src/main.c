/******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2019 STMicroelectronics</center></h2>
  *
  * Licensed under ST MYLIBERTY SOFTWARE LICENSE AGREEMENT (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/myliberty
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied,
  * AND SPECIFICALLY DISCLAIMING THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
******************************************************************************/
/*! \file main.c
 *
 *  \author Saurab RAWAT
 *
 *  \brief Sample Application to read the Teseo-LIV3F GNSS data from X-NUCLEO-GNSS1
 * and upload it to ST Asset Tracking Dashboard
 *
 */
/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include "main.h"
#include "app_gnss.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/gpio.h>
static int GNSS_Reset();

/*
 ******************************************************************************
 * MAIN FUNCTION
 ******************************************************************************
 */

int main(void)
{
  GNSS_Reset();
  MX_GNSS_Init();
  while (1)
  {

  }
}


static int GNSS_Reset()
{

  	struct gpiohandle_request req;
  	struct gpiohandle_data data;
  	char chrdev_name[20];
  	int fd, ret;

  	strcpy(chrdev_name, "/dev/gpiochip3");

  	/*  Open device: gpiochip0 for GPIO bank A */
  	fd = open(chrdev_name, 0);
  	if (fd == -1) {
  		ret = -errno;
  		fprintf(stderr, "Failed to open %s\n", chrdev_name);

  		return ret;
  	}

  	/* request GPIO line: GPIO_A_14 */
  	req.lineoffsets[0] = 1;
  	req.flags = GPIOHANDLE_REQUEST_OUTPUT;
  	memcpy(req.default_values, &data, sizeof(req.default_values));
  	strcpy(req.consumer_label, "gnss_reset");
  	req.lines  = 1;

  	ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
  	if (ret == -1) {
  		ret = -errno;
  		fprintf(stderr, "Failed to issue GET LINEHANDLE IOCTL (%d)\n",
  			ret);
  	}
  	if (close(fd) == -1)
  		perror("Failed to close GPIO character device file");

  	/*  Start led blinking */

          data.values[0] = 0;
  	ret = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
  	if (ret == -1) {
  		ret = -errno;
  		fprintf(stderr, "Failed to issue %s (%d)\n",
  					"GPIOHANDLE_SET_LINE_VALUES_IOCTL", ret);
  	}
  	sleep(1);
  	ret = ioctl(req.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);

  	data.values[0] = 1;
  	ret = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
  	if (ret == -1) {
  		ret = -errno;
  		fprintf(stderr, "Failed to issue %s (%d)\n",
  					"GPIOHANDLE_SET_LINE_VALUES_IOCTL", ret);
  	}

  	ret = ioctl(req.fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data);

  	/*  release line */
  	ret = close(req.fd);
  	if (ret == -1) {
  		perror("Failed to close GPIO LINEHANDLE device file");
  		ret = -errno;
  	}
  	return ret;
  }

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
