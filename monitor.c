/*
 *
 */
void monitor_init() {
    /* initialize mutex and condition vars */
    if (pthread_mutex_init(&env.lock, NULL) != 0) {
        perror("sem_init lock");
        exit(-1);
    }
    if (pthread_cond_init(&env.north_go, NULL) != 0) {
        perror("sem_init data_avail");
        exit(-1);
    }
    if (pthread_cond_init(&env.south_go, NULL) != 0) {
        perror("sem_init space_avail");
        exit(-1);
    }
    if (pthread_cond_init(&env.west_go, NULL) != 0) {
        perror("sem_init space_avail");
        exit(-1);
    }
    if (pthread_cond_init(&env.east_go, NULL) != 0) {
        perror("sem_init space_avail");
        exit(-1);
    }
}

void monitor_arrive(cart_t *cart) {
    /* wait for mutex lock */
    if (pthread_mutex_lock(&env.lock) != 0) {
        perror("pthread_mutex_lock");
        ret = -1;
        break;
    }

    switch (cart.dir) {
        case 'n':
            /* wait on data_avail condition variable if there is no data available */
            if (!cbuf_data_is_available()) {
                if (pthread_cond_wait(&env.north_go, &env.lock) != 0) {
                    perror("pthread_cond_wait drain_thread");
                    ret = -1;
                    break;
                }
            }
            break;
        case 's':
            /* wait on data_avail condition variable if there is no data available */
            if (!cbuf_data_is_available()) {
                if (pthread_cond_wait(&env.south_go, &env.lock) != 0) {
                    perror("pthread_cond_wait drain_thread");
                    ret = -1;
                    break;
                }
            }
            break;
        case 'e':
            /* wait on data_avail condition variable if there is no data available */
            if (!cbuf_data_is_available()) {
                if (pthread_cond_wait(&env.east_go, &env.lock) != 0) {
                    perror("pthread_cond_wait drain_thread");
                    ret = -1;
                    break;
                }
            }
            break;
        case 'w':
            /* wait on data_avail condition variable if there is no data available */
            if (!cbuf_data_is_available()) {
                if (pthread_cond_wait(&env.west_go, &env.lock) != 0) {
                    perror("pthread_cond_wait drain_thread");
                    ret = -1;
                    break;
                }
            }
            break;

    }

}

void monitor_cross(cart_t *cart) {

}

void monitor_leave(cart_t *cart) {

}

void monitor_shutdown() {
    /* destroy mutex and condition vars once threads are finished */
    if (pthread_mutex_destroy(&env.lock) != 0) {
        perror("sem_destroy lock");
        exit(-1);
    }
    if (pthread_cond_destroy(&env.north_go) != 0) {
        perror("sem_destroy data_avail");
        exit(-1);
    }
    if (pthread_cond_destroy(&env.south_go) != 0) {
        perror("sem_destroy space_avail");
        exit(-1);
    }
    if (pthread_cond_destroy(&env.west_go) != 0) {
        perror("sem_destroy space_avail");
        exit(-1);
    }
    if (pthread_cond_destroy(&env.east_go) != 0) {
        perror("sem_destroy space_avail");
        exit(-1);
    }
}
