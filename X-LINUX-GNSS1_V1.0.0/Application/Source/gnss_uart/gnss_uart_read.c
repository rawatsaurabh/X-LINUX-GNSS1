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
/*! \file gnss_uart_read.c
 *
 *  \author Saurab RAWAT
 *
 *  \brief Sample Application to read the Teseo-LIV3F GNSS data from X-NUCLEO-GNSS1 using UART(ttySTM2).
 *
 */
/*
 ******************************************************************************
 * INCLUDES
 ******************************************************************************
 */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <linux/gpio.h>
#include <sys/ioctl.h>



/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */
 #define BUFF_LEN 	83
 #define DEBUG 		0
 #define SERIALTERMINAL      "/dev/ttySTM2"

/*
 ******************************************************************************
 * MAIN FUNCTION
 ******************************************************************************
 */
int set_interface_attribs(int fd, int speed);
static int GNSS_Reset();


int main()
{
    char *portname = SERIALTERMINAL;
    int fd;


    /*Reset The GNSS Device */
    GNSS_Reset();

    /*Open the device file*/
    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }
    /*baudrate 9600, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(fd, B9600);

    /* Continously read the data */
    do {
        u_int8_t buf[BUFF_LEN];
        int rdlen;

        rdlen = read(fd, buf, sizeof(buf) - 1);
        if (rdlen > 0) {
            buf[rdlen] = 0;
            #if DEBUG
            /* display as hex numbers  */
            printf("Read %d:", rdlen);
            for (p = buf; rdlen-- > 0; p++) {
                printf(" 0x%x", *p);
                if (*p < ' ')
                    *p = '.';   /* replace any control chars */
            }
            #endif
            printf("%s\n", buf);
        } else if (rdlen < 0) {
            printf("Error from read: %d: %s\n", rdlen, strerror(errno));
        } else {  /* rdlen == 0 */
            printf("Nothing read. EOF?\n");
        }
        /* repeat read */
    } while (1);
}

/*Set Terimal Attributes*/
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