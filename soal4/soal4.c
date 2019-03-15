#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int program();

int counter = 1;
int main()
{
    if ((chdir("/home/durianpeople/Documents/makanan")) < 0)
    {
        exit(EXIT_FAILURE);
    }
    // return program();
    pid_t pid, sid;

    pid = fork();

    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();

    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        int tmp = program();
        sleep(5);
    }

    exit(EXIT_SUCCESS);
}

int program()
{
    struct stat filestat;
    stat("makan_enak.txt", &filestat);

    time_t current_time = time(NULL);
    time_t diff = difftime(current_time, filestat.st_atime);
    int diff_int = (int)diff;
    printf("%d\n", diff_int);
    if (diff_int <= 30)
    {
        char target[1000];
        sprintf(target, "makan_sehat%d.txt", counter++);
        FILE *ftarget = fopen(target, "w+");
        fclose(ftarget);
    }

    return 0;
}