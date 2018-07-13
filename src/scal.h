/*
 * =====================================================================================
 *
 *       Filename:  scal.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  07/13/2018 06:21:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef SCAL_H
#define SCAL_H
void show_calendar_date(GtkGrid *grid, struct month_t *mon,
                        struct date_t *current_date);

static void activate(GtkApplication *app, gpointer user_data);
#endif
