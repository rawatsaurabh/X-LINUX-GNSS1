#ifndef CLOUD_COMM_HTTPS_HEADER
#define CLOUD_COMM_HTTPS_HEADER

#include "cloud_comm_common.h"
#include "NMEA_parser.h"


#ifdef __cplusplus
extern "C" {
#endif
typedef struct gps_pos
{

 int device_id;
 unsigned long timestamp;
 char d[5] ;
 float longitude;
 float latitude;
 float elevation;
} liv3f_pos;

int JsonStringyfy(char *str,liv3f_pos *position);

#ifdef __cplusplus
}
#endif


#endif /* CLOUD_COMM_HTTPS_HEADER */
