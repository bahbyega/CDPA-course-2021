#ifndef __FILTERSPANEL__
#define __FILTERSPANEL__

#include "../filters.h"
#include "imagebox.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <sys/time.h>

void setup_filters_on_main_window(GtkWidget *, GdkPixbuf *, const gchar *);

GtkWidget *setup_predefined_filters_page(GdkPixbuf *, const gchar *);
GtkWidget *setup_custom_filters_page(GdkPixbuf *, const gchar *);

typedef struct {
  GdkPixbuf *pixbuf;
  const gchar *filename;
  GtkSpinButton *apply_times_btn;
  GtkCheckButton *use_gpu_btn;
} ImageFileData;

typedef struct {
  GdkPixbuf *pixbuf;
  GtkEntry *values_entry;
  GtkSpinButton *size_btn;
  GtkSpinButton *factor_btn;
  GtkSpinButton *bias_btn;
  const gchar *filename;
} CustomFilterData;

void on_filter_type_change(GtkWidget *, gpointer);
void on_flip_x_btn_click(GtkWidget *, gpointer);
void on_flip_y_btn_click(GtkWidget *, gpointer);
void on_blur_btn_click(GtkWidget *, gpointer);
void on_sharp_btn_click(GtkWidget *, gpointer);
void on_edges_btn_click(GtkWidget *, gpointer);
void on_mblur_btn_click(GtkWidget *, gpointer);
void on_apply_btn_click(GtkWidget *, gpointer);
void on_save_menu_item_click(GtkWidget *, gpointer);
void on_save_as_menu_item_click(GtkWidget *, gpointer);

double *parse_kernelstr_for_kernel(const char *, gint, gint);
const char *generate_default_kernel_str(gint);
void show_resulting_image_in_new_window(GdkPixbuf *, const gchar *);

#endif /* __FILTERSPANEL__ */