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
void setup_filters_on_main_window(GtkWidget *notebook, GdkPixbuf *pixbuf, const gchar *filename)
{
    GtkWidget *predefined_page = setup_predefined_filters_page(pixbuf, filename);
    GtkWidget *custom_page = setup_custom_filters_page(pixbuf, filename);
    GtkWidget *folder_page = setup_filters_for_folder_page();

    GtkWidget *predefined_label = gtk_label_new("Predefined filters");
    GtkWidget *custom_label = gtk_label_new("Custom filter");
    GtkWidget *folder_label = gtk_label_new("Folder filter");
    gtk_label_set_angle(GTK_LABEL(predefined_label), 90); // make it vertical
    gtk_label_set_angle(GTK_LABEL(custom_label), 90);
    gtk_label_set_angle(GTK_LABEL(folder_label), 90);

    gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_LEFT);

    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), predefined_page, predefined_label);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), custom_page, custom_label);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), folder_page, folder_label);
}

/**
 * Configures page with predefined filters. These filters are 5x5
 * matrices which configured in filters.c file.
 **/
GtkWidget *setup_predefined_filters_page(GdkPixbuf *pixbuf, const gchar *filename)
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    GtkWidget *flip_x_btn = gtk_button_new_with_label("Flip on x axis");
    GtkWidget *flip_y_btn = gtk_button_new_with_label("Flip on y axis");
    GtkWidget *blur_btn   = gtk_button_new_with_label("Apply gaus blur");
    GtkWidget *sharp_btn  = gtk_button_new_with_label("Apply sharpening");
    GtkWidget *edge_btn   = gtk_button_new_with_label("Apply finding edges");
    GtkWidget *mblur_btn  = gtk_button_new_with_label("Apply motion blur");
    
    GtkWidget     *apply_times_label  = gtk_label_new ("Apply filter n times:");
    GtkAdjustment *apply_times_config = gtk_adjustment_new(1, 1, 10, 1, 0, 0);
    GtkWidget     *apply_times_btn    = gtk_spin_button_new(apply_times_config, 1.0, 0);

    GtkWidget *use_gpu_btn = gtk_check_button_new_with_label("Use GPU");

    gtk_box_pack_start(GTK_BOX(box), flip_x_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), flip_y_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), blur_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), sharp_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), edge_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), mblur_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), apply_times_label, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), apply_times_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), use_gpu_btn, TRUE, FALSE, 5);

    ImageFileData *data;
    data = (ImageFileData *)calloc(1, sizeof(ImageFileData));
    data->pixbuf = pixbuf;
    data->filename = filename;
    data->apply_times_btn = GTK_SPIN_BUTTON(apply_times_btn);
    data->use_gpu_btn = GTK_CHECK_BUTTON(use_gpu_btn);

    g_signal_connect(G_OBJECT(flip_x_btn), "clicked", G_CALLBACK(on_flip_x_btn_click),
                     data);
    g_signal_connect(G_OBJECT(flip_y_btn), "clicked", G_CALLBACK(on_flip_y_btn_click),
                     data);
    g_signal_connect(G_OBJECT(blur_btn), "clicked", G_CALLBACK(on_blur_btn_click),
                     data);
    g_signal_connect(G_OBJECT(sharp_btn), "clicked", G_CALLBACK(on_sharp_btn_click),
                     data);
    g_signal_connect(G_OBJECT(edge_btn), "clicked", G_CALLBACK(on_edges_btn_click),
                     data);
    g_signal_connect(G_OBJECT(mblur_btn), "clicked", G_CALLBACK(on_mblur_btn_click),
                     data);
    
    return box;
}

/**
 * Configures page where you can specify your custom filter.
 **/
GtkWidget *setup_custom_filters_page(GdkPixbuf *pixbuf, const gchar *filename)
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
    data->filename = filename;

    g_signal_connect(G_OBJECT(apply_btn), "clicked", G_CALLBACK(on_apply_btn_click),
                     data);
    
    return box;
}

/**
 * Configures page where you can apply filters to a whole folder.
 **/
