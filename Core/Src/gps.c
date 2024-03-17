#include "gps.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int gps_validate(const uint8_t* gps_msg){
  char check[3];
  char checkcalcstr[3];
  int i;
  int calculated_check;

  i=0;
  calculated_check=0;

  if((char)gps_msg[i++] != '$') return 0;

  while((gps_msg[i] != 0) && (gps_msg[i] != '*') && (i < GPS_MSG_LEN)){
    calculated_check ^= gps_msg[i];// calculate the checksum
    i++;
  }

  if(i >= GPS_MSG_LEN){
    return 0;
  }

  if (gps_msg[i] == '*'){
    check[0] = gps_msg[i+1];
    check[1] = gps_msg[i+2];
    check[2] = 0;
  }
  else
    return 0;

  sprintf(checkcalcstr,"%02X",calculated_check);
  return((checkcalcstr[0] == check[0])
    && (checkcalcstr[1] == check[1])) ? 1 : 0 ;
}

static int get_lat_lon(char **start, GPS_POINT* data)
{
  char *next;
  int diff = 0, tmp_strtol = 0;
  char tmp_dd[4] = {0}, tmp_mm[7] = {0};
  float tmp_strtof = 0.0;

  next = strchr(*start, ',');
  diff = next - *start;
  if( diff == 0 || diff > 10 ) return 0;
  tmp_dd[0] = *(*start)++;
  tmp_dd[1] = *(*start)++;
  tmp_dd[2] = '\0';
  for (int z = 0; z < 6; ++z) tmp_mm[z] = (*start)[z];

  tmp_strtol = strtol(tmp_dd, NULL, 10);
  tmp_strtof = strtof(tmp_mm, NULL);
  data->latitude = tmp_strtol + tmp_strtof / 60;

  *start = ++next;
  next = strchr(*start, ',');
  diff = next - *start;
  if( diff != 1) return 0;
  data->latSide = **start;

  *start = ++next;
  next = strchr(*start, ',');
  diff = next - *start;
  if( diff == 0 || diff > 12 ) return 0;

  tmp_dd[0] = *(*start)++;
  tmp_dd[1] = *(*start)++;
  tmp_dd[2] = *(*start)++;
  tmp_dd[3] = '\0';
  for (int z = 0; z < 6; ++z) tmp_mm[z] = (*start)[z];

  tmp_strtol = strtol(tmp_dd, NULL, 10);
  tmp_strtof = strtof(tmp_mm, NULL);

  data->longitude = tmp_strtol + tmp_strtof / 60;

  *start = ++next;
  next = strchr(*start, ',');
  diff = next - *start;
  if( diff != 1 ) return 0;
  data->lonSide = **start;
  *start = ++next;
  return 1;
}

static int get_time(char **start, GPS_POINT* data)
{
  char *next;
  int diff = 0;
  next = strchr(*start, ',');
  diff = next - *start;
  if( diff == 0 || diff > 11 ) return 0;
  strncpy(data->lastMeasure, *start, diff);
  *start = ++next;
  return 1;
}

int gps_parse_gga(const char *msg, GPS_POINT* data)
{
  char *curr, *next;
  int diff = 0;

  curr = strchr(msg, ',');
  curr++;
  if(!get_time(&curr, data)) return 0;

  if(!get_lat_lon(&curr, data)) return 0;
  next = strchr(curr, ',');
  diff = next - curr;
  if( diff != 1 ) return 0;
  data->fix = strtol(curr, NULL, 10);

  curr = ++next;
  next = strchr(curr, ',');
  diff = next - curr;
  if( diff == 0 || diff > 2 ) return 0;
  data->satilits = strtol(curr, NULL, 10);

  curr = ++next;
  next = strchr(curr, ',');
  diff = next - curr;
  if( diff == 0 || diff > 4 ) return 0;
  data->hdop = strtof(curr, NULL);

  curr = ++next;
  next = strchr(curr, ',');
  diff = next - curr;
  if( diff == 0 || diff > 6 ) return 0;
  data->altitude = strtof(curr, NULL);

  return 1;
}

int gps_parse_gll(const char *msg, GPS_POINT* data)
{
  char *curr, *next;

  curr = strchr(msg, ',');
  curr++;

  if(!get_lat_lon(&curr, data)) return 0;

  if(!get_time(&curr, data)) return 0;

  next = strchr(curr, ',');

  curr = ++next;
  ++next;
  if(*next != ',' && *next != '*') return 0;

  data->fix = *curr == 'A' ? 1 : 0;
  return 1;
}

int gps_parse_rmc(const char *msg, GPS_POINT* data)
{
  char *curr, *next;
  int diff = 0;

  curr = strchr(msg, ',');
  curr++;
  if(!get_time(&curr, data)) return 0;


  next = strchr(curr, ',');
  diff = next - curr;
  if( diff != 1 ) return 0;

  data->fix = *curr == 'A' ? 1 : 0;
  next++;
  if(!get_lat_lon(&next, data)) return 0;

  return 1;
}