#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <err.h>
#include <bits/signum.h>
#include <signal.h>

#define NUMSEMS 2

typedef struct
{
    int a, b;
    long result;
    int flg;
} mem_t;

mem_t* data;
key_t key;
int shmid, semid;
struct sembuf sb[2] = {{0,1,0}, {0,0,0}};
struct sembuf wait_client[1] = {{0,-1,0}};
struct sembuf calculated[1] = {{1,1,0}};

void handler(int sig)
{
    if (sig == SIGINT)
    {
        semctl(semid, NUMSEMS, IPC_RMID);
        semctl(shmid, sizeof(mem_t), IPC_RMID);
        exit(0);
    }
    signal(SIGINT, handler);
}
int main()
{
    key = ftok("\bin\ls",  '1');
    if ( (shmid = shmget(key,  sizeof(mem_t), IPC_CREAT | IPC_EXCL | 0600)) == -1)
        err(20, "shmget");
    if ( (semid = semget(key, NUMSEMS, IPC_CREAT | IPC_EXCL | 0600)) == -1)
        err(21, "semget");

    data = (mem_t*)shmat(shmid, NULL, 0);
    if (data == (mem_t*)(-1))
    {
        err(22, "shmat");
    }

    signal(SIGINT, handler);
    semop(semid, sb, 1);

    while(1)
    {
        semop(semid, wait_client, 1);
        data->result = data->a + data->b;
        semop(semid, calculated, 1);
    }

    return 0;
}


