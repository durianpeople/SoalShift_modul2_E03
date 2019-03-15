#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

int program();

int main()
{
    return program();
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

    if ((chdir("/home/durianpeople/log/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        int tmp = program();
        sleep(30);
    }

    exit(EXIT_SUCCESS);
}

int program() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int dd = tm.tm_mday;
    int MM = tm.tm_mon + 1;
    int yyyy = tm.tm_year + 1900;
    int hh = tm.tm_hour;
    int mm = tm.tm_min;
    char foldername[1000] = "";
    struct stat st = {0};
    if (stat("gambar", &st) == -1)
    {
        
        mkdir("gambar", 0777);
    }
    
    return 0;
}