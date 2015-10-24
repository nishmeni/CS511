#ifndef _MONITOR_
#define _MONITOR_

typedef struct s_glenv {
    pthread_mutex_t lock;       /* lock semaphore for controlling access to the intersection */
    pthread_cond_t north_go;    /*  */
    pthread_cond_t south_go;    /*  */
    pthread_cond_t east_go;     /*  */
    pthread_cond_t west_go;     /*  */
} t_glenv;

t_glenv env;

void monitor_init();
void monitor_arrive(cart_t *cart);
void monitor_cross(cart_t *cart);
void monitor_leave(cart_t *cart);
void monitor_shutdown();

#endif
