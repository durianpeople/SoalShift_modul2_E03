#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int main() {
    pid_t child_id;
    struct dirent *de;
    int status;
    char *ptr, *before;
    DIR *dr=opendir(".");
    if (dr==NULL) {
        printf("Gagal buka direktori\n");
        return 0;
    }
    while ((de=readdir(dr))!=NULL) {
        
        child_id=fork();
        if (child_id==0) {
            //child
            char* ext=strstr(de->d_name, ".png");
            int stringlength=strlen(de->d_name);
            int pointer=(ext-de->d_name);
            int trueext=(stringlength-pointer==4);
            if (trueext) {
                printf("True Ext\n");
                char basename[pointer+1];
                memcpy(basename, &de->d_name[0], pointer );
                basename[pointer] = '\0';
                //https://stackoverflow.com/questions/7430248/creating-a-new-directory-in-c
                struct stat st = {0};
                if (stat("gambar", &st) == -1) {
                    mkdir("gambar", 0777);
                }
                char target[1000]="./gambar/";
                strcat(target, basename);
                strcat(target, "_grey.png");
                // char* target=strcat(basename, "_grey.png");
                printf("Execv %s to %s\n", de->d_name, target);
                rename(de->d_name, target);
            }
            exit(0);
        }
        else {
            //parent
            while(wait(&status)>0);
            continue;
        }
    }
    closedir(dr);
    return 0;
}