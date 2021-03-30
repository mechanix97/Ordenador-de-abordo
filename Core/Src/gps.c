#include "gps.h"

gps_t parseGPSRead(char * str) {
    gps_t gps = {{0,0,0,0,0,0}, 'U', 0,0,'U',0,0,'U'};
    char * ptr;
    if((ptr = strstr(str, NMEA_MESSAGE)) != NULL) {
        uint32_t hour_raw = atoi(ptr + 7);
        if( hour_raw ){
            gps.date.hour = ((hour_raw / 10000) + 24 + GMT) %24;
            gps.date.minute = hour_raw /100 %100;
            gps.date.second = hour_raw %100;
        } 

        gps.status = ptr[17];
        uint32_t date_raw = 0;
        
        if ( gps.status != 'A'){
            date_raw = atoi(ptr + 25);
        } else {
            date_raw = atoi(ptr + 53);
        }        

        if( date_raw ){
            gps.date.year = date_raw % 100 + 2000;
            gps.date.month = date_raw  /100 %100;
            gps.date.day = date_raw /10000;
            if( GMT < 0 && gps.date.hour - GMT  > 24 ){
                gps.date.day--;
            } else if (GMT > 0 && gps.date.hour < GMT) {
                gps.date.day++;
            } 
        } else {
            gps.status = 'N';
        }

        if( !gps.date.day || !gps.date.month || !gps.date.year ){
            gps.status = 'N';
        }

        uint32_t lat1 = atoi(ptr+19);
        uint32_t lat2 = atoi(ptr+24);
        
        if( lat1 && lat2 ) {
            gps.lat_int = lat1 /100;
            gps.lat_dec = (lat1%100*100000+lat2)/60;
            gps.lat = ptr[30];
        }

        uint32_t lon1 = atoi(ptr+32);
        uint32_t lon2 = atoi(ptr+38);
        
        if( lon1 && lon2 ) {
            gps.lon_int = lon1 /100;
            gps.lon_dec = (lon1%100*100000+lon2)/60;
            gps.lon = ptr[44];
        }        
    }
    return gps;
}

bool isValid(gps_t gps) {
    return gps.status == 'A' || gps.status == 'V';
}
