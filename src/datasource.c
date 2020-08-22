/*
 * =====================================================================================
 *
 *       Filename:  datasource.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  07/13/2018 10:20:23 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "datasource.h"

int get_current_date(struct date_t *date) {

  time_t rawtime;
  struct tm *timeinfo;

  rawtime = time(NULL);
  timeinfo = localtime((time_t *)&rawtime);

  date->year = timeinfo->tm_year + 1900;
  date->month = timeinfo->tm_mon + 1;
  date->day = timeinfo->tm_mday;
  date->week = timeinfo->tm_wday;
  date->zone = 8; // hard code CST

  return 0;
}

int month_t_set_firstwday(struct month_t *mon) {

  struct tm *timeinfo = malloc(sizeof(struct tm));
  bzero(timeinfo, sizeof(struct tm));

  timeinfo->tm_year = mon->year - 1900;
  timeinfo->tm_mon = mon->month - 1;
  timeinfo->tm_mday = 1;
  timeinfo->tm_hour = 1;
  timeinfo->tm_min = 0;
  timeinfo->tm_isdst = 0;

  mktime(timeinfo);

  mon->firstwday = timeinfo->tm_wday;
  free(timeinfo);
  return 0;
}

int month_t_set_maxmdays(struct month_t *mon) {

  int year = mon->year;
  int month = mon->month;

  int days;
  int is_leap_year;

  if (month == 4 || month == 6 || month == 9 || month == 11) {
    days = 30;
  } else if (month == 2) {
    is_leap_year = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (is_leap_year) {
      days = 29;
    } else {
      days = 28;
    }
  } else {
    days = 31;
  }
  mon->maxmdays = days;

  return 0;
}

int month_t_set_iscurrent(struct month_t *mon) {
  struct date_t *date = malloc(sizeof(struct date_t));
  get_current_date(date);
  mon->iscurrent = 0;
  if (date->year == mon->year && date->month == mon->month) {
    mon->iscurrent = 1;
  }
  return 0;
}

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
            tok && *tok;
            tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

int month_t_set_days(struct month_t *mon) {
  char path[128];
  size_t line_len = 1024;
  char * line;
  char * tmp;

  line = malloc(line_len);
  tmp = malloc(line_len);

  struct day_info_t * day_info;
  snprintf((char *)&path, 128, DATA_PATH_FORMAT, mon->year, mon->month);
  // TODO check file?>
  FILE* stream = fopen((char *)&path, "r");
  while (fgets(line, line_len, stream)){
    day_info = (struct day_info_t *)malloc(sizeof(struct day_info_t));
    strncpy(tmp, line, line_len);
    day_info -> index = atoi(getfield(tmp, 1));
    strncpy(tmp, line, line_len);
    strncpy(day_info -> nl_month, getfield(tmp, 2), 8);
    strncpy(tmp, line, line_len);
    strncpy(day_info -> nl_day, getfield(tmp, 3), 8);
    strncpy(tmp, line, line_len);
    strncpy(day_info -> nl_jieqi, getfield(tmp, 4), 8);
    strncpy(tmp, line, line_len);
    strncpy(day_info -> festival, getfield(tmp, 5), 8);
    strncpy(tmp, line, line_len);
    day_info -> holiday = atoi(getfield(tmp, 6));
    strncpy(tmp, line, line_len);
    strncpy(day_info -> recommend, getfield(tmp, 7), 8);
  }
  fclose(stream);
  free(tmp);
  free(line);
  return 0;
}
