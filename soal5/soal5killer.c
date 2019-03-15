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

int main()
{
    if ((chdir("/home/durianpeople/log/")) < 0)
    {
        exit(EXIT_FAILURE);
    }
    FILE *piddaemon = fopen("pid", "r");
    char pids[100];
    fgets(pids, 100, piddaemon);
    fclose(piddaemon);
    int pid = atoi(pids);
    kill(pid, SIGKILL);
    return 0;
}