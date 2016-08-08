// Old school GTK status icon
// Unity seems to ignore these :|

#include <gtk/gtk.h>

void on_click(GtkStatusIcon *status_icon, gpointer user_data)
{
    printf("Click!\n");
}

int main( int   argc,
          char *argv[] )
{
    gtk_init (&argc, &argv);

    GtkStatusIcon* gtk_status_icon_ = gtk_status_icon_new();
    g_signal_connect(gtk_status_icon_, "activate", G_CALLBACK(on_click), NULL);

    gtk_main();

    return 0;
}