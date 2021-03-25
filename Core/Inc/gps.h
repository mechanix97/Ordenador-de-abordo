#ifndef _GPS_H_
#define _GPS_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define GMT -3
#define NMEA_MESSAGE "$GPRMC"

typedef struct {
    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t hour;
    uint16_t minute;
    uint16_t second;    
} date_t;

typedef struct {
    date_t date;
    char status;
    uint32_t lat_int;
    uint32_t lat_dec;
    char lat;
    uint32_t lon_int;
    uint32_t lon_dec;
    char lon;
} gps_t;

gps_t parseGPSRead(char * str);
bool isValid(gps_t gps);

#endif
