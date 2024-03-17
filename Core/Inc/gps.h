#ifndef __GPS_H
#define __GPS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include "cmsis_os2.h"

#define GPS_MSG_LEN 128

typedef enum {
	GGA,
	GLL,
	RMC
}GPS_MSG_TYPE;

typedef struct {
	GPS_MSG_TYPE type;
	int fix;
	double latitude;
	char latSide;
	double longitude;
	char lonSide;
	float altitude;
	float hdop;
	int satilits;
	char lastMeasure[11];
} GPS_POINT;


typedef struct {
	GPS_MSG_TYPE type;
	char msg[GPS_MSG_LEN];
} GPS_MSG;

extern osMessageQueueId_t gps_charsHandle;
extern osMessageQueueId_t gps_msgHandle;
extern osMessageQueueId_t gps_pointsHandle;

int gps_validate(const uint8_t* gps_msg);
int gps_parse_gga(const char *msg, GPS_POINT* data);
int gps_parse_gll(const char *msg, GPS_POINT* data);
int gps_parse_rmc(const char *msg, GPS_POINT* data);

#ifdef __cplusplus
}
#endif

#endif