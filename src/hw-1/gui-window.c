#include "gui-window.h"                               

/**
 * Code below is responsible for initial set up of
 * a window and its components, which first appear 
 * on the user screen
 **/

void initialize_window (GtkWidget *window)   
{
    GtkWidget *init_btn;

    gtk_window_set_type_hint   (GTK_WINDOW (window), GDK_WINDOW_TYPE_HINT_DIALOG);
    gtk_window_set_title       (GTK_WINDOW (window), "Filters application");
    
    gtk_window_move(GTK_WINDOW(window), 700, 400);

    g_signal_connect(G_OBJECT(window), "destroy",
                     G_CALLBACK (gtk_main_quit), NULL);
    
    init_btn = gtk_button_new_with_label("Open image");
    setup_init_button(init_btn);

    gtk_container_add(GTK_CONTAINER(window), init_btn);
}

void setup_init_button (GtkWidget *button)
{
    gtk_widget_set_halign(button, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(button, GTK_ALIGN_CENTER);

    g_signal_connect(G_OBJECT(button), "clicked",
                     G_CALLBACK (on_init_btn_click),
                     NULL);
}

void on_init_btn_click (GtkWidget *widget)
{
    GtkWidget *dialog;
    GtkWidget *window = gtk_widget_get_toplevel(widget);
    gint       res;

    GdkPixbuf *pixbuf; // buffer for image data

    dialog = gtk_file_chooser_dialog_new("Choose an image",
                                         GTK_WINDOW (window),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_OK", GTK_RESPONSE_OK,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         NULL);
    
    res = gtk_dialog_run(GTK_DIALOG (dialog));
    
    if (res == GTK_RESPONSE_OK)
    {
        gchar *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        if (GTK_IS_BUTTON(widget))
        {
            gtk_widget_destroy(widget);
        }
        else if (GTK_IS_MENU_BAR(widget))
        {
            GtkWidget *parent = gtk_widget_get_parent(GTK_WIDGET(widget));
            gtk_widget_destroy(parent);
        }

        pixbuf = gdk_pixbuf_new_from_file_at_scale(filename, -1, -1, TRUE, NULL);
        
        gulong i_size = get_file_size(filename);
        setup_window_layout_elements(window, pixbuf, i_size);
        
        gtk_widget_show_all(window);
    }

    gtk_widget_destroy(dialog);
}

/**
 * This is responsible for setting up the main window elements after we 
 * chose an image to display
 **/
void setup_window_layout_elements (GtkWidget *window, GdkPixbuf *pixbuf, gulong i_size)
{
    GtkWidget *image     = gtk_image_new_from_pixbuf(pixbuf);
    GtkWidget *image_box = gtk_layout_new(0, 0); // need a wrapper for image resizing
    GtkWidget *info_lbox = gtk_list_box_new();
    GtkWidget *menu_bar  = gtk_menu_bar_new();
    GtkWidget *grid      = gtk_grid_new();

    
    setup_listbox_on_main_window(info_lbox, image, i_size);
    setup_menu_bar_on_main_window(menu_bar);

    gtk_layout_put(GTK_LAYOUT(image_box), image, 0, 0);
   
    gtk_grid_attach(GTK_GRID(grid), menu_bar, 0, 0, 2, 1);

    gtk_grid_attach_next_to(GTK_GRID(grid), info_lbox,
                                           menu_bar,
                                           GTK_POS_BOTTOM,
                                           1, 1);
                                                               
    gtk_grid_attach_next_to(GTK_GRID(grid), image_box,
                                           info_lbox,
                                           GTK_POS_RIGHT,
                                           1, 1);
    gtk_container_add(GTK_CONTAINER(window), grid);
    

    setup_image_on_main_window(image_box, image, pixbuf);
    //gtk_window_resize(GTK_WINDOW(window), 100, 50);
    gtk_window_move(GTK_WINDOW(window), 700, 400);
}

gulong get_file_size(gchar *filename)
{
    FILE   *file;
    
    file = fopen(filename, "r");
    fseek(file, 0L, SEEK_END);
    gulong size = ftell(file);
    fclose(file);
    
    return size;
}
