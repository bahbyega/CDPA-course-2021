#include "main_window.h"


int main (int argc, char **argv)
{
    GtkWidget  *window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    initialize_window(window);

    gtk_widget_show_all(window);
    
    gtk_main();

    return 0;
}