GtkWidget *setup_filters_for_folder_page()
{
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *src_folder_label = gtk_label_new ("Chosen input folder:");
    GtkWidget *src_foldername_entry = gtk_entry_new();
    GtkWidget *out_folder_label = gtk_label_new ("Chosen output folder:");
    GtkWidget *out_foldername_entry = gtk_entry_new();

    GtkWidget *choose_src_folder_btn = gtk_button_new_with_label("Choose folder with images to apply filters to");
    GtkWidget *choose_out_folder_btn = gtk_button_new_with_label("Choose output folder");

    GtkWidget *blur_btn   = gtk_button_new_with_label("Apply gaus blur");
    GtkWidget *sharp_btn  = gtk_button_new_with_label("Apply sharpening");
    GtkWidget *edge_btn   = gtk_button_new_with_label("Apply finding edges");
    GtkWidget *mblur_btn  = gtk_button_new_with_label("Apply motion blur");

    GtkWidget     *apply_times_label  = gtk_label_new ("Apply filter n times:");
    GtkAdjustment *apply_times_config = gtk_adjustment_new(1, 1, 10, 1, 0, 0);
    GtkWidget     *apply_times_btn    = gtk_spin_button_new(apply_times_config, 1.0, 0);

    GtkWidget *use_gpu_btn = gtk_check_button_new_with_label("Use GPU");

    FolderData *data = (FolderData *)calloc(1, sizeof(FolderData));
    data->apply_times_btn = GTK_SPIN_BUTTON(apply_times_btn);
    data->use_gpu_btn = GTK_CHECK_BUTTON(use_gpu_btn);
    data->src_folder_entry = GTK_ENTRY(src_foldername_entry);
    data->out_folder_entry = GTK_ENTRY(out_foldername_entry);
    
    g_signal_connect(G_OBJECT(choose_src_folder_btn), "clicked", G_CALLBACK(on_choose_folder_btn_click),
                     src_foldername_entry);
    g_signal_connect(G_OBJECT(choose_out_folder_btn), "clicked", G_CALLBACK(on_choose_folder_btn_click),
                     out_foldername_entry);

    g_signal_connect(G_OBJECT(blur_btn), "clicked", G_CALLBACK(on_blur_folder_btn_click),
                     data);
    g_signal_connect(G_OBJECT(sharp_btn), "clicked", G_CALLBACK(on_sharp_folder_btn_click),
                     data);
    g_signal_connect(G_OBJECT(edge_btn), "clicked", G_CALLBACK(on_edges_folder_btn_click),
                     data);
    g_signal_connect(G_OBJECT(mblur_btn), "clicked", G_CALLBACK(on_mblur_folder_btn_click),
                     data);

    gtk_box_pack_start(GTK_BOX(box), choose_src_folder_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), src_folder_label, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), src_foldername_entry, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), choose_out_folder_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), out_folder_label, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), out_foldername_entry, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), blur_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), sharp_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), edge_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), mblur_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), apply_times_label, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), apply_times_btn, TRUE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), use_gpu_btn, TRUE, FALSE, 5);

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
    GdkPixbuf *src_pixbuf = ((ImageFileData *)data)->pixbuf;
    const gchar *filename = ((ImageFileData *)data)->filename;

    GdkPixbuf *res_pixbuf = apply_flip(src_pixbuf, TRUE);
    show_resulting_image_in_new_window(res_pixbuf, filename);
}

void on_flip_y_btn_click(GtkWidget *caller
                         __attribute__((unused)), 
                         gpointer data)
{
    GdkPixbuf *src_pixbuf = ((ImageFileData *)data)->pixbuf;
    const gchar *filename = ((ImageFileData *)data)->filename;

    GdkPixbuf *res_pixbuf = apply_flip(src_pixbuf, FALSE);
    show_resulting_image_in_new_window(res_pixbuf, filename);
}

void on_blur_btn_click(GtkWidget *caller
                       __attribute__((unused)), 
                       gpointer data)
{
    GdkPixbuf *src_pixbuf = ((ImageFileData *)data)->pixbuf;
    const gchar *filename = ((ImageFileData *)data)->filename;

    gint apply_times = gtk_spin_button_get_value_as_int(((ImageFileData *)data)->apply_times_btn);
    gboolean use_gpu_flag 
        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(((ImageFileData *)data)->use_gpu_btn));
    
    FilterData filter_data = {
        .use_gpu_flag = use_gpu_flag,
        .apply_times = apply_times,
        .bias = 0.0,
        .factor = 1/256.0,
        .ker_width = 5,
        .kernel = &gaussian_blur_kernel_5x5[0][0]
    };

    GdkPixbuf *out_pixbuf = apply_filter_service(src_pixbuf, &filter_data);

    show_resulting_image_in_new_window(out_pixbuf, filename);
}

