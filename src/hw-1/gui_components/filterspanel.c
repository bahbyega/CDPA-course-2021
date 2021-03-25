#include "filterspanel.h"

void setup_filters_on_main_window(GtkWidget *notebook, GdkPixbuf *pixbuf)
{
    GtkWidget *predefined_page = setup_predefined_filters_page(pixbuf);
    GtkWidget *custom_page = setup_custom_filters_page(pixbuf);

    GtkWidget *predefined_label = gtk_label_new("Predefined filters");
    GtkWidget *custom_label = gtk_label_new("Custom filter");
    gtk_label_set_angle(GTK_LABEL(predefined_label), 90); // make it vertical
    gtk_label_set_angle(GTK_LABEL(custom_label), 90);

    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_LEFT);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), predefined_page, predefined_label);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), custom_page, custom_label);
}

GtkWidget *setup_predefined_filters_page(GdkPixbuf *pixbuf)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *flip_x_btn = gtk_button_new_with_label("Flip on x axis");
    GtkWidget *flip_y_btn = gtk_button_new_with_label("Flip on y axis");
    GtkWidget *blur_btn   = gtk_button_new_with_label("Apply gaus blur");
    GtkWidget *sharp_btn  = gtk_button_new_with_label("Apply sharpening");
    GtkWidget *edge_btn   = gtk_button_new_with_label("Apply finding edges");
    GtkWidget *mblur_btn  = gtk_button_new_with_label("Apply motion blur");

    gtk_box_pack_start(GTK_BOX(box), flip_x_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), flip_y_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), blur_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), sharp_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), edge_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), mblur_btn, TRUE, FALSE, 5);

    g_signal_connect(G_OBJECT(flip_x_btn), "clicked", G_CALLBACK(on_flip_x_btn_click),
                     pixbuf);
    g_signal_connect(G_OBJECT(flip_y_btn), "clicked", G_CALLBACK(on_flip_y_btn_click),
                     pixbuf);
    g_signal_connect(G_OBJECT(blur_btn), "clicked", G_CALLBACK(on_blur_btn_click),
                     pixbuf);
    g_signal_connect(G_OBJECT(sharp_btn), "clicked", G_CALLBACK(on_sharp_btn_click),
                     pixbuf);
    g_signal_connect(G_OBJECT(edge_btn), "clicked", G_CALLBACK(on_edges_btn_click),
                     pixbuf);
    g_signal_connect(G_OBJECT(mblur_btn), "clicked", G_CALLBACK(on_mblur_btn_click),
                     pixbuf);
    
    return box;
}

GtkWidget *setup_custom_filters_page(GdkPixbuf *pixbuf __attribute__((unused)))
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *filter_type_box = gtk_combo_box_text_new_with_entry();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(filter_type_box), 0, "Blur");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(filter_type_box), 0, "Sharpening");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(filter_type_box), 0, "Edge detection");

    GtkAdjustment *config = gtk_adjustment_new(1, 3, 5, 2, 0, 0);
    GtkWidget *size_btn = gtk_spin_button_new(config, 1.0, 0);

    const gchar *info_text = "Please select filter type\n"
                             "and filter size. That will\n"
                             "generate default filter\n"
                             "values in box below. \n"
                             "Also, specify weight and\n"
                             "bias below. When ready \n"
                             "press apply.";
    GtkWidget *info_label = gtk_label_new(info_text);

    /*GtkWidget *values_entry = gtk_text_view_new();
    GtkTextBuffer *text_buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(values_entry));
    gtk_text_view_set_editable(GTK_TEXT_VIEW(values_entry), TRUE);
    gtk_text_buffer_create_tag(text_buf, "blue_fg", "foreground", "blue", NULL); 
    gtk_text_buffer_insert_with_tags_by_name(text_buf, NULL, 
        info_text, -1, "blue_fg",  NULL);*/

    GtkWidget *weight_entry = gtk_entry_new();
    GtkWidget *bias_entry   = gtk_entry_new();
    GtkWidget *apply_btn    = gtk_button_new_with_label("Apply filter");
    
    gtk_entry_set_placeholder_text(GTK_ENTRY(weight_entry), "Weight (0.0 - 1.0)");
    gtk_entry_set_placeholder_text(GTK_ENTRY(bias_entry), "Bias (0 - 255)");

    gtk_box_pack_start(GTK_BOX(box), filter_type_box, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), size_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), info_label, TRUE, FALSE, 5);
    //gtk_box_pack_start(GTK_BOX(box), values_entry, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), weight_entry, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), bias_entry, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), apply_btn, TRUE, FALSE, 5);

    return box;
}

void on_flip_x_btn_click(GtkWidget *caller
                         __attribute__((unused)), 
                         gpointer data)
{
    GdkPixbuf *res_image = apply_flip((GdkPixbuf *)data, TRUE);
    show_resulting_image_in_new_window(res_image);
}

void on_flip_y_btn_click(GtkWidget *caller
                         __attribute__((unused)), 
                         gpointer data)
{
    GdkPixbuf *res_image = apply_flip((GdkPixbuf *)data, FALSE);
    show_resulting_image_in_new_window(res_image);
}

void on_blur_btn_click(GtkWidget *caller
                       __attribute__((unused)), 
                       gpointer data)
{
    double *kernel = &gaussian_blur_kernel_5x5[0][0];
    gint    ker_width = 5, ker_height = 5;
    double  factor = 1/256.0, bias = 0.0;

    GdkPixbuf *res_image = apply_filter((GdkPixbuf *)data,
                                        kernel,
                                        ker_width, ker_height,
                                        factor, bias);
    
    show_resulting_image_in_new_window(res_image);
}

void on_sharp_btn_click(GtkWidget *caller
                        __attribute__((unused)), 
                        gpointer data)
{
    double *kernel = &sharpening_kernel_5x5[0][0];
    gint    ker_width = 5, ker_height = 5;
    double  factor = 1/9.0, bias = 0.0;

    GdkPixbuf *res_image = apply_filter((GdkPixbuf *)data,
                                        kernel,
                                        ker_width, ker_height,
                                        factor, bias);
    
    show_resulting_image_in_new_window(res_image);
}

void on_edges_btn_click(GtkWidget *caller
                        __attribute__((unused)), 
                        gpointer data)
{
    double *kernel = &edges_kernel_5x5[0][0];
    gint    ker_width = 5, ker_height = 5;
    double  factor = 1.00, bias = 0.0;

    GdkPixbuf *res_image = apply_filter((GdkPixbuf *)data,
                                        kernel,
                                        ker_width, ker_height,
                                        factor, bias);
    
    show_resulting_image_in_new_window(res_image);
}

void on_mblur_btn_click(GtkWidget *caller
                        __attribute__((unused)), 
                        gpointer data)
{
    double *kernel = &motionblur_kernel_9x9[0][0];
    gint    ker_width = 9, ker_height = 9;
    double  factor = 1/9.00, bias = 0.0;

    GdkPixbuf *res_image = apply_filter((GdkPixbuf *)data,
                                        kernel,
                                        ker_width, ker_height,
                                        factor, bias);
    
    show_resulting_image_in_new_window(res_image);
}


void show_resulting_image_in_new_window(GdkPixbuf *pixbuf)
{
    GtkWidget *res_window;
    GtkWidget *image;

    res_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(res_window), "Filtering result");
    gtk_window_set_default_size(GTK_WINDOW(res_window), 320, 200);

    image = gtk_image_new_from_pixbuf(pixbuf);
    gtk_container_add(GTK_CONTAINER(res_window), image);

    gtk_widget_show_all(res_window);
}