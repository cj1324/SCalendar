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


#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <gio/gio.h>

gboolean
status_icon_button_release(GtkStatusIcon *status_icon,
							GdkEvent *event,
							gpointer user_data) {
	g_debug("status_icon_button_release");


}

GNotification *
init_notification(GtkWidget *window) {
	GNotification *notifi;
	GtkStatusIcon *status_icon;
	GFile *icon_file;

	g_debug("init_notification");

	notifi = g_notification_new("Hi Notifi");

	g_notification_set_title(notifi, "Title");
	g_notification_set_body(notifi, "Hello world");

	icon_file = g_file_new_for_path("../res/scal2.png");
	g_notification_set_icon(notifi, G_ICON(g_file_icon_new(icon_file)));
	status_icon = gtk_status_icon_new_from_gicon(G_ICON(g_file_icon_new(icon_file)));
	gtk_status_icon_set_screen(status_icon, gdk_screen_get_default());
	g_signal_connect(status_icon, "button-release-event",
		G_CALLBACK(status_icon_button_release), (gpointer)window);
	// FIXME: free icon/icon_file
	return notifi;
}

static void
activate(GtkApplication *app,
	gpointer user_data) {
	GtkWidget *window;

	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Hello GNOME");

	GNotification * notifi = init_notification(window);
	g_application_send_notification(G_APPLICATION(app), "new-window", notifi);

	gtk_widget_show_all(window);
}


int
main(int argc, char **argv) {
	GtkApplication *app;
	int status;
	app = gtk_application_new("org.gtk.scal",
		G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate",
		G_CALLBACK(activate), NULL);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);
	return (status);
}
