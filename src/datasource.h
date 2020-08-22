/*
 * =====================================================================================
 *
 *       Filename:  datasource.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  07/13/2018 10:20:33 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef DATASOURCE_H
#define DATASOURCE_H


#define  DATA_PATH_FORMAT "~/cal_data/%d-%d.csv"

struct day_info_t{
  int index;
  char recommend[8];
  char nl_month[8];
  char nl_day[8];
  char nl_jieqi[8];
  char nl_jiaqi[8];
  char festival[32];
  int holiday;
};

struct month_t {
  int year;
  int month;
  int iscurrent;
  int firstwday;
  int maxmdays;
  struct day_info_t days[32];
};

struct date_t {
  int year;
  int month;
  int day;
  int zone;
  int week;
  int flag;
};

int month_t_set_maxmdays(struct month_t *mon);
int month_t_set_firstwday(struct month_t *mon);
int month_t_set_iscurrent(struct month_t *mon);
int month_t_set_days(struct month_t *mon);
int get_current_date(struct date_t *date);
#endif
