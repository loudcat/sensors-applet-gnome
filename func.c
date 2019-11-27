#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <regex.h>


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
    //printf("con = %s\n", res);
    return res;
}

void find_dir(const char *path){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path)) != NULL){
        while ((ent = readdir(dir)) != NULL)
        {
            if (match(ent->d_name, "^name$") != 0){
                printf("%s\n", con(path, ent->d_name));
            }
            
            // printf("%d\n", ent->d_type);
            if (ent->d_type == 10 & (match(ent->d_name, "^hwmon.$") != 0)){
                //const char *new_p = con(path, ent->d_name);
                printf("%s\n", ent->d_name);
                find_dir(con(path, ent->d_name));
            }

        }
        
    }
    
}