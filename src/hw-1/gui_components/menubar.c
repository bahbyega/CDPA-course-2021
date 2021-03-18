#include "menubar.h"

void setup_menu_bar_on_main_window(GtkWidget *menu_bar)
{
    GtkWidget *file_item = gtk_menu_item_new_with_label("File");
    GtkWidget *help_item = gtk_menu_item_new_with_label("Help");

    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), help_item);

    GtkWidget *file_menu = gtk_menu_new();
    GtkWidget *help_menu = gtk_menu_new();

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_item), help_menu);

    GtkWidget *open_item = gtk_menu_item_new_with_label("Open");
    GtkWidget *exit_item = gtk_menu_item_new_with_label("Exit");

    GtkWidget *github_item = gtk_menu_item_new_with_label("Github");

    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), open_item);
    gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), exit_item);

    gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), github_item);

    g_signal_connect(G_OBJECT(open_item), "activate",
                     G_CALLBACK(on_open_menu_item_click), menu_bar);

    g_signal_connect(G_OBJECT(exit_item), "activate",
                     G_CALLBACK(gtk_main_quit), NULL);
    
    g_signal_connect(G_OBJECT(github_item), "activate",
                     G_CALLBACK(open_github_page), NULL);

}

void open_github_page()
{
    // just calling the shell to open a link
    system("xdg-open https://github.com/bahbyega/CDPA-course-2021");
}

void on_open_menu_item_click(GtkWidget *caller __attribute__((unused)), gpointer menu_bar)
{
    // i have to pass a pointer to GtkWidget as a first argument
    // in this event handler function, and to remove compiler warning
    // i added 'unused' gcc's attribute in the signature

    on_init_btn_click(menu_bar); // call initial loading event handler
}
