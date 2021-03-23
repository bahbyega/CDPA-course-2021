#ifndef __FILTERSPANEL__
#define __FILTERSPANEL__

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "../filters.h"

void setup_filters_on_main_window(GtkWidget *, GdkPixbuf *);
void on_blur_btn_click(GtkWidget *, gpointer);

#endif /* __FILTERSPANEL__ */