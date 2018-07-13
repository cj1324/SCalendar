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

struct date_t {
  int year;
  int month;
  int day;
  int zone;
  int week;
  int flag;
};

void get_current_date(struct date_t *date);
void get_current_month_first(struct date_t *date);
void get_current_month_last(struct date_t *date);
