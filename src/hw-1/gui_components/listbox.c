#include "listbox.h"

void setup_listbox_on_main_window(GtkWidget *listbox, GtkWidget *image,
                                  gsize size)
{
    gtk_widget_set_halign(listbox, GTK_ALIGN_FILL);
    gtk_widget_set_valign(listbox, GTK_ALIGN_FILL);

    gchar *dims = get_image_dimensions(image);
    gchar *f_size = get_image_file_size_from_gulong_to_char(size);

    add_item_to_listbox(listbox, "Image size:");
    add_item_to_listbox(listbox, dims);
    add_item_to_listbox(listbox, f_size);
}

void add_item_to_listbox(GtkWidget *listbox, char *text)
{
    GtkWidget *label = gtk_label_new(text);

    gtk_widget_set_margin_top(label, 2);
    gtk_widget_set_margin_bottom(label, 2);
    gtk_widget_set_margin_start(label, 5);
    gtk_widget_set_margin_end(label, 5);

    gtk_list_box_insert(GTK_LIST_BOX(listbox), label, -1);
}

gchar *get_image_dimensions(GtkWidget *image)
{
    GdkPixbuf *dimensions = gtk_image_get_pixbuf(GTK_IMAGE(image));

    gchar width[5];
    gchar height[5];

    g_snprintf(width, 5, "%d", gdk_pixbuf_get_width(dimensions));
    g_snprintf(height, 5, "%d", gdk_pixbuf_get_height(dimensions));

    gchar *str = g_strjoin("x", width, height, NULL);

    return str;
}

gchar *get_image_file_size_from_gulong_to_char(gulong size_in_bytes)
{
    gfloat size_in_mb = size_in_bytes / 1000000.00;
    gchar *str_size_in_mb = (gchar *)calloc(10, sizeof(gchar));
    sprintf(str_size_in_mb, "%0.3f MB", size_in_mb);

    return str_size_in_mb;
}
