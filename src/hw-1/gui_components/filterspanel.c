#include "filterspanel.h"

enum 
{
    FILTER_ID_NONE = 0,
    FILTER_ID_GAUSBLUR = 1,
    FILTER_ID_SHARPENING = 2,
    FILTER_ID_EDGES = 3
};

#define GAUS_BLUR_KERNEL_STR  "{0.0, 0.2, 0.0},{0.2, 0.2, 0.2},{0.0, 0.2, 0.0}"
#define SHARPENING_KERNEL_STR "{-1, -1, -1},{-1, 9, -1},{-1, -1, -1}"
#define EDGES_KERNEL_STR      "{0, -1, 0},{-1, 4, -1},{0, -1, 0}"
#define DEFAULT_KERNEL_STR    "{0, 0, 0},{0, 1, 0},{0, 0, 0}"

/** 
 * Sets up filters panel on main window. It consists of a notebook
 * with two pages. Each page is then configured in other functions.
 **/
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

/**
 * Configures page with predefined filters. These filters are 5x5
 * matrices which configured in filters.c file.
 **/
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

/**
 * Configures page where you can specify your custom filter.
 **/
GtkWidget *setup_custom_filters_page(GdkPixbuf *pixbuf __attribute__((unused)))
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget *filter_type_box = gtk_combo_box_text_new_with_entry();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(filter_type_box), 0, "None");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(filter_type_box), 0, "Blur");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(filter_type_box), 0, "Sharpening");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(filter_type_box), 0, "Edge detection");

    GtkAdjustment *size_config = gtk_adjustment_new(3, 1, 9, 2, 0, 0);
    GtkWidget     *size_btn    = gtk_spin_button_new(size_config, 1.0, 0);

    const gchar *info_text = "Select filter type. That\n"
                             "will generate values in box\n"
                             "below. You can set up your\n"
                             "own kernel values, but make\n"
                             "sure that kernel size matches\n"
                             "value in filter size box.\n"
                             "Basically, a kernel is an \n"
                             "array of doubles, so you can\n"
                             "input this array in any form.\n"
                             "{{a,b},{c,d}} is preffered.\n"
                             "When ready press apply.";
    GtkWidget *info_label = gtk_label_new(info_text);

    GtkWidget *values_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(values_entry), "Default values (zeros now)");

    GtkWidget *type_label = gtk_label_new ("Filter type:");
    GtkWidget *size_label = gtk_label_new ("Filter size:");
    GtkWidget *factor_label = gtk_label_new ("Weight (factor):");
    GtkWidget *bias_label = gtk_label_new ("Bias (brightness):");

    GtkAdjustment *factor_config = gtk_adjustment_new(1.0, 0.0, 5.0, 0.0000001, 0.00001, 0);
    GtkWidget     *factor_btn    = gtk_spin_button_new(factor_config, 1.0, 7);
    GtkAdjustment *bias_config   = gtk_adjustment_new(0, -255, 255, 1, 5, 0);
    GtkWidget     *bias_btn      = gtk_spin_button_new(bias_config, 1.0, 0);
    
    GtkWidget *apply_btn = gtk_button_new_with_label("Apply filter");

    gtk_box_pack_start(GTK_BOX(box), type_label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), filter_type_box, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), size_label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), size_btn, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), info_label, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), values_entry, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), factor_label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), factor_btn, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), bias_label, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(box), bias_btn, TRUE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), apply_btn, TRUE, FALSE, 0);

    g_signal_connect(G_OBJECT(filter_type_box), "changed", G_CALLBACK(on_filter_type_change),
                     values_entry);

    // initialize widgets' data and pass it to callback
    CustomFilterData *data;
    data = (CustomFilterData *)calloc(1, sizeof(*data));
    
    data->pixbuf    = pixbuf;
    data->values_entry = GTK_ENTRY(values_entry);
    data->size_btn  = GTK_SPIN_BUTTON(size_btn);
    data->factor_btn= GTK_SPIN_BUTTON(factor_btn);
    data->bias_btn  = GTK_SPIN_BUTTON(bias_btn);

    g_signal_connect(G_OBJECT(apply_btn), "clicked", G_CALLBACK(on_apply_btn_click),
                     data);
    
    return box;
}

