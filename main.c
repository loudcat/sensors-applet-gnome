#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <regex.h>
#include <unistd.h>
#include <gtk/gtk.h>

typedef struct node
{
    char *path;
    struct node *next;
    
} node_t;

void print_list(node_t * head) {
    node_t * current = head;

    while (current != NULL) {
        printf("Value: %s\n", current->path);
        printf("Address: %p\n", (void *)current->next);
        current = current->next;
    }
}

void append(node_t *head, char *path){

    node_t * current = head;
    if (current->path == 0){
        // if first element is then fill
        current->path = path;
        current->next = NULL;
    } else {
        // now we can add a new variable
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = malloc(sizeof(node_t));
        current->next->path = path;
        current->next->next = NULL;
    }
    
}

int match(const char *string, const char *pattern){
    /*
        const char* s1 = "abc";
        const char* s2 = "123";
        const char* re = "[1-9]+";
        printf("%s matches %s? %s\n", s1, re, match(s1, re) ? "true" : "false");
        printf("%s matches %s? %s\n", s2, re, match(s2, re) ? "true" : "false");
    */
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return 0;
    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);
    if (status != 0) return 0;
    return 1;
}

char *con(const char *s1, char *s2){
    // for glue string
    char *res = malloc(strlen(s1)+strlen(s2)+2);
    strcpy(res, s1);
    strcat(res, "/");
    strcat(res, s2);
    return res;
}

node_t *find_dir(const char *path){
    // search all dirs of devices
    node_t *head = malloc(sizeof(node_t));
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path)) != NULL){
        while ((ent = readdir(dir)) != NULL)
        {   
            if (ent->d_type == 10 & (match(ent->d_name, "^hwmon[0-9]+$") != 0)){
                append(head, con(path, ent->d_name));
            }
        }
    }
    return head;
}

// Gtk
gboolean Loop (gpointer data){
    GtkWidget *label = GTK_WIDGET(data);
    gtk_label_set_text(GTK_LABEL(label), "aaaaa");

    return TRUE;
}


static void activate(GtkApplication *app, gpointer user_data){
    GtkWidget *window;
    
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Window");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_widget_show_all(window);
    GtkWidget *label = gtk_label_new ("<big>This is a long text that might need to be wrapped</big>");
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);
    g_object_set(label, "margin", 20, NULL);
    gtk_container_add(GTK_CONTAINER(window), label);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_max_width_chars(GTK_LABEL(label), 30);
    gtk_widget_show_now(label);
    g_timeout_add_seconds(1,Loop,label);    


}


int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;
    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    // printf("Hello\n");
    // node_t *head = find_dir("/sys/class/hwmon");

    // while (1){
    //     node_t * current = head;

    //     while (current != NULL) {
    //         char name_from_file[30];
    //         int temp_from_file;
    //         FILE *name, *temp;
    //         if((name = fopen(con(current->path, "name"), "r")) != NULL){
    //             fscanf(name, "%s", name_from_file);
    //             fclose(name);
    //         }
    //         else {
    //             return 1;
    //         }
    //         if((temp = fopen(con(current->path, "temp1_input"), "r")) != NULL){
    //             fscanf(temp, "%d", &temp_from_file);
    //             fclose(temp);
    //         }
    //         else {
    //             printf("error read form file (%s)\n", con(current->path, "/temp1_input"));
    //             temp_from_file = 0;
    //         }
    //         printf("%s: %d\n", name_from_file ,temp_from_file);
    //         //printf("Value: %s\n", current->path);
    //         current = current->next;
    //     }
    //     sleep(3);
    // }
    return 0;
}