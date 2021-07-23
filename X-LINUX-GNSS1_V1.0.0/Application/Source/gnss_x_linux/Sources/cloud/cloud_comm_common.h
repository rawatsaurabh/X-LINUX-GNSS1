#ifndef CLOUD_COMM_COMMON_HEADER
#define CLOUD_COMM_COMMON_HEADER

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>



#define TEST_MODE 1
#define PRINTF(...) if(TEST_MODE) printf(__VA_ARGS__)


#define CONF_FILE_NAME "creds.conf"

#endif /* CLOUD_COMM_COMMON_HEADER */