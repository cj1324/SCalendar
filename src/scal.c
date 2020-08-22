/*
 * =====================================================================================
 *
 *	   Filename:  scal.c
 *
 *	Description:
 *
 *		Version:  1.0
 *		Created:  05/29/2018 02:40:53 PM
 *	   Revision:  none
 *	   Compiler:  gcc
 *
 *		 Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>

#include <gdk/gdk.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include "datasource.h"
#include "notifications.h"
#include "scal.h"

// {{{ define data

#define GRID_MONTH_START 2
#define GRID_CONTRL_START 8

const char weeknames[7][16] = {
    "周日", "周一", "周二", "周三", "周四", "周五", "周六",
};

const char monthnames[12][16] = {
    "一月", "二月", "三月", "四月", "五月",   "六月",
    "七月", "八月", "九月", "十月", "十一月", "十二月",
};
static struct month_t *mon;

// }}}

// {{{ debug function
void debug_date_t(struct date_t *date) {
  g_debug("debug %.4d-%.2d-%.2d Week:%d  Zone:%d", date->year, date->month,
          date->day, date->week, date->zone);
}
void debug_month_t(struct month_t *mon) {
  g_debug("debug %.4d-%.2d maxmdays:%d firstwday:%d iscurrent:%d", mon->year,
          mon->month, mon->maxmdays, mon->firstwday, mon->iscurrent);
}
// }}}

// {{{ event

static void hide_button_clicked(GtkButton *button, gpointer user_data) {
  gtk_widget_hide(GTK_WIDGET(user_data));
}

static void close_button_clicked(GtkButton *button, gpointer user_data) {
  gtk_widget_destroy(GTK_WIDGET(user_data));
}

static void plus_button_clicked(GtkButton *button, gpointer user_data) {

  g_debug("plus_button_clicked");
  struct date_t *current_date = g_malloc(sizeof(struct date_t));
  get_current_date(current_date);

  if (mon->month < 12) {
    mon->month += 1;
  } else {
    mon->year += 1;
    mon->month = 1;
  }
  month_t_set_maxmdays(mon);
  month_t_set_firstwday(mon);
  month_t_set_iscurrent(mon);

  debug_date_t(current_date);
  debug_month_t(mon);

  show_calendar_date(GTK_GRID(user_data), mon, current_date);

  g_free(current_date);
}

static void minus_button_clicked(GtkButton *button, gpointer user_data) {
  g_debug("minus_button_clicked");
  struct date_t *current_date = g_malloc(sizeof(struct date_t));
  get_current_date(current_date);

  // TODO: min limit?
  if (mon->month > 1) {
    mon->month -= 1;
  } else {
    mon->year -= 1;
    mon->month = 12;
  }
  month_t_set_maxmdays(mon);
  month_t_set_firstwday(mon);
  month_t_set_iscurrent(mon);

  debug_date_t(current_date);
  debug_month_t(mon);

  show_calendar_date(GTK_GRID(user_data), mon, current_date);

  g_free(current_date);
}

static gboolean timer_timeout(gpointer user_data) {
  g_debug("timer_timeout");

  struct date_t *current_date = g_malloc(sizeof(struct date_t));
  get_current_date(current_date);

  month_t_set_maxmdays(mon);
  month_t_set_firstwday(mon);
  month_t_set_iscurrent(mon);

  debug_month_t(mon);

  show_calendar_date(GTK_GRID(user_data), mon, current_date);
  g_free(current_date);
  return TRUE;
}

// }}}

// {{{ ui render day
void show_null_day(GtkWidget *image,gint j, gint index){
  cairo_surface_t *s;
  cairo_t *cr;

  GdkPixbuf *pixbuf;

  s = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 32, 32);
  cr = cairo_create (s);
  pixbuf = gdk_pixbuf_get_from_surface(s, 0, 0, 32, 32);
  cairo_stroke(cr);
  cairo_destroy (cr);
  gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
}

void show_day_index(cairo_t *cr,gint index){
  char c[8];
  if (index > 9){
    cairo_move_to(cr, 6, 22);
  } else {
    cairo_move_to(cr, 12, 22);
  }

  cairo_set_font_size(cr, 18);
  snprintf((char *)&c, 8, "%d", index);
  cairo_text_path(cr, (char *)&c);
  cairo_set_line_width(cr, 1);
}

void show_current_day(GtkWidget *image,gint j, gint index){
  cairo_surface_t *s;
  cairo_t *cr;

  GdkPixbuf *pixbuf;

  s = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 32, 32);
  cr = cairo_create (s);
  cairo_rectangle(cr, 0, 0, 31, 31);
  cairo_set_source_rgb(cr, 0.9, 0.8, 0.7);
  cairo_fill(cr);

  cairo_set_source_rgb(cr, 1, 0 ,0);
  show_day_index(cr, index);
  cairo_stroke(cr);
  cairo_destroy (cr);
  pixbuf = gdk_pixbuf_get_from_surface(s, 0, 0, 32, 32);
  gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
}

void show_month_day(GtkWidget *image,gint j, gint index){
  cairo_surface_t *s;
  cairo_t *cr;

  GdkPixbuf *pixbuf;
  s = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 32, 32);
  cr = cairo_create (s);

  if (j == 0 || j == 6){
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_rectangle(cr,0, 0, 31, 31);
    cairo_fill(cr);
  }

  cairo_set_source_rgb(cr, 0, 0 ,0);
  show_day_index(cr, index);
  cairo_stroke(cr);
  cairo_destroy (cr);
  pixbuf = gdk_pixbuf_get_from_surface(s, 0, 0, 32, 32);
  gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixbuf);
}
// }}}

// {{{ ui update
void show_calendar_date(GtkGrid *grid, struct month_t *mon,
                        struct date_t *current_date) {
  GtkWidget *label, *image;
  gchar label_text[32];
  gint index = 1;
  gint current_day = 0;

  // set year
  label = gtk_grid_get_child_at(GTK_GRID(grid), 1, 0);
  snprintf((char *)&label_text, 32, "%.4d 年", mon->year);
  gtk_label_set_text(GTK_LABEL(label), (gchar *)&label_text);

  // set month
  label = gtk_grid_get_child_at(GTK_GRID(grid), 4, 0);
  gtk_label_set_text(GTK_LABEL(label), monthnames[mon->month - 1]);


  // set every days
  for (int i = 0; i <= 5; i++) {
    for (int j = 0; j <= 6; j++) {
      image = gtk_grid_get_child_at(GTK_GRID(grid), j, i + GRID_MONTH_START);
      if (i == 0 && j < mon->firstwday) {
        show_null_day(image, j, index);
        continue;
      }

      if (index > mon->maxmdays) {
        show_null_day(image, j, index);
        continue;
      }

      if (index == current_date->day && mon->iscurrent) {
        show_current_day(image, j, index);
        current_day = 1;
      } else {
        show_month_day(image, j, index);
      }
      index++;
    }
  }
}
// }}}

// {{{ ui init
static void init_ui_grid(GtkGrid *grid) {
  gtk_grid_attach(GTK_GRID(grid), gtk_button_new_with_label("<"), 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new(" N 年 "), 1, 0, 2, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new(""), 3, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new(" N 月 "), 4, 0, 2, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_button_new_with_label(">"), 6, 0, 1, 1);

  for (int i = 0; i <= 6; i++) {
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new((char *)&weeknames[i]), i, 1,
                    1, 1);
  }


  for (int i = 0; i <= 6; i++) {
    for (int j = 0; j <= 5; j++) {
      gtk_grid_attach(
          GTK_GRID(grid),
          gtk_image_new_from_icon_name("image-x-generic", GTK_ICON_SIZE_DND), i,
          j + GRID_MONTH_START, 1, 1);
    }
  }

  gtk_grid_attach(GTK_GRID(grid), gtk_button_new_with_label(" X "), 0,
                  GRID_CONTRL_START, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new(""), 1, GRID_CONTRL_START, 5,
                  1);
  gtk_grid_attach(GTK_GRID(grid), gtk_button_new_with_label(" H "), 6,
                  GRID_CONTRL_START, 1, 1);

  gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 3);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

  gtk_widget_set_hexpand(GTK_WIDGET(grid), TRUE);
  gtk_widget_set_halign(GTK_WIDGET(grid), GTK_ALIGN_CENTER);
  gtk_widget_set_vexpand(GTK_WIDGET(grid), TRUE);
  gtk_widget_set_valign(GTK_WIDGET(grid), GTK_ALIGN_CENTER);

  return;
}

static void init_ui_grid_event(GtkGrid *grid, GtkWindow *window) {
  GtkWidget *hide_button, *close_button, *plus_button, *minus_button;
  hide_button = gtk_grid_get_child_at(GTK_GRID(grid), 6, GRID_CONTRL_START);
  close_button = gtk_grid_get_child_at(GTK_GRID(grid), 0, GRID_CONTRL_START);
  minus_button = gtk_grid_get_child_at(GTK_GRID(grid), 0, 0);
  plus_button = gtk_grid_get_child_at(GTK_GRID(grid), 6, 0);
  g_signal_connect(GTK_BUTTON(hide_button), "clicked",
                   G_CALLBACK(hide_button_clicked), window);
  g_signal_connect(GTK_BUTTON(close_button), "clicked",
                   G_CALLBACK(close_button_clicked), window);

  g_signal_connect(GTK_BUTTON(minus_button), "clicked",
                   G_CALLBACK(minus_button_clicked), grid);

  g_signal_connect(GTK_BUTTON(plus_button), "clicked",
                   G_CALLBACK(plus_button_clicked), grid);
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkGrid *grid;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Calendar");
  gtk_window_stick(GTK_WINDOW(window));

  GNotification *notifi = init_notification(window);

  // g_application_send_notification(G_APPLICATION(app), "new-window", notifi);

  g_debug("init grid ui.");
  grid = (GtkGrid *)gtk_grid_new();
  init_ui_grid(GTK_GRID(grid));

  g_debug("init grid event.");
  init_ui_grid_event(GTK_GRID(grid), GTK_WINDOW(window));

  g_debug("calc current date info.");
  struct date_t *current_date = g_malloc(sizeof(struct date_t));
  get_current_date(current_date);

  mon->year = current_date->year;
  mon->month = current_date->month;
  month_t_set_maxmdays(mon);
  month_t_set_firstwday(mon);
  month_t_set_iscurrent(mon);

  debug_date_t(current_date);
  debug_month_t(mon);

  g_debug("default date show.");
  show_calendar_date(GTK_GRID(grid), mon, current_date);

  g_free(current_date);

  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(grid));

  g_debug("window show.");
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_window_move(GTK_WINDOW(window), 800, 200);

  g_debug("init timer");
  g_timeout_add_seconds(300, (GSourceFunc)timer_timeout, grid);
}

// }}}

// {{{ main
int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  mon = g_malloc(sizeof(struct month_t));

  app = gtk_application_new("org.gtk.scal", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  g_free(mon);
  return (status);
}
// }}}
