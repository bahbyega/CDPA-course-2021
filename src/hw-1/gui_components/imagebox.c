#include "imagebox.h"


void setup_image_on_main_window(GtkWidget *image_box, GtkWidget *image, GdkPixbuf *image_pixbuf)
{
    gtk_widget_set_hexpand(image, TRUE);
    gtk_widget_set_vexpand(image, TRUE);

    gtk_widget_set_halign(image, GTK_ALIGN_BASELINE );
    gtk_widget_set_valign(image, GTK_ALIGN_BASELINE );

    gtk_widget_set_size_request( image, 100, 100);
    
    // use this struct to pass data to the event handler below
    ResizeImage *image_to_resize;
    image_to_resize = (ResizeImage *)malloc(sizeof(*image_to_resize));
    image_to_resize->image = image;
    image_to_resize->pixbuf = image_pixbuf;

    g_signal_connect_data  (G_OBJECT(image_box), "size-allocate", 
                            G_CALLBACK(resize_image_keeping_aspect_ratio),
                            image_to_resize, NULL, 0);
}

void resize_image_keeping_aspect_ratio(GtkWidget *image_box,
                                           GdkRectangle *allocation,
                                           gpointer user_data)
{
    GdkPixbuf *pxbuffer_after_scale;

    GtkWidget *image  = (GtkWidget *) ((ResizeImage *) user_data)->image;
    GdkPixbuf *pixbuf = (GdkPixbuf *) ((ResizeImage *) user_data)->pixbuf;

    gint x_coord = 0;
    gint y_coord = 0;

    gint height = allocation->height;
    gint width = (gdk_pixbuf_get_width(pixbuf) * height) / gdk_pixbuf_get_height(pixbuf);

    pxbuffer_after_scale = gdk_pixbuf_scale_simple(pixbuf, width, height, GDK_INTERP_BILINEAR);

    if (width < allocation->width) 
    {
        x_coord = (allocation->width - width) / 2;
        gtk_layout_move(GTK_LAYOUT(image_box), image, x_coord, y_coord);
    }
    
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), pxbuffer_after_scale);
    g_object_unref (pxbuffer_after_scale); // free cause gdk
}
