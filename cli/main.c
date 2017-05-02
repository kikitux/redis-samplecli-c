// https://github.com/redis/hiredis/blob/master/examples/example.c

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <hiredis.h>

int main(int argc, char **argv) {
    unsigned int j;
    redisContext *c;
    redisReply *reply;
    const char *hostname = (argc > 1) ? argv[1] : "192.168.56.11";
    int port = (argc > 2) ? atoi(argv[2]) : 6379;

    struct timeval timeout = { 0, 500000 }; // 0.5 seconds
    c = redisConnectWithTimeout(hostname, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

    /* PING server */
    reply = redisCommand(c,"PING");
    printf("PING: %s\n", reply->str);
    freeReplyObject(reply);

    /* Set a key */
    reply = redisCommand(c,"SET %s %s", "foo", "hello world");
    printf("SET: %s\n", reply->str);
    freeReplyObject(reply);

    /* Try a GET */
    reply = redisCommand(c,"GET foo");
    printf("GET foo: %s\n", reply->str);
    freeReplyObject(reply);

    /* Create a list of numbers, from 0 to 4 */
    reply = redisCommand(c,"DEL mylist");
    freeReplyObject(reply);

    // lets use OpenMP for Multi Thread
    int nthreads, tid;

    omp_lock_t writelock;

    omp_init_lock(&writelock);

    /* Fork a team of threads giving them their own copies of variables */
    #pragma omp parallel private(nthreads, tid)
    {

        /* Obtain thread number */
        tid = omp_get_thread_num();

        /* set lock */
        omp_set_lock(&writelock);
        printf("Hello World from thread = %d\n", tid);

        /* PING server */
        reply = redisCommand(c,"PING");
        printf("PING: %s\n", reply->str);
        freeReplyObject(reply);

        /* unset lock */
        omp_unset_lock(&writelock);

        #pragma omp parallel for
        for (j = 0; j < 5; j++) {
            char buf[64];

            snprintf(buf,64,"t%u-%u",tid,4-j);
            /* set lock */
            omp_set_lock(&writelock);

            reply = redisCommand(c,"LPUSH mylist element-%s", buf);
            freeReplyObject(reply);

            /* increment */
            reply = redisCommand(c,"INCR counter");
            printf("INCR counter: %lld from thread: %d\n", reply->integer,tid);
            freeReplyObject(reply);

            /* unset lock */
            omp_unset_lock(&writelock);
        }

        /* Only master thread does this */
        if (tid == 0)
        {
            nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }

    }  /* All threads join master thread and disband */

    /* destroy the lock */
    omp_destroy_lock(&writelock);

    /* Let's check what we have inside the list */
    reply = redisCommand(c,"LRANGE mylist 0 -1");
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (j = 0; j < reply->elements; j++) {
            printf("%u) %s\n", j, reply->element[j]->str);
        }
    }
    freeReplyObject(reply);

    /* Disconnects and frees the context */
    redisFree(c);

    return 0;
}
