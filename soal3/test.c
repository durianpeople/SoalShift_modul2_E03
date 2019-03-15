#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#define MSGSIZE 1000

int main() {
  pid_t child_id, child_id1, child_id2;
  int status;
  int p[2];
  pipe(p);
  int record;
  child_id = fork();
  if (child_id == 0) {
    char *argv[3] = {"unzip", "/home/akmalfakhrib/Documents/sisop/SoalShift_modul2_E03-back/campur2.zip", NULL};
  	execv("/usr/bin/unzip", argv);
    close(p[0]);
  } else {
    while ((wait(&status)) > 0);
    DIR *dr = opendir("/home/akmalfakhrib/Documents/sisop/SoalShift_modul2_E03-back/campur2");
        if (dr == NULL)
        {
            // printf("Gagal buka direktori\n");
            break;
        }
        while ((de = readdir(dr)) != NULL)
        {
            child_id = fork();
            if (child_id == 0)
            {
                // child
                char *ext = strstr(de->d_name, ".txt");
                int stringlength = strlen(de->d_name);
                int pointer = (ext - de->d_name);
                int trueext = (strcmp(ext, ".txt")==0);
                if (trueext)
                {
                    // de->d_name
                    
                }
                exit(0);
            }
            else
            {
                // parent
                while (wait(&status) > 0);
                continue;
            }
        }
        closedir(dr);
    }
  }
