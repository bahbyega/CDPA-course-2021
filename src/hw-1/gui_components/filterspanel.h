#ifndef __FILTERSPANEL__
#define __FILTERSPANEL__

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "../filters.h"

void setup_filters_on_main_window(GtkWidget *, GdkPixbuf *);

GtkWidget *setup_predefined_filters_page(GdkPixbuf *);
GtkWidget *setup_custom_filters_page(GdkPixbuf *);

void on_flip_x_btn_click(GtkWidget *, gpointer );
void on_flip_y_btn_click(GtkWidget *, gpointer );
void on_blur_btn_click(GtkWidget *, gpointer );
void on_sharp_btn_click(GtkWidget *, gpointer );
void on_edges_btn_click(GtkWidget *, gpointer );
void on_mblur_btn_click(GtkWidget *, gpointer );

void show_resulting_image_in_new_window(GdkPixbuf *);

#endif /* __FILTERSPANEL__ */