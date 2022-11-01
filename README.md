X-LINUX_GNSS1_V1.1.0 Linux Package
==============================================================================================================
The X-LINUX-GNSS1 is a Linux Package running on STM32MPU .This software provides applications for reading the NMEA GNSS data from X-NUCLEO-GNSS1A1(Teseo-LIV3F) or X-NUCLEO-GNSS2A1(Teseo-VIC3DA) plugged to the Arduino Connector of STM32MP157F-DK2 Discovery Board .X-LINUX-GNSS1 includes user space applications and Yocto Recipe (device tree) for the X-NUCLEO-GNSS1A1/X-NUCLEO-GNSS2A1 board ,a library for NMEA protocol support and POSIX thread for task scheduling to ensure better asynchronous message parsing.


X-LINUX-GNSS1 software features:

• Standalone applications to read the NMEA data over UART and I²C
• Complete software to build applications on Linux using Teseo-LIV3F GNSS module and Teseo-VIC3DA module
• Middleware for the NMEA protocol
• POSIX thread task scheduling to ensure better asynchronous message parsing
• Easy portability across different Linux platforms
• Application example to retrieve and parse GNSS data and send them to DSHASSETRACKING for live tracking
• Python example to read the NMEA data over UART


![image](https://user-images.githubusercontent.com/8255773/199159792-141395c2-5464-4870-9770-633b9983aa09.png)


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
  
Related Information and Documentation:
==============================================================================================================

  -X-NUCLEO-GNSS2A1 : https://www.st.com/content/ccc/resource/technical/document/user_manual/group2/07/81/7e/35/df/3d/45/91/DM00875974/files/DM00875974.pdf/jcr:content/translations/en.DM00875974.pdf
  -X-NUCLEO-GNSS1A1
  -X-LINUX-GNSS1 https://www.st.com/en/embedded-software/x-linux-gnss1.html
  - STM32 MPU Wiki : https://wiki.st.com/stm32mpu/wiki/Main_Page
  
  
