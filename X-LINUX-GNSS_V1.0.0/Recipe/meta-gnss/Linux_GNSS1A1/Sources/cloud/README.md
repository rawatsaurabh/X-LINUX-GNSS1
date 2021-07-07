# Cloud Communication Module

## This is a C based module to upload sensor data to cloud dashboard

### File Description

- **cloud_commm_config .c/.h**: Routines using libcurl to upload the data to the cloud application https endpoint
    This is an integrator file, it accepts a binary data received from P2P module, invokes *sensor_parser* to parse the sensor data, loads api end points and other configuration data using *cloud_comm_config*, generates the json payload to upload the data and uses libCurl functions to upload the data to cloud
- **cloud_comm_config .c/.h**: This file contains routines to read API endpoint, API keys and node names from *creds.conf* file
  > The user should update the *creds.conf* file after registering on the dashboard
- **sensor_parser .c/.h**: This file contain routines to parse binary sensor data to extract the information of each sensor

### Cloud End Points

> This Application tagets the Asset Tracking Dashboard hosted

    The dashboard is accesible at [this link](https://dsh-assetracking.st.com) 

### Developer Notes

- Enable debug logs in *cloud_comm_common.h*, set TEST_MODE 1

#### Updates since last commmit

Corrected printf newlines as per Linux conventions (`\r\n` to `\n`)
better handling of debug traces, use cloud_comm_common.h
