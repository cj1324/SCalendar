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
  timeinfo->tm_year = mon->year - 1900;
  timeinfo->tm_mon = mon->month - 1;
  timeinfo->tm_mday = 1;
  timeinfo->tm_hour = 0;
  timeinfo->tm_min = 0;
  timeinfo->tm_sec = 0;
  timeinfo->tm_wday = 0;

  mktime(timeinfo);

  mon->firstwday = timeinfo->tm_wday;

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