void on_sharp_btn_click(GtkWidget *caller
                        __attribute__((unused)), 
                        gpointer data)
{
    GdkPixbuf *src_pixbuf = ((ImageFileData *)data)->pixbuf;
    const gchar *filename = ((ImageFileData *)data)->filename;
    
    gint apply_times = gtk_spin_button_get_value_as_int(((ImageFileData *)data)->apply_times_btn);
    gboolean use_gpu_flag 
        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(((ImageFileData *)data)->use_gpu_btn));

    FilterData filter_data = {
        .use_gpu_flag = use_gpu_flag,
        .apply_times = apply_times,
        .bias = 0.0,
        .factor = 1/9.0,
        .ker_width = 5,
        .kernel = &sharpening_kernel_5x5[0][0]
    };

    GdkPixbuf *out_pixbuf = apply_filter_service(src_pixbuf, &filter_data);

    show_resulting_image_in_new_window(out_pixbuf, filename);
}

void on_edges_btn_click(GtkWidget *caller
                        __attribute__((unused)), 
                        gpointer data)
{
    GdkPixbuf *src_pixbuf = ((ImageFileData *)data)->pixbuf;
    const gchar *filename = ((ImageFileData *)data)->filename;

    gint apply_times = gtk_spin_button_get_value_as_int(((ImageFileData *)data)->apply_times_btn);
    gboolean use_gpu_flag 
        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(((ImageFileData *)data)->use_gpu_btn));

    FilterData filter_data = {
        .use_gpu_flag = use_gpu_flag,
        .apply_times = apply_times,
        .bias = 0.0,
        .factor = 1.00,
        .ker_width = 5,
        .kernel = &edges_kernel_5x5[0][0]
    };

    GdkPixbuf *out_pixbuf = apply_filter_service(src_pixbuf, &filter_data);

    show_resulting_image_in_new_window(out_pixbuf, filename);
}

void on_mblur_btn_click(GtkWidget *caller
                        __attribute__((unused)), 
                        gpointer data)
{
    GdkPixbuf *src_pixbuf = ((ImageFileData *)data)->pixbuf;
    const gchar *filename = ((ImageFileData *)data)->filename;

    gint apply_times = gtk_spin_button_get_value_as_int(((ImageFileData *)data)->apply_times_btn);
    gboolean use_gpu_flag 
        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(((ImageFileData *)data)->use_gpu_btn));

    FilterData filter_data = {
        .use_gpu_flag = use_gpu_flag,
        .apply_times = apply_times,
        .bias = 0.0,
        .factor = 1/9.00,
        .ker_width = 9,
        .kernel = &motionblur_kernel_9x9[0][0]
    };

    GdkPixbuf *out_pixbuf = apply_filter_service(src_pixbuf, &filter_data);

    show_resulting_image_in_new_window(out_pixbuf, filename);
}

void on_blur_folder_btn_click(GtkWidget *caller
                       __attribute__((unused)), 
                       gpointer data)
{
    gint apply_times = gtk_spin_button_get_value_as_int(((FolderData *)data)->apply_times_btn);
    gboolean use_gpu_flag 
        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(((FolderData *)data)->use_gpu_btn));
    GtkEntry *src_folder_entry = GTK_ENTRY(((FolderData *)data)->src_folder_entry);
    GtkEntry *out_folder_entry = GTK_ENTRY(((FolderData *)data)->out_folder_entry);

    FilterData filter_data = {
        .use_gpu_flag = use_gpu_flag,
        .apply_times = apply_times,
        .bias = 0.0,
        .factor = 1/256.0,
        .ker_width = 5,
        .kernel = &gaussian_blur_kernel_5x5[0][0]
    };

    parallel_folder_service(gtk_entry_get_text(src_folder_entry),
                                        gtk_entry_get_text(out_folder_entry),
                                        &filter_data);
    quick_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(src_folder_entry))), "Success");
}

