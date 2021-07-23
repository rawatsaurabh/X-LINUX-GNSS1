X-LINUX_GNSS1_V1.0.0 :
==============================================================================================================
First Release of X-LINUX-GNSS1.This package is for reading the NMEA GNSS data from X-NUCLEO-GNSS1A1(Teseo-LIV3F)
plugged to the Arduino Connector of STM32MP157F-DK2 Discovery Board.All the code runs on STM32MP1-DK2 Board . 
For detaield instructions refer "Getting started with X-LINUX-GNSS1 package for developing GNSS Applications on
Linux" inside the Documentation


Code Directories :
==============================================================================================================

There are 4 code directory :

1.gnss_i2c

This is the C code to read the i2c data from /dev/i2c-1

2.gnss_python

This is the python code to read the GNSS Data over uart using Python Script.It uses pynmea2 Library. 
You need to install the pyserial and pynmea2 before using this.

3.gnss_uart

This is the C code to read the uart data from /dev/ttySTM2

3.gnss_x_linux

This is the C code to retrieve and upload the GNSS NMEA data over UART to asset Tracking dashboard -
https://dsh-assetracking.st.com/#/home .This code is derived from X-CUBE-GNSS1A1 with some customization.


System Requirements :
==============================================================================================================
A Linux® PC running under Ubuntu® 18.04 or 20.04 is to be used. The developer can follow the below
link.
https://wiki.st.com/stm32mpu/wiki/PC_prerequisites


Build :
==============================================================================================================

Make sure to source the build environment for STM32MP1-DK2 board ( the step you do while Building kernel) 
Go into each of the code directory and run Make.Respective binary will be generated. For python code you can
directly transfer it to STM32MP1-DK2 board 


Deploy :
==============================================================================================================

Use scp command to transfer built files to STM32MP1-DK2 board 

Run:
==============================================================================================================

Run the application using command : ./<application name>
Run the python code using command : python3 gnss_pynmea2.py


Pre-built Binaries:
==============================================================================================================

You can Find the Pre-built Binaries inside : Application\Binaries