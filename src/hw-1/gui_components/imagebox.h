#ifndef __IMAGE_BOX_LAYOUT__
#define __IMAGE_BOX_LAYOUT__

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

typedef struct 
{
    GtkWidget *image;
    GdkPixbuf *pixbuf;
} ResizeImage;

void setup_image_on_main_window(GtkWidget *,GtkWidget *, GdkPixbuf *);
void resize_image_keeping_aspect_ratio(GtkWidget *,
                                       GdkRectangle *,
                                       gpointer);
gboolean resize_image(GtkWidget *, GdkRectangle *, gpointer );

#endif /* __IMAGE_BOX_LAYOUT__ */
