#define _POSIX_C_SOURCE 200901L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "../common/utils.h"
#include "../task-s2-gets_safe/safe_string.h"

#define BACKLOG 5
#define THREADS 3

typedef struct
{
    char* name;
    char* key;
} Info;

Info* infos;
volatile int size;
volatile int cursize;

struct sthread
{
    int free;
    int fd;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

volatile int numconnected;
struct sthread arr[THREADS];
pthread_t arrthread[THREADS];

pthread_mutex_t infos_mutex, numconnected_mutex;

#define KEYOK "key ok"
#define KEYNO "key error"

void get_all(char* buf, Info* info)
{
    char* iter1;
    /*sscanf(buf, "%s:%s\n", infos[cursize].name, infos[cursize].key);*/
    info->name = iter1 = buf;
    while ( (*iter1 != ':'))
    {
        iter1++;
    }
    *iter1 = '\0';
    ++iter1;
    info->key = iter1;
    while ( (*iter1 != '\n'))
    {
        iter1++;
    }
    *iter1 = '\0';
    ++cursize;
    if (cursize >= size)
    {
        size *= 2;
        infos = realloc_s(infos, size);
    }
}

void read_file(char* filename)
{
    FILE* inf;
    char* buf;
    size = 10;
    if ((inf = fopen(filename, "r")) == NULL)
    {
        err(6, "fopen");
    }
    infos = malloc_s(sizeof(Info)*size);
    while ( ( gets_safe(inf, &buf)) != 0)
    {
        get_all(buf, &infos[cursize]);
    }
    size = cursize - 1;
}

int lookup(char* name, char* key)
{
    int i;
    for (i = 0; i<size; ++i)
    {
        if ( (!strcmp(name, infos[i].name)) && (!strcmp(key, infos[i].key)) )
        {
            return 1;
        }
        if ( (!strcmp(name, infos[i].name)) && !(!strcmp(key, infos[i].key)) )
        {
            return 0;
        }
    }
    return 0;
}

/* 0 - busy, 1 - free, 2 - accepting */
void* fthread(void* arg)
{
    struct sthread* ptr = (struct sthread*) arg;
    char buf[1000];
    Info info;
    int num;
    while (1)
    {
        pthread_mutex_lock(&(ptr->mutex));
        pthread_cond_wait(&(ptr->cond), &(ptr->mutex));
        ptr->free = 0;
        pthread_mutex_unlock(&(ptr->mutex));

        printf("Lookup started");
        pthread_mutex_lock(&infos_mutex);
        CHN1(num = read(ptr->fd, buf, 1000), 12, "read");
        get_all(buf, &info);
        if (lookup(info.name, info.key))
        {
            CHN1( write( ptr->fd, KEYOK, strlen( KEYOK ) + 1 ), 13, "write" );
        }
        else
        {
            CHN1(write(ptr->fd, KEYNO, strlen(KEYNO) + 1), 13, "write");
        }
        close(ptr->fd);
        pthread_mutex_unlock(&infos_mutex);

        pthread_mutex_lock(&(ptr->mutex));
        pthread_mutex_lock(&(numconnected_mutex));

        --numconnected;
        ptr->free = 1;

        pthread_mutex_unlock(&(numconnected_mutex));
        pthread_mutex_unlock(&(ptr->mutex));
    }
    return NULL;
}

#define OVERLOAD "SERVER PEREGRUZHEN!!!1"

int main(int argc, char* argv[])
{
    int i;
    struct sockaddr_in serv_addr;
    int sock_id;
    int port_num;
    char* filename;
    if (argc != 3)
    {
        errx(5, "Wrong number of parameters");
    }
    port_num = atoi(argv[1]);
    filename = argv[2];

    read_file(filename);

    CHN1(sock_id = socket( AF_INET, SOCK_STREAM, 0 ), 1, "socket");

    /* initialize socket structure */
    memset( &serv_addr, 0, sizeof( serv_addr ));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons((uint16_t) port_num );

    /* bind the socket*/
    CHN1( bind( sock_id, (struct sockaddr*) &serv_addr, sizeof( serv_addr )), 3, "Bind" );

    /* listen socket */
    CHN1( listen( sock_id, BACKLOG ), 4, "Listen" );

    for(i = 0; i < THREADS; ++i)
    {
        pthread_mutex_init(&(arr[i].mutex), NULL);
        pthread_cond_init(&(arr[i].cond), NULL);
    }
    pthread_mutex_init(&infos_mutex, NULL);

    for(i = 0; i < THREADS; ++i)
    {
        arr[i].free = 1;
        pthread_create(&arrthread[i], NULL, fthread, &arr[i]);
    }

    pthread_mutex_init(&numconnected_mutex, NULL);

    while (1)
    {
        int all_not_free;
        int acc_ret;
        CHN1( acc_ret = accept(sock_id, NULL, NULL), 11, "accept");
        printf("%d accepted\n", acc_ret);

        pthread_mutex_lock(&numconnected_mutex);
        all_not_free = (numconnected == THREADS);
        pthread_mutex_unlock(&numconnected_mutex);

        if (all_not_free)
        {
            write(acc_ret, OVERLOAD, strlen(OVERLOAD) + 1);
            close(acc_ret); /* ?? */
            continue;
        }

        pthread_mutex_lock(&numconnected_mutex);
        ++numconnected;
        pthread_mutex_unlock(&numconnected_mutex);

        for (i = 0; i < THREADS; ++i)
        {
            pthread_mutex_lock(&(arr[i].mutex));
            if (arr[i].free == 1)
            {
                printf("Giving work to thread %d\n", i);
                arr[i].free = 2;
                arr[i].fd = acc_ret;
                pthread_cond_signal(&arr[i].cond);
                pthread_mutex_unlock(&(arr[i].mutex));
                break;
            }
            pthread_mutex_unlock(&(arr[i].mutex));
        }
    }

    return 0;
}