void on_sharp_folder_btn_click(GtkWidget *caller
                        __attribute__((unused)), 
                        gpointer data)
{
    gint apply_times = gtk_spin_button_get_value_as_int(((FolderData *)data)->apply_times_btn);
    gboolean use_gpu_flag 
        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(((FolderData *)data)->use_gpu_btn));
    GtkEntry *src_folder_entry = GTK_ENTRY(((FolderData *)data)->src_folder_entry);
    GtkEntry *out_folder_entry = GTK_ENTRY(((FolderData *)data)->out_folder_entry);

    FilterData filter_data = {
        .use_gpu_flag = use_gpu_flag,
        .apply_times = apply_times,
        .bias = 0.0,
        .factor = 1/9.0,
        .ker_width = 5,
        .kernel = &sharpening_kernel_5x5[0][0]
    };

    parallel_folder_service(gtk_entry_get_text(src_folder_entry),
                                        gtk_entry_get_text(out_folder_entry),
                                        &filter_data);
    quick_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(src_folder_entry))), "Success");
}

void on_edges_folder_btn_click(GtkWidget *caller
                        __attribute__((unused)), 
                        gpointer data)
{
    gint apply_times = gtk_spin_button_get_value_as_int(((FolderData *)data)->apply_times_btn);
    gboolean use_gpu_flag 
        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(((FolderData *)data)->use_gpu_btn));
    GtkEntry *src_folder_entry = GTK_ENTRY(((FolderData *)data)->src_folder_entry);
    GtkEntry *out_folder_entry = GTK_ENTRY(((FolderData *)data)->out_folder_entry);

    FilterData filter_data = {
        .use_gpu_flag = use_gpu_flag,
        .apply_times = apply_times,
        .bias = 0.0,
        .factor = 1.00,
        .ker_width = 5,
        .kernel = &edges_kernel_5x5[0][0]
    };

    parallel_folder_service(gtk_entry_get_text(src_folder_entry),
                                        gtk_entry_get_text(out_folder_entry),
                                        &filter_data);
    quick_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(src_folder_entry))), "Success");
}

void on_mblur_folder_btn_click(GtkWidget *caller
                        __attribute__((unused)), 
                        gpointer data)
{
    gint apply_times = gtk_spin_button_get_value_as_int(((FolderData *)data)->apply_times_btn);
    gboolean use_gpu_flag 
        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(((FolderData *)data)->use_gpu_btn));
    GtkEntry *src_folder_entry = GTK_ENTRY(((FolderData *)data)->src_folder_entry);
    GtkEntry *out_folder_entry = GTK_ENTRY(((FolderData *)data)->out_folder_entry);

    FilterData filter_data = {
        .use_gpu_flag = use_gpu_flag,
        .apply_times = apply_times,
        .bias = 0.0,
        .factor = 1/9.00,
        .ker_width = 9,
        .kernel = &motionblur_kernel_9x9[0][0]
    };

    parallel_folder_service(gtk_entry_get_text(src_folder_entry),
                                        gtk_entry_get_text(out_folder_entry),
                                        &filter_data);
    quick_message(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(src_folder_entry))), "Success");
}

void on_apply_btn_click(GtkWidget *caller
                        __attribute__((unused)), 
                        gpointer data)
{
    CustomFilterData *d = (CustomFilterData *)data;
    const gchar *kernelstr = gtk_entry_get_text(d->values_entry);

    GdkPixbuf *pixbuf = d->pixbuf;
    gint       ker_width = gtk_spin_button_get_value_as_int(d->size_btn);
    gint       ker_height = gtk_spin_button_get_value_as_int(d->size_btn);
    double    *kernel = parse_kernelstr_for_kernel(kernelstr, ker_width, ker_height);
    double     factor = gtk_spin_button_get_value(d->factor_btn);
    double     bias = gtk_spin_button_get_value(d->bias_btn);
    const gchar *filename = d->filename;

    GdkPixbuf *res_image = apply_filter_parallel(pixbuf,
                                        kernel,
                                        ker_width, ker_height,
                                        factor, bias);
    
    show_resulting_image_in_new_window(res_image, filename);
}