/* event handlers */
void on_filter_type_change(GtkWidget *caller, 
                           gpointer data)
{
    gint filter_type_id = gtk_combo_box_get_active(GTK_COMBO_BOX(caller));

    const char *kernel_str = generate_default_kernel_str(filter_type_id);

    gtk_entry_set_text(GTK_ENTRY(data), kernel_str);
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

    GdkPixbuf *res_image = apply_filter_parallel((GdkPixbuf *)data,
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

    GdkPixbuf *res_image = apply_filter_parallel((GdkPixbuf *)data,
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

    GdkPixbuf *res_image = apply_filter_parallel((GdkPixbuf *)data,
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

    GdkPixbuf *res_image = apply_filter_parallel((GdkPixbuf *)data,
                                        kernel,
                                        ker_width, ker_height,
                                        factor, bias);

    show_resulting_image_in_new_window(res_image);
}

void on_apply_btn_click(GtkWidget *caller
                        __attribute__((unused)), 
                        gpointer data)
{
    CustomFilterData *d = (CustomFilterData *)data;
    const char *kernelstr = gtk_entry_get_text(d->values_entry);

    GdkPixbuf *pixbuf = d->pixbuf;
    gint       ker_width = gtk_spin_button_get_value_as_int(d->size_btn);
    gint       ker_height = gtk_spin_button_get_value_as_int(d->size_btn);
    double    *kernel = parse_kernelstr_for_kernel(kernelstr, ker_width, ker_height);
    double     factor = gtk_spin_button_get_value(d->factor_btn);
    double     bias = gtk_spin_button_get_value(d->bias_btn);

    GdkPixbuf *res_image = apply_filter_parallel(pixbuf,
                                        kernel,
                                        ker_width, ker_height,
                                        factor, bias);
    
    show_resulting_image_in_new_window(res_image);
}

/**
 * It gets called for generating default value in entry box.
 * Returns one of the predefined string literals.
 **/
const char *generate_default_kernel_str(gint id)
{
    switch (id)
    {
    case FILTER_ID_NONE:
        return DEFAULT_KERNEL_STR;
        break;
    case FILTER_ID_GAUSBLUR:
        return GAUS_BLUR_KERNEL_STR;
        break;
    case FILTER_ID_SHARPENING:
        return SHARPENING_KERNEL_STR;
        break;
    case FILTER_ID_EDGES:
        return EDGES_KERNEL_STR;
    
    default:
        return DEFAULT_KERNEL_STR;
        break;
    }
}

/** 
 * Helper function for next function.
 * Parameter 'str' is a string representing a kernel.
 * This function basically parses a string with
 * kernel values and stores them in 'res' parameter.
 **/
void split_string(char *str, double *res, gint width, gint height)
{
    const char delim[] = " {},[]";

    gint i = 0;
    gint exact_kernel_size = width*height;

    char *token = strtok(str, delim);
    while(token != NULL)
    {
        res[i++] = strtod(token, NULL);
        token = strtok(NULL, delim);

        if (i > exact_kernel_size)
        {
            g_print("Your filter kernel is bigger in size than the value in\n"
                    "the filter size box (%d). The result is going to be incorrect.\n",
                    width);
            break;
        }
    }

    if (i < exact_kernel_size)
    {
        g_print("Your filter kernel is smaller in size than the value in\n"
                "the filter size box (%d). The result is going to be incorrect.\n",
                width);
        return;
    }
}

/**
 * Allocates memory for a user specified kernel and returns
 * a pointer to its first element. Width and Height are also user
 * specified. And it's not checked anywhere if kernel size matches 
 * with width and height. So it is on user to make them the same.
 * But if they don't match the difference is only in 0s in the kernel.
 **/
double *parse_kernelstr_for_kernel(const char *str, gint width, gint height)
{
    double *res = (double *)calloc(width*height, sizeof(double));

    char copy[strlen(str)];
    strcpy(copy, str); // need a copy cause split_string will modify string
    split_string(copy, res, width, height);

    return &res[0];
}

/**
 * Just shows the resulting image.
 **/
void show_resulting_image_in_new_window(GdkPixbuf *pixbuf)
{
    GtkWidget *res_window;
    GtkWidget *image;
    GtkWidget *image_box;

    res_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(res_window), "Filtering result");
    gtk_window_set_default_size(GTK_WINDOW(res_window), 700, 400);
    gtk_window_set_type_hint(GTK_WINDOW(res_window), GDK_WINDOW_TYPE_HINT_DIALOG);

    image = gtk_image_new();
    image_box = gtk_layout_new(0, 0);

    gtk_layout_put(GTK_LAYOUT(image_box), image, 0, 0);
    gtk_container_add(GTK_CONTAINER(res_window), image_box);
    setup_image_on_main_window(image_box, image, pixbuf);

    gtk_widget_show_all(res_window);
}