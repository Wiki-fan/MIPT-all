/*
 * client
 */
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <err.h>
#include <signal.h>

#define NUMSEMS 4

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
    if (sig == SIGUSR1)
    {
        semctl(semid, NUMSEMS, IPC_RMID);
        semctl(shmid, sizeof(mem_t), IPC_RMID);
        exit(0);
    }
    signal(SIGUSR1, handler);
}
int main()
{
    int a, b;
    long result;
    key = ftok("\bin\ls",  '1');
    if ( (shmid = shmget(key,  sizeof(mem_t), 0)) == -1)
        err(20, "shmget");
    if ( (semid = semget(key, NUMSEMS, 0)) == -1)
        err(21, "semget");

    data = (mem_t*)shmat(shmid, NULL, 0);
    if (data == (mem_t*)(-1))
    {
        err(22, "shmat");
    }

    semop(semid, sb, 1);

    while(!feof(stdin))
    {
        printf( "Input >:\n" );
        scanf( "%d %d", &a, &b );
        data->a = a;
        data->b = b;
        semop( semid, wakeup_server, 1 );
        semop( semid, wait_server, 1 );

        printf( "Result >: %ld\n", result );
    }
    shmdt(data);

    return 0;
}