void on_choose_folder_btn_click(GtkWidget *caller, gpointer data)
{
    GtkWidget *dialog;
    GtkWidget *window = gtk_widget_get_toplevel(caller);
    gint res;

    dialog = gtk_file_chooser_dialog_new(
        "Choose an image", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
        "_OK", GTK_RESPONSE_OK, "_Cancel", GTK_RESPONSE_CANCEL, NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_OK)
    {
        const gchar *foldername;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        foldername = gtk_file_chooser_get_filename(chooser);
        
        gtk_entry_set_text(GTK_ENTRY(data), foldername);
        gtk_editable_set_editable(GTK_EDITABLE(data), FALSE);
    }

    gtk_widget_destroy(dialog);
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
void show_resulting_image_in_new_window(GdkPixbuf *pixbuf, const gchar *src_filename)
{
    GtkWidget * res_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(res_window), "Filtering result");
    gtk_window_set_default_size(GTK_WINDOW(res_window), 700, 400);
    gtk_window_set_type_hint(GTK_WINDOW(res_window), GDK_WINDOW_TYPE_HINT_DIALOG);

    GtkWidget *image     = gtk_image_new();
    GtkWidget *image_box = gtk_layout_new(0, 0);
    GtkWidget *grid      = gtk_grid_new();

    GtkWidget *menu_bar          = gtk_menu_bar_new();
    GtkWidget *save_item         = gtk_menu_item_new_with_label("Save");
    GtkWidget *save_menu         = gtk_menu_new();
    GtkWidget *save_menu_item    = gtk_menu_item_new_with_label("Save");
    GtkWidget *save_as_menu_item = gtk_menu_item_new_with_label("Save as...");

    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), save_item);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(save_item), save_menu);
    gtk_menu_shell_append(GTK_MENU_SHELL(save_menu), save_menu_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(save_menu), save_as_menu_item);

    ImageFileData *data;
    data = (ImageFileData *)calloc(1, sizeof(ImageFileData));
    data->pixbuf = pixbuf;
    data->filename = src_filename;

    g_signal_connect(G_OBJECT(save_menu_item), "activate",
                     G_CALLBACK(on_save_menu_item_click), data);
    g_signal_connect(G_OBJECT(save_as_menu_item), "activate",
                     G_CALLBACK(on_save_as_menu_item_click), data);

    gtk_layout_put(GTK_LAYOUT(image_box), image, 0, 0);
    gtk_grid_attach(GTK_GRID(grid), menu_bar, 1, 1, 1, 1);

    gtk_grid_attach_next_to(GTK_GRID(grid), image_box, menu_bar,
                            GTK_POS_BOTTOM, 1, 1);

    gtk_container_add(GTK_CONTAINER(res_window), grid);
    setup_image_on_main_window(image_box, image, pixbuf);

    gtk_widget_show_all(res_window);
}

/**
 * Saves image in the same folder src image was opened from.
 **/
void on_save_menu_item_click(GtkWidget *caller
                            __attribute__((unused)), 
                            gpointer data)
{
    GdkPixbuf *res_pixbuf = ((ImageFileData *)data)->pixbuf;
    const gchar *filename = ((ImageFileData *)data)->filename;
    
    const gchar *appendix = "-(filtered).";
    gchar *extension = strrchr(filename, '.') + 1;
    
    // jpg is not supported but jpeg is supported
    // that's why i need this weird check
    gchar res_extension[strlen(extension + 1)];
    if (strcmp(extension, "jpg") == 0) 
    {
        strcpy(res_extension, "jpeg");
    }
    else
    {
        strcpy(res_extension, extension);
    }
    
    gchar res_filename[strlen(filename) + strlen(appendix) + strlen(res_extension)];
    strcpy(res_filename, filename);
    
    // strip extension
    gchar *filename_dot = strchr(res_filename, '.');
    gint offset = filename_dot - res_filename;
    res_filename[offset] = '\0';

    // add appendix and extension
    strcat(res_filename, appendix);
    strcat(res_filename, res_extension);

    gboolean saved = gdk_pixbuf_save(res_pixbuf, res_filename, res_extension,
                                    NULL, NULL);

    if (saved) g_print("Wrote new file: %s\n", res_filename);
}

