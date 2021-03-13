#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include <gtk/gtk.h>

void initialize_window (GtkWidget *);
void setup_button_on_opening_window (GtkWidget *,GtkWidget *);
void on_open_btn_click (GtkWidget *,gpointer );
void open_dialog ();

#endif /* __MAIN_WINDOW_H__ */

