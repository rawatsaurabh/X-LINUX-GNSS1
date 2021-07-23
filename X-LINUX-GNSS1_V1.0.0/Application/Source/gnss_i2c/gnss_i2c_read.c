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
/*! \file gnss_i2c_read.c
 *
 *  \author Saurab RAWAT
 *
 *  \brief Sample Application to read the Teseo-LIV3F GNSS data from X-NUCLEO-GNSS1
 *  using I2C.
 *
 */
/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/gpio.h>
#include <sys/ioctl.h>



/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */
#define BUF_SIZE 180
#define TESEO_LIV3F_I2C_ADDRESS 0x3A
#define GNSS_I2C_DEVICE "/dev/i2c-1"
#define SUCCESS 0
#define FILE_OPEN_FAIL_ERR -1
#define IOCTL_I2C_ERR -2
#define IOCTL_I2C_WRITE_ERR -3
#define IOCTL_I2C_READ_ERR -4

static int GNSS_Reset();

/*
 ******************************************************************************
 * MAIN FUNCTION
 ******************************************************************************
 */

int main() {
    int file;
    char filename[40];
    int addr = TESEO_LIV3F_I2C_ADDRESS;        
    int status = 0;
    int i;
    char read_buf[BUF_SIZE];
    sprintf(filename,GNSS_I2C_DEVICE);

    /*Reset The GNSS Device */
    GNSS_Reset();

    /*Open the device file*/
    if ((file = open(filename,O_RDWR)) < 0) {
        printf("Failed to open the bus.");
    /*device opening failed*/
        status = FILE_OPEN_FAIL_ERR;
    }

    printf("GNSS Device opened Successfully \n.");
    if (ioctl(file,I2C_SLAVE,addr) < 0) {
        printf("IOCTL : Failed to acquire I2C bus \n");
        status = IOCTL_I2C_ERR;
    }

    printf("I2C bus acquired Successfully \n.");

    /*GNSS Message to get the NMEA data on I2C*/
    const char *gpgll_msg = "$PSTMNMEAREQUEST,100000,0\n\r";

    while(1) {

    if (write(file,gpgll_msg,1) != 1) {
        /*Failure in Writing to the device */
        printf("Failed to write to the i2c bus.\n");
        status = IOCTL_I2C_WRITE_ERR;
    }


    for(i=0;read_buf[i] != 0xff;) {
    /*Read the continous GNSS stream*/
        if (read(file,read_buf,BUF_SIZE) != 0) {
            for(i = 0;i < BUF_SIZE ; i++)
		    if(read_buf[i] != 0xff)
			printf("%c",read_buf[i]);
        } else {
            printf("Read failed");
            status = IOCTL_I2C_READ_ERR;
        	}
    	}
    }

    printf("\n");
    return status;
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
