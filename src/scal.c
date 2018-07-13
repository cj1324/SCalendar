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

#define GRID_MONTH_START 2
#define GRID_CONTRL_START 8

void debug_date_t(struct date_t *date) {
  g_debug("debug %.4d-%.2d-%.2d Week:%d  Zone:%d", date->year, date->month,
          date->day, date->week, date->zone);
}

void init_default_date(GtkGrid *grid) {
  struct date_t *current_date = malloc(sizeof(struct date_t));
  struct date_t *current_month = malloc(sizeof(struct date_t));
  struct date_t *current_month_last = malloc(sizeof(struct date_t));
  GtkWidget *label;
  gchar label_text[32];
  gint index = 1;

  get_current_date(current_date);
  get_current_month_first(current_month);
  get_current_month_last(current_month_last);

  debug_date_t(current_date);
  debug_date_t(current_month);
  for (int i = 0; i <= 5; i++) {
    for (int j = 0; j <= 6; j++) {
      label = gtk_grid_get_child_at(GTK_GRID(grid), j, i + GRID_MONTH_START);
      if (i == 0 && j < current_month->week) {
        gtk_label_set_text(GTK_LABEL(label), "");
        continue;
      }

      if (index > current_month_last->day) {
        gtk_label_set_text(GTK_LABEL(label), "");
        continue;
      }

      if (index == current_date->day) {
        const char *format = "<span weight=\"bold\" background=\"#FF0000\" "
                             "foreground=\"#FFFFFF\" size=\"large\" > \%d "
                             "</span>";
        char *markup;
        markup = g_markup_printf_escaped(format, index);
        gtk_label_set_markup(GTK_LABEL(label), markup);
        g_free(markup);
      } else {
        snprintf((char *)&label_text, 32, " %d ", index);
        gtk_label_set_text(GTK_LABEL(label), (char *)&label_text);
      }
      index++;
    }
  }

  free(current_date);
  free(current_month);
  free(current_month_last);
}

static void init_ui_grid(GtkGrid *grid) {
  gtk_grid_attach(GTK_GRID(grid), gtk_button_new_with_label("<"), 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new(" 2018 年 "), 1, 0, 2, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new(""), 3, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new(" 七月 "), 4, 0, 2, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_button_new_with_label(">"), 6, 0, 1, 1);

  gchar weeknames[7][32] = {
      " 周日 ", " 周一 ", " 周二 ", " 周三 ", " 周四 ", " 周五 ", " 周六 ",
  };

  for (int i = 0; i <= 6; i++) {
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new((gchar *)&weeknames[i]), i, 1,
                    1, 1);
  }

  for (int i = 0; i <= 6; i++) {
    for (int j = 0; j <= 5; j++) {
      gtk_grid_attach(GTK_GRID(grid), gtk_label_new(" # "), i,
                      j + GRID_MONTH_START, 1, 1);
    }
  }

  gtk_grid_attach(GTK_GRID(grid), gtk_button_new_with_label(" X "), 0,
                  GRID_CONTRL_START, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), gtk_label_new(" 第 28 周 "), 1,
                  GRID_CONTRL_START, 5, 1);
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

void hide_button_clicked(GtkButton *button, gpointer user_data) {
  gtk_widget_hide(GTK_WIDGET(user_data));
}

void close_button_clicked(GtkButton *button, gpointer user_data) {
  gtk_widget_destroy(GTK_WIDGET(user_data));
}

static void init_ui_grid_event(GtkGrid *grid, GtkWindow *window) {
  GtkWidget *hide_button, *close_button;
  hide_button = gtk_grid_get_child_at(GTK_GRID(grid), 6, GRID_CONTRL_START);
  close_button = gtk_grid_get_child_at(GTK_GRID(grid), 0, GRID_CONTRL_START);
  g_signal_connect(GTK_BUTTON(hide_button), "clicked",
                   G_CALLBACK(hide_button_clicked), window);
  g_signal_connect(GTK_BUTTON(close_button), "clicked",
                   G_CALLBACK(close_button_clicked), window);
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkGrid *grid;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Calendar");

  GNotification *notifi = init_notification(window);
  g_application_send_notification(G_APPLICATION(app), "new-window", notifi);

  g_debug("init grid table.");
  grid = (GtkGrid *)gtk_grid_new();

  init_ui_grid(GTK_GRID(grid));
  init_ui_grid_event(GTK_GRID(grid), GTK_WINDOW(window));
  init_default_date(GTK_GRID(grid));
  gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(grid));
  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_window_move(GTK_WINDOW(window), 800, 200);
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;
  app = gtk_application_new("org.gtk.scal", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return (status);
}
