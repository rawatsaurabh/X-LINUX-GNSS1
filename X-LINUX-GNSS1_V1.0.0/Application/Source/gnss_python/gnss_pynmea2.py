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
**********************************************************************************/
/*! \file gnss_pynmea2.py
 *
 *  \author Saurab RAWAT
 *
 *  \brief Sample Application to read the Teseo-LIV3F GNSS data from X-NUCLEO-GNSS1
 *   over UART using pyserial and pynmea2 python module.
 *
 */
/*
 **********************************************************************************
 * INCLUDES
 **********************************************************************************
 */
import serial
import time
import string
import pynmea2
import re


while True:
    port="/dev/ttySTM2"
    ser=serial.Serial(port, baudrate=9600, timeout=0.5)
    dataout = pynmea2.NMEAStreamReader()
    newdata=ser.readline()
    print(newdata)
    gpsdataString=newdata.decode()
    string_to_search="GPGLL"
    if string_to_search in gpsdataString:
        newmsg=pynmea2.parse(gpsdataString)
        print(newmsg)
        lat=newmsg.latitude
        lng=newmsg.longitude
        print("Latitude= " + str(lat) + " and Longitude= " + str(lng))
