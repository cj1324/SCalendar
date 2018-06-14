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

#include <gdk/gdk.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include "notifications.h"

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Hello GNOME");

  GNotification *notifi = init_notification(window);
  g_application_send_notification(G_APPLICATION(app), "new-window", notifi);

  gtk_widget_show_all(window);
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
