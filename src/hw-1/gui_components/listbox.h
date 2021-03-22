#ifndef __LISTBOX__
#define __LISTBOX__

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <string.h>

void setup_listbox_on_main_window(GtkWidget *, GtkWidget *, gsize);
void add_item_to_listbox(GtkWidget *, char *);

gchar *get_image_dimensions(GtkWidget *);
gchar *get_image_file_size_from_gulong_to_char(gulong);

#endif /* __LISTBOX__ */
