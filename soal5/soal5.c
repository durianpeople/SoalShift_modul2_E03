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
#include <signal.h>

int program();

char *itoa(int num, char *str)
{
    if (str == NULL)
    {
        return NULL;
    }
    sprintf(str, "%d", num);
    return str;
}

int main()
{
    if ((chdir("/home/durianpeople/log/")) < 0)
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

    FILE *piddaemon = fopen("pid", "w+");
    fprintf(piddaemon, "%d", getpid());
    fclose(piddaemon);
    while (1)
    {
        printf("Iterate\n");
        int tmp = program();
        sleep(0);
    }
    exit(EXIT_SUCCESS);
}

int program()
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char foldername[1000];
    strftime(foldername, 1000, "%d:%m:Y-%H:%M", &tm);
    struct stat st = {0};
    if (stat(foldername, &st) == -1)
    {
        mkdir(foldername, 0777);
    }
    int counter = 0;
    while (counter++ < 30)
    {
        char counter_string[1000];
        itoa(counter, counter_string);
        FILE *source = fopen("/var/log/syslog", "r");
        if (source == NULL)
            printf("Gagal 1\n"), exit(0);
        char filename[1000] = "/home/durianpeople/log/";
        strcat(filename, foldername);
        strcat(filename, "/log");
        strcat(filename, counter_string);
        strcat(filename, ".log");
        printf("%s\n", filename);
        FILE *dest = fopen(filename, "a+");
        if (dest == NULL)
            printf("Gagal 2\n"), exit(0);
        char pointer;
        pointer = fgetc(source);
        while (pointer != EOF)
        {
            fputc(pointer, dest);
            pointer = fgetc(source);
        }
        fclose(source);
        fclose(dest);
        sleep(60);
    }
    return 0;
}