/**
 * Saves image in chosen folder with chosen name.
 **/
void on_save_as_menu_item_click(GtkWidget *caller, 
                                gpointer data)
{
    GdkPixbuf *res_pixbuf = ((ImageFileData *)data)->pixbuf;
    const gchar *src_filename = ((ImageFileData *)data)->filename;
    const gchar *extension = strrchr(src_filename, '.') + 1;

    GtkWidget *window = gtk_widget_get_toplevel(caller);

    GtkWidget *dialog = gtk_file_chooser_dialog_new("Save as...",
                                                    GTK_WINDOW(window),
                                                    GTK_FILE_CHOOSER_ACTION_SAVE,
                                                    "_Save", GTK_RESPONSE_OK,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    NULL);
    
    gint res = gtk_dialog_run(GTK_DIALOG(dialog));

    if (res == GTK_RESPONSE_OK)
    {
        const gchar *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        // jpg is not supported by gdk_pixbuf_save but jpeg is supported
        // that's why i need this weird check
        gchar res_extension[strlen(extension + 1)];
        if (strcmp(extension, "jpg") == 0) 
        {
            strcpy(res_extension, "jpeg");
        }
        else
        {
            strcpy(res_extension, extension);
        }

        gchar res_filename[strlen(filename) + 1 + strlen(res_extension)];
        strcpy(res_filename, filename);
        strcat(res_filename, ".");
        strcat(res_filename, res_extension);

        gboolean saved = gdk_pixbuf_save(res_pixbuf, res_filename, res_extension,
                                        NULL, NULL);
        
        if (saved) g_print("Wrote new file: %s\n", res_filename);
    }

    gtk_widget_destroy(dialog);
}

/**
 * Shows quick message on screen (usually success message)
 **/
void quick_message (GtkWindow *parent, gchar *message)
{
    GtkWidget *dialog, *label, *content_area;
    GtkDialogFlags flags;

    flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    dialog = gtk_dialog_new_with_buttons ("Message",
                                        parent,
                                        flags,
                                        "_OK",
                                        GTK_RESPONSE_NONE,
                                        NULL);
    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
    label = gtk_label_new (message);

    g_signal_connect_swapped (dialog,
                            "response",
                            G_CALLBACK (gtk_widget_destroy),
                            dialog);

    gtk_container_add (GTK_CONTAINER (content_area), label);
    gtk_widget_show_all (dialog);
}

/**
 * Creates new filename from src_filename (adds '-(filtered)' to file name)
 **/
gchar *get_new_filename(const gchar *src_filename) {
    const gchar *appendix = "-(filtered).";
    gchar *extension = strrchr(src_filename, '.') + 1;

    gchar res_extension[strlen(extension + 1)];
    if (strcmp(extension, "jpg") == 0) 
    {
        strcpy(res_extension, "jpeg");
    }
    else
    {
        strcpy(res_extension, extension);
    }
    
    gchar *res_filename = g_new(gchar, strlen(src_filename) + strlen(appendix) + strlen(res_extension));
    strcpy(res_filename, src_filename);
    
    // strip extension
    gchar *filename_dot = strchr(res_filename, '.');
    gint offset = filename_dot - res_filename;
    res_filename[offset] = '\0';

    // add appendix and extension
    strcat(res_filename, appendix);
    strcat(res_filename, res_extension);

    return res_filename;
}


/**
 * Assignes a thread to a mailbox and loads them.
 **/
void parallel_folder_service(const gchar *src_folder, const gchar *out_folder, FilterData *filter_data)
{
    int numThreads = 3;
    
    mbox_t mailbox[numThreads];
    pthread_t threadID[numThreads];

    for(int i = 0; i < numThreads; i++) {
		msg_init(&mailbox[i]);
	}

    struct loading_thread_args * l_struct = &(struct loading_thread_args) {
        .src_folder = src_folder,
        .img_loader = &mailbox[0],
        .img_processor = &mailbox[1]
    };
    struct processing_thread_args * p_struct = &(struct processing_thread_args) {
        .filter_data = filter_data,
        .img_processor = &mailbox[1],
        .img_saver = &mailbox[2]
    };
    struct saving_thread_args * s_struct = &(struct saving_thread_args) {
        .out_folder = out_folder,
        .img_saver = &mailbox[2]
    };

    pthread_create(&threadID[0], NULL, img_loading, (void *)l_struct);
    pthread_create(&threadID[1], NULL, img_processing, (void *)p_struct);
    pthread_create(&threadID[2], NULL, img_saving, (void *)s_struct);

    for(int i = 0; i < numThreads; i++) {
		pthread_join(threadID[i],NULL);
		sem_destroy(&mailbox[i].NotFull);
		sem_destroy(&mailbox[i].NotEmpty);
	}
}


