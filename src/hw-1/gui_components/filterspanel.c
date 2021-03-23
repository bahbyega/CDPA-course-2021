#include "filterspanel.h"

void setup_filters_on_main_window(GtkWidget *box, GdkPixbuf *pixbuf)
{
    GtkWidget *gaussian_blur_btn;
    GtkWidget *edge_detect_btn;
    GtkWidget *sharpening_btn;

    gaussian_blur_btn = gtk_button_new_with_label("Blur");
    edge_detect_btn = gtk_button_new_with_label("Edge");
    sharpening_btn = gtk_button_new_with_label("Sharp");

    gtk_box_pack_start(GTK_BOX(box), gaussian_blur_btn, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), edge_detect_btn, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), sharpening_btn, TRUE, FALSE, 0);

    g_signal_connect_data(G_OBJECT(gaussian_blur_btn), "clicked",
                                G_CALLBACK(on_blur_btn_click), pixbuf, NULL, 0);
}

void on_blur_btn_click(GtkWidget *caller
                       __attribute__((unused)), 
                       gpointer data)
{
    apply_flip_on_x_axis((GdkPixbuf *)data); // not doing anything with it for now
}