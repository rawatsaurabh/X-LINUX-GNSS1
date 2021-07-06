# X-LINUX-GNSS1
This package is for retrieving the GNSS NMEA data from X-Nucleo-GNSS1A1 plugged to the Arduino Connector of STM32MP157C-DK2 Discovery Board.All the code 
runs on STM32MPU DK2 Board

There are 4 code repos : 

**1.gnss_i2c**

This is the C code to read the i2c data from /dev/i2cdev1

**2.gnss_python**

This is the python code to read the GNSS Data over uart using Python Script.It uses pynmea2 Library.
You need to install the pyserial and pynmea2 before using this.

**3.gnss_uart**

This is the C code to read the uart data from /dev/ttySTM2

**3.gnss_x_linux**

This is the cross compiled code with STM32 Layers removed.This is derived from X-Cube-GNSS1A1 


**Build  :**

Make sure to source the build environment for STM32MPU ( the step you do while Building kernel)
Go into each of the directory and run Make.Respective binary will be generated.
For python code you can directly scp it to STM32MPU Board


**Deploy :**

Use scp command to paste built files  to STM32MPU

**Run:**

run the app using command : ./<app name>
run the python code using command : python3 <python-filename>.py 


