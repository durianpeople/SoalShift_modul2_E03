#include <dirent.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t child_id;
    struct dirent *de;
    int status;
    DIR *dr=opendir(".");
    if (dr==NULL) {
        printf("Gagal buka direktori\n");
        return 0;
    }
    while ((de=readdir(dr))!=NULL) {
        // printf("%s\n", de->d_name);
        child_id=fork();
        if (child_id==0) {
            //child
            char *argv[4] = {"mv", de->d_name, "sample-dir", NULL};
            execv("/bin/mkdir", argv);
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