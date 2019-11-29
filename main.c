#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <regex.h>

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

int main(void)
{

    printf("Hello\n");
    node_t *head = find_dir("/sys/class/hwmon");

    for (int i = 0; i<=10; i++){
        print_list(head);
    }
    return 0;
}