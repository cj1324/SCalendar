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

struct month_t {
  int year;
  int month;
  int iscurrent;
  int firstwday;
  int maxmdays;
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
int get_current_date(struct date_t *date);
#endif
