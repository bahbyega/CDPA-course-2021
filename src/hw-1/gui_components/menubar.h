#ifndef __MENUBAR__
#define __MENUBAR__

#include <gtk/gtk.h>
#include "../gui-window.h"    

void setup_menu_bar_on_main_window(GtkWidget *);
void open_github_page();
void on_open_menu_item_click(GtkWidget *, gpointer);

#endif /* __MENUBAR__ */
