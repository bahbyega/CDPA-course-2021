#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <stdio.h>

#include "gui_components/imagebox.h"
#include "gui_components/listbox.h"
#include "gui_components/menubar.h"

void initialize_window (GtkWidget *);
void setup_init_button (GtkWidget *);
void on_init_btn_click (GtkWidget *);


void setup_window_layout_elements(GtkWidget *, GdkPixbuf *, gulong);

gulong get_file_size(gchar *);

#endif /* __GUI_WINDOW_H__ */
