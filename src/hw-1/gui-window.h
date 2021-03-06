#ifndef __GUI_WINDOW_H__
#define __GUI_WINDOW_H__

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <stdio.h>

#include "gui_components/imagebox.h"
#include "gui_components/listbox.h"
#include "gui_components/menubar.h"
#include "gui_components/filterspanel.h"

void initialize_window(GtkWidget *);
void setup_init_button(GtkWidget *);
void on_init_btn_click(GtkWidget *);

void setup_window_layout_elements(GtkWidget *, GdkPixbuf *,
                                  const gchar *, gulong);

gulong get_file_size(const gchar *);

#endif /* __GUI_WINDOW_H__ */
