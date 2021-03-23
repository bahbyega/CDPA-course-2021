#ifndef __FILTERS__
#define __FILTERS__

#include <stdint.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

GdkPixbuf *apply_flip_on_x_axis(GdkPixbuf *);
GdkPixbuf *apply_flip_on_y_axis(GdkPixbuf *);

#endif /* __FILTERS__ */