#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

int program();

int main()
{
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

    if ((chdir("/home/durianpeople/Documents/Notes/SISOP/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1)
    {
        int tmp = program();
        sleep(3);
    }

    exit(EXIT_SUCCESS);
}

int program()
{
    struct stat info;
    stat("hatiku/elen.ku", &info);
    struct passwd *pw = getpwuid(info.st_uid);
    struct group *gr = getgrgid(info.st_gid);
    printf("Checking user\n");
    if (pw != 0 && gr != 0)
    {
        printf("User: %s\nGroup: %s\n", pw->pw_name, gr->gr_name);
        if (strcmp(pw->pw_name, "www-data") == 0 && strcmp(gr->gr_name, "www-data") == 0)
        {
            printf("User matches\n");
            //https://stackoverflow.com/questions/4568681/using-chmod-in-a-c-program
            char mode_raw[] = "0777";
            int mode = strtol(mode_raw, 0, 8);
            if (!(chmod("hatiku/elen.ku", mode) < 0))
            {
                printf("Erasing file\n");
                remove("hatiku/elen.ku");
            }
            else
                printf("chmod failed\n");
        }
        else
            printf("User not matches\n");
    }
    return 0;
}