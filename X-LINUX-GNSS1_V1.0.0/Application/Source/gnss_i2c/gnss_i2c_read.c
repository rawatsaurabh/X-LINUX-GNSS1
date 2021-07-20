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
#define BUF_SIZE 180
int main() {
    int file;
    char filename[40];
    int addr = 0b00111010;        // The I2C address of the ADC
    int status = 0;
    int i;
    char read_buf[BUF_SIZE];
    sprintf(filename,"/dev/i2c-1");
    if ((file = open(filename,O_RDWR)) < 0) {
        printf("Failed to open the bus.");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        status = -1;
    }

    printf("I2C Device opened Successfully \n.");
    if (ioctl(file,I2C_SLAVE,addr) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        /* ERROR HANDLING; you can check errno to see what went wrong */
        status = -1;
    }

    printf("I2C bus acquired Successfully \n.");

    const char *gpgll_msg = "$PSTMNMEAREQUEST,100000,0\n\r";

    while(1) {

    if (write(file,gpgll_msg,1) != 1) {
        /* ERROR HANDLING: i2c transaction failed */
        printf("Failed to write to the i2c bus.\n");
       // buffer = g_strerror(errno);
        printf("\n\n");
    }


    for(read_buf[BUF_SIZE] = 0;read_buf[BUF_SIZE] != 0xff;) {
        // Using I2C Read
        if (read(file,read_buf,BUF_SIZE) != 0) {
            /* ERROR HANDLING: i2c transaction failed */
            for(i = 0;i < BUF_SIZE ; i++)
		    if(read_buf[i] != 0xff)
			printf("%c",read_buf[i]);
        } else {
            printf("Read failed");
        	}
    	}
    }

    printf("\n");
    return status;
}
