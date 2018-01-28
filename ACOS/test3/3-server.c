#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <memory.h>
#include "../common/utils.h"
#include "common_types.h"

key_t key;
int msgid;
mes_t data;

void handler(int sig)
{
    if (sig == SIGINT)
    {
        msgctl(msgid, IPC_RMID, NULL);
        exit(0);
    }
    signal(SIGINT, handler);
}

char buf[BUF_SIZE];

int main()
{
    key = ftok("/bin/ls", '1');
    msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0600);

    if (msgid == -1)
    {
        printf("msgget failed\n");
        return 1;
    }

    signal(SIGINT, handler);

    memset(buf, 0, BUF_SIZE);
    while ((fgets(data.buf, BUF_SIZE, stdin) != NULL))
    {
        printf("in process %s, client = %d\n", data.buf, (int)data.client);
        fflush(stdout);
        data.mes_type = RESPONSE + (long)data.client;
        msgsnd(msgid, &data, sizeof(mes_t), 0);
        memset(buf, 0, BUF_SIZE);
    }

    return 0;
}
