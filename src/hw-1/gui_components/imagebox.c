#include "imagebox.h"

void setup_image_on_main_window(GtkWidget *image_box, GtkWidget *image,
                                GdkPixbuf *image_pixbuf)
{
    gtk_widget_set_hexpand(image_box, TRUE);
    gtk_widget_set_vexpand(image_box, TRUE);

    gtk_widget_set_halign(image, GTK_ALIGN_START);
    gtk_widget_set_valign(image, GTK_ALIGN_START);

    gtk_widget_set_size_request(image, 100, 100);

    // use this struct to pass data to the event handler below
    ResizeImage *image_to_resize;
    image_to_resize = (ResizeImage *)calloc(1, sizeof(*image_to_resize));
    image_to_resize->image = image;
    image_to_resize->pixbuf = image_pixbuf;

    resize_image_keeping_aspect_ratio(image_box, NULL, image_to_resize);

    g_signal_connect_data(G_OBJECT(image_box), "size-allocate",
                          G_CALLBACK(resize_image_keeping_aspect_ratio),
                          image_to_resize, NULL, 0);
}

/**
 * Calculates new position and size for an image (passed with
 * pixbuf as a sctruct in 'user_data' parameter), and then
 * moves it to new position inside layout container (image_box).
 *
 * Documentation advises not to use 'allocation' parameter,
 * which passes inside this function by default, but rather
 * we should recalculate the size of an allocation area
 * ourselves. That's why i marked it as unused.
 **/
void resize_image_keeping_aspect_ratio(GtkWidget *image_box,
                                       GdkRectangle *allocation
                                       __attribute__((unused)),
                                       gpointer user_data)
{
    GdkPixbuf *pxbuffer_after_scale;

    GtkWidget *image = (GtkWidget *)((ResizeImage *)user_data)->image;
    GdkPixbuf *pixbuf = (GdkPixbuf *)((ResizeImage *)user_data)->pixbuf;

    gint alloc_width, alloc_height;
    gtk_window_get_size(GTK_WINDOW(gtk_widget_get_toplevel(image_box)),
                        &alloc_width, &alloc_height);

    gint x_coord = 0;
    gint y_coord = 0;

    gint height = alloc_height;
    gint width =
        (gdk_pixbuf_get_width(pixbuf) * height) / gdk_pixbuf_get_height(pixbuf);

    pxbuffer_after_scale =
        gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);

    if (width < alloc_width)
    {
        x_coord = (alloc_width - width) / 2;
        gtk_layout_move(GTK_LAYOUT(image_box), image, x_coord, y_coord);
    }

    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pxbuffer_after_scale);
    g_object_unref(pxbuffer_after_scale); // free cause gdk
}