void *img_loading(void *ptr)
{
    struct loading_thread_args *args = (struct loading_thread_args *)ptr;
    const gchar *src_folder = args->src_folder;
    mbox_t *img_processor   = args->img_processor;

    int tid = pthread_self();

    DIR *dp;
    struct dirent *ep;
    gchar filename[200];
    GdkPixbuf *src_pixbuf;

    dp = opendir(src_folder);
    if (dp != NULL)
    {
        while((ep = readdir(dp)))
        {
            if (ep->d_type == DT_REG || ep->d_type == DT_UNKNOWN)
            {
                if (strcmp(".", ep->d_name) != 0 && strcmp("..", ep->d_name) != 0)
                {
                    gchar *extension = strrchr(ep->d_name, '.') + 1;
                    if (strcmp("bmp", extension) == 0 || 
                        strcmp("png", extension) == 0 ||
                        strcmp("jpg", extension) == 0)
                    {
                        strcpy(filename, src_folder);
                        strcat(filename, "/");
                        strcat(filename, ep->d_name);
                        src_pixbuf = gdk_pixbuf_new_from_file_at_scale(filename, -1, -1, TRUE, NULL);

                        msg_t img_msg = {tid, MSG_TYPE_IMG, filename, src_pixbuf};
                        msg_send(img_processor, &img_msg);
                    }
                }
            }
        }
        (void) closedir(dp);
        msg_t eos_msg = {tid, MSG_TYPE_EOS, NULL, NULL};
        msg_send(img_processor, &eos_msg);
    }
}

void *img_processing(void *ptr)
{
    struct processing_thread_args *args = (struct processing_thread_args *)ptr;
    FilterData *filter_data = args->filter_data;
    mbox_t *img_processor   = args->img_processor;
    mbox_t *img_saver       = args->img_saver;
    
    int tid = pthread_self();
    msg_t msg = {0, 0, NULL, NULL};

    while(1)
    {
        msg_receive(img_processor, &msg);

        if (msg.type == MSG_TYPE_EOS)
        {
            msg_t eos_msg = {tid, MSG_TYPE_EOS, NULL, NULL};
            msg_send(img_saver, &eos_msg);
            break;
        }

        if (msg.type == MSG_TYPE_IMG)
        {            
            GdkPixbuf *out_pixbuf = apply_filter_service(msg.pixbuf, filter_data);
            msg_t img_msg = {tid, MSG_TYPE_IMG, msg.filename, out_pixbuf};
            msg_send(img_saver, &img_msg);
        }

    }
}

void *img_saving(void *ptr)
{
    struct saving_thread_args *args = (struct saving_thread_args *)ptr;
    const gchar *out_folder = args->out_folder;
    mbox_t *img_saver       = args->img_saver;

    int tid = pthread_self();
    msg_t msg = {0, 0, NULL, NULL};

    while(1)
    {
        msg_receive(img_saver, &msg);

        if (msg.type == MSG_TYPE_EOS)
        {
            msg_t eos_msg = {tid, MSG_TYPE_EOS, NULL, NULL};
            msg_send(img_saver, &eos_msg);
            break;
        }

        if (msg.type == MSG_TYPE_IMG)
        {
            gchar filename[200];
            gchar *extension = strrchr(msg.filename, '.') + 1;
            strcpy(filename, out_folder);
            strcat(filename, "/");
            strcat(filename, msg.filename);
            gchar *new_filename = get_new_filename(filename);

            gboolean saved = gdk_pixbuf_save(msg.pixbuf, new_filename, extension, NULL, NULL);
            if (saved) g_print("Wrote new file: %s\n", new_filename);
        }
    }
}