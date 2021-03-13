#include "main_window.h"

/**
 * Code below is responsible for initial set up of
 * a window, which first appears on the user
 * screen
 **/

void initialize_window (GtkWidget *window)   
{
    GdkMonitor     *monitor;
    gint            w_width;
    gint            w_height;
    GdkRectangle    workarea;

    GtkWidget      *open_btn;

    monitor = gdk_display_get_primary_monitor(gdk_display_get_default());
    gdk_monitor_get_workarea(monitor, &workarea);
    w_width  = workarea.width / 2;
    w_height = workarea.height / 2;
    
    gtk_window_set_default_size(GTK_WINDOW (window), w_width, w_height);
    gtk_window_set_title       (GTK_WINDOW (window), "Filters application");

    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK (gtk_main_quit), NULL);

    open_btn = gtk_button_new_with_label("Open image");
    setup_button_on_opening_window(window, open_btn);
}

void setup_button_on_opening_window (GtkWidget *window,
                                     GtkWidget *open_btn)
{
    gtk_container_add(GTK_CONTAINER(window), open_btn);
    gtk_widget_set_halign(open_btn, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(open_btn, GTK_ALIGN_CENTER);

    g_signal_connect(G_OBJECT(open_btn), "clicked",
                      G_CALLBACK (on_open_btn_click),
                      window);
}

void on_open_btn_click (GtkWidget *widget, 
                        gpointer window)
{
    g_print("Hi there - %s was pressed\n", gtk_widget_get_name(widget));
    open_dialog(window);
}

void open_dialog (gpointer window)
{
    GtkWidget *dialog;
    gint       res;

    dialog = gtk_file_chooser_dialog_new("Choose an image",
                                         GTK_WINDOW(window),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_OK", GTK_RESPONSE_OK,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);
    
    res = gtk_dialog_run(GTK_DIALOG (dialog));
    
    if (res == GTK_RESPONSE_OK)
    {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        g_print("Filepath: %s\n", filename);
    }
    else
    {
        g_print("You canceled");
    }
    

    gtk_widget_destroy(dialog);
}
