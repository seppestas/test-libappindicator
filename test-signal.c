#include <gtk/gtk.h>
#include <libappindicator/app-indicator.h>
#include <libdbusmenu-glib/server.h>
#include <libdbusmenu-gtk/parser.h>

void on_about_to_show(DbusmenuMenuitem *menuitem, gpointer user_data)
{
    printf("About to show menu\n");
}

// Detect clicks by subscribing to "about-to-show" signals
// https://bugs.launchpad.net/ubuntu/+source/libappindicator/+bug/522152/comments/15
void workaround_subsribe_about_to_show(AppIndicator *indicator)
{
    DbusmenuServer *menuServer;
    DbusmenuMenuitem *rootMenuItem;
    g_object_get(indicator, "dbus-menu-server", &menuServer, NULL);
    g_object_get(menuServer, "root-node", &rootMenuItem, NULL);

    g_signal_connect(rootMenuItem, "about-to-show", G_CALLBACK(on_about_to_show), NULL);
}

void on_unity_panel_signal(GDBusProxy *proxy, gchar *sender_name, gchar *signal_name, GVariant *parameters, gpointer user_data)
{
    // gchar *parameters_str = g_variant_print (parameters, TRUE);
    // printf("Signal: %s, params: %s\n", signal_name, parameters_str);
    // g_free(parameters_str);

    if (g_strcmp0(signal_name, "ReSync"))
    {
        const gchar *to_sync = g_variant_get_string(parameters, NULL); // Why the fuck is this not a string
        printf("Time to sync %s\n", to_sync);
        if (g_strcmp0(to_sync, "libapplication.so"))
        {
            printf("Time to sync %s\n", signal_name);
            GError *err = NULL;
            GVariant *appindicators = g_dbus_proxy_call_sync(
                proxy,
                "SyncOne",
                g_variant_new("s", "libapplication.so"),
                G_DBUS_CALL_FLAGS_NONE,
                -1,
                NULL,
                &err
            );

            if (err != NULL)
            {
                printf("Error syncing libapplication: %s\n", err->message);
                g_error_free(err);
                return;
            }
            gchar *appindicators_str = g_variant_print(appindicators, TRUE);
            printf("appindicators: %s\n", appindicators_str);
            g_free(appindicators_str);
        }
        
        // Check if it is for "libapplication.so"
        // Send SyncOne method call
        // change id of indicator
    }
    else if (g_strcmp0(signal_name, "EntryActivated"))
    {
        // Do we have the id of the indicator
        // is it equal to the one in of parameters?
        // report click and location
    }

}

void unity_panel_proxy_ready(GObject *source_object, GAsyncResult *res, gpointer user_data)
{
    GError *err = NULL;
    GDBusProxy *unity_panel_proxy = g_dbus_proxy_new_for_bus_finish(res, &err);

    if (err != NULL)
    {
        printf("Error creating DBus proxy: %s\n", err->message);
        g_error_free(err);
        return;
    }

    g_signal_connect(unity_panel_proxy, "g-signal", G_CALLBACK(on_unity_panel_signal), NULL);
}

// INCOMPLETE
// Detect clicks by subscribing to "EntryActivated" signals from the Unity
// panel service. This also contains the location of the icon.
// https://bugs.launchpad.net/ubuntu/+source/libappindicator/+bug/522152/comments/11
void workaround_entry_activated()
{
    g_dbus_proxy_new_for_bus(
        G_BUS_TYPE_SESSION,
        G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES | G_DBUS_PROXY_FLAGS_DO_NOT_AUTO_START,
        NULL,
        "com.canonical.Unity.Panel.Service.Desktop",
        "/com/canonical/Unity/Panel/Service",
        "com.canonical.Unity.Panel.Service",
        NULL,
        unity_panel_proxy_ready,
        NULL
    );
}


void on_click(GtkStatusIcon *status_icon, gpointer user_data)
{
    printf("Click!\n");
}

int main( int   argc,
          char *argv[] )
{
    gtk_init (&argc, &argv);

    gchar *cwd = g_get_current_dir();
    AppIndicator *indicator = app_indicator_new_with_path(
        "testing-123",
        "icon",
        APP_INDICATOR_CATEGORY_APPLICATION_STATUS,
        cwd
    );
    g_free(cwd);

    printf("Icon pointer: %p\n", indicator);

    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ATTENTION);

    GtkWidget *menu_widget = gtk_menu_new();
    GtkMenu* menu = GTK_MENU(menu_widget);
    GtkMenuShell* menu_shell = GTK_MENU_SHELL(menu);

    GtkWidget *menu_item = gtk_menu_item_new_with_mnemonic("hello, world");

    gtk_widget_show(menu_item);
    gtk_menu_shell_append(menu_shell, menu_item);
    
    app_indicator_set_menu(indicator, menu);

    // GTkStatusIcon test
    GtkStatusIcon* gtk_status_icon_ = gtk_status_icon_new();
    g_signal_connect(gtk_status_icon_, "activate", G_CALLBACK(on_click), NULL);

    workaround_subsribe_about_to_show(indicator);

    workaround_entry_activated();

    gtk_main();
    return 0;
}
