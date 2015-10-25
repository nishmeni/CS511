#ifndef _MONITOR_
#define _MONITOR_

#include "q.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct s_glenv {
    pthread_mutex_t lock;       /* lock semaphore for controlling access to the intersection */
    pthread_cond_t north_go;    /* cond var to signal the north direction thread to go */
    pthread_cond_t south_go;    /* cond var to signal the south direction thread to go */
    pthread_cond_t east_go;     /* cond var to signal the east direction thread to go */
    pthread_cond_t west_go;     /* cond var to signal the west direction thread to go */
    char nextdir;               /* holds the next direction to be signaled after a cart leaves the intersection */
} t_glenv;

t_glenv env;

void monitor_init();
void monitor_arrive(struct cart_t *cart);
void monitor_cross(struct cart_t *cart);
void monitor_leave(struct cart_t *cart);
void monitor_shutdown();

#endif
