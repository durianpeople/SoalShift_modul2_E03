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
    child_id1 = fork();
    if (child_id1 == 0){
      dup2(p[1],1);
      close(p[0]);
      char *argv[3] = {"ls", "/home/akmalfakhrib/Documents/sisop/SoalShift_modul2_E03-back/campur2", NULL};
      execv("/bin/ls", argv);
    } else {
      while ((wait(&status)) > 0);
      child_id2 = fork();
      if (child_id2 == 0){
        char *argv[3] = {"touch", "/home/akmalfakhrib/Documents/sisop/SoalShift_modul2_E03-back/text.txt", NULL};
        execv("/usr/bin/touch", argv);
      } else {
            while ((wait(&status)) > 0);
            record = open("text.txt",O_WRONLY);
            dup2(p[0],0);
            close(p[1]);
            dup2(record,1);
            char *argv[3] = {"grep", "[.]txt$", NULL};
            execv("/bin/grep", argv);
            close(p[0]);
      }
    }
  }
}