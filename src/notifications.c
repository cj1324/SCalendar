/*
 * =====================================================================================
 *
 *       Filename:  notifications.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/14/2018 08:00:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <gdk/gdk.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include "notifications.h"

gboolean status_icon_button_release(GtkStatusIcon *status_icon, GdkEvent *event,
                                    gpointer user_data) {
  g_debug("status_icon_button_release");
  gtk_widget_destroy(GTK_WIDGET(user_data));
}

GNotification *init_notification(GtkWidget *window) {
  GNotification *notifi;
  GtkStatusIcon *status_icon;
  GFile *icon_file;

  g_debug("init_notification");

  notifi = g_notification_new("Hi Notifi");

  g_notification_set_title(notifi, "Title");
  g_notification_set_body(notifi, "Hello world");

  icon_file = g_file_new_for_path("../res/scal2.png");
  g_notification_set_icon(notifi, G_ICON(g_file_icon_new(icon_file)));
  status_icon =
      gtk_status_icon_new_from_gicon(G_ICON(g_file_icon_new(icon_file)));
  gtk_status_icon_set_screen(status_icon, gdk_screen_get_default());
  g_signal_connect(status_icon, "button-release-event",
                   G_CALLBACK(status_icon_button_release), (gpointer)window);
  // FIXME: free icon/icon_file
  return notifi;
}
