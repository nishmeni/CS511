#include "monitor.h"

/*
 *  Initializes mutex and condition vars for signaling each direction.
 */
void monitor_init() {
    if (pthread_mutex_init(&env.lock, NULL) != 0) {
        perror("pthread_mutex_init");
        exit(-1);
    }
    if (pthread_cond_init(&env.north_go, NULL) != 0) {
        perror("pthread_cond_init north_go");
        exit(-1);
    }
    if (pthread_cond_init(&env.south_go, NULL) != 0) {
        perror("pthread_cond_init south_go");
        exit(-1);
    }
    if (pthread_cond_init(&env.west_go, NULL) != 0) {
        perror("pthread_cond_init west_go");
        exit(-1);
    }
    if (pthread_cond_init(&env.east_go, NULL) != 0) {
        perror("pthread_cond_init east_go");
        exit(-1);
    }

    env.nextdir = 'z';
}

/*
 *  Controls when arriving carts should enter the intersection or westCartIsWaiting
 *
 *  cart - cart arriving at the intersection
 */
void monitor_arrive(struct cart_t *cart) {

    printf("\nCart %i: arrived from direction %c\n", cart->num, cart->dir);

    /* wait for mutex lock */
    if (pthread_mutex_lock(&env.lock) != 0) {
        perror("pthread_mutex_lock");
        exit(EXIT_FAILURE);
    }

    /* set nextdir to whatever the next direction to go in is */
    if (env.nextdir == 'z') {
        env.nextdir = cart->dir;
    }

    /* wait threads on their respective "go" cond vars while their direction is not the next one to go */
    switch (cart->dir) {
        case 'n':

            while (env.nextdir != 'n') {
                printf("\nCart %i: waiting for go signal in %c direction\n", cart->num, cart->dir);

                if (pthread_cond_wait(&env.north_go, &env.lock) != 0) {
                    perror("pthread_cond_wait north_go");
                    exit(EXIT_FAILURE);
                }
            }
            break;

        case 's':

            while (env.nextdir != 's') {
                printf("\nCart %i: waiting for go signal in %c direction\n", cart->num, cart->dir);

                if (pthread_cond_wait(&env.south_go, &env.lock) != 0) {
                    perror("pthread_cond_wait south_go");
                    exit(EXIT_FAILURE);
                }
            }
            break;

        case 'e':

            while (env.nextdir != 'e') {
                printf("\nCart %i: waiting for go signal in %c direction\n", cart->num, cart->dir);

                if (pthread_cond_wait(&env.east_go, &env.lock) != 0) {
                    perror("pthread_cond_wait east_go");
                    exit(EXIT_FAILURE);
                }
            }
            break;
        case 'w':

            while (env.nextdir != 'w') {
                printf("\nCart %i: waiting for go signal in %c direction\n", cart->num, cart->dir);

                if (pthread_cond_wait(&env.west_go, &env.lock) != 0) {
                    perror("pthread_cond_wait west_go");
                    exit(EXIT_FAILURE);
                }
            }
            break;
    }

}

/*
 *  Makes the given cart cross the intersection
 *
 *  cart - cart that is to cross the intersection
 */
void monitor_cross(struct cart_t *cart) {

    printf("\n\t<< Cart %i entering intersection from direction %c... >>\n", cart->num, cart->dir);
    q_cartHasEntered(cart->dir);
    sleep(10);
    printf("\n\t<< Cart %i crossed intersection from direction %c >>\n", cart->num, cart->dir);

}

/*
 *  Controls which "go" cond var to signal next after the given cart leaves the intersection
 *
 *  cart - cart that is leaving the intersection
 */
void monitor_leave(struct cart_t *cart) {

    printf("\n\t<< ...Cart %i: leaving intersection from direction %c >>\n", cart->num, cart->dir);

    /* signal next cond var based on the direction of cart and the current value on nextdir */
    switch (cart->dir) {

        case 'n':
            if (q_cartIsWaiting('w')) {

                env.nextdir = 'w';
                pthread_cond_signal(&env.west_go);

            } else if (q_cartIsWaiting('s')) {

                env.nextdir = 's';
                pthread_cond_signal(&env.south_go);

            } else if (q_cartIsWaiting('e')) {

                env.nextdir = 'e';
                pthread_cond_signal(&env.east_go);

            } else if (q_cartIsWaiting('n')) {

                env.nextdir = 'n';
                pthread_cond_signal(&env.north_go);

            }
            break;

        case 's':
            if (q_cartIsWaiting('e')) {

                env.nextdir = 'e';
                pthread_cond_signal(&env.east_go);

            } else if (q_cartIsWaiting('n')) {

                env.nextdir = 'n';
                pthread_cond_signal(&env.north_go);

            } else if (q_cartIsWaiting('w')) {

                env.nextdir = 'w';
                pthread_cond_signal(&env.west_go);

            } else if (q_cartIsWaiting('s')) {

                env.nextdir = 's';
                pthread_cond_signal(&env.south_go);

            }
            break;

        case 'e':
            if (q_cartIsWaiting('n')) {

                env.nextdir = 'n';
                pthread_cond_signal(&env.north_go);

            } else if(q_cartIsWaiting('w')) {

                env.nextdir = 'w';
                pthread_cond_signal(&env.west_go);

            } else if(q_cartIsWaiting('s')) {

                env.nextdir = 's';
                pthread_cond_signal(&env.south_go);

            } else if(q_cartIsWaiting('e')) {

                env.nextdir = 'e';
                pthread_cond_signal(&env.east_go);

            }
            break;

        case 'w':
            if (q_cartIsWaiting('s')) {

                env.nextdir = 's';
                pthread_cond_signal(&env.south_go);

            } else if (q_cartIsWaiting('e')) {

                env.nextdir = 'e';
                pthread_cond_signal(&env.east_go);

            } else if (q_cartIsWaiting('n')) {

                env.nextdir = 'n';
                pthread_cond_signal(&env.north_go);

            } else if (q_cartIsWaiting('w')) {

                env.nextdir = 'w';
                pthread_cond_signal(&env.west_go);

            }
            break;
    }

    printf("\nDirection %c thread signaled direction %c to go\n", cart->dir, env.nextdir);

    q_deleteOne(cart->dir);
    pthread_mutex_unlock(&env.lock);

}

/*
 *  Destroys mutex and cond vars once threads are finished
 */
void monitor_shutdown() {
    if (pthread_mutex_destroy(&env.lock) != 0) {
        perror("pthread_mutex_destroy");
        exit(-1);
    }
    if (pthread_cond_destroy(&env.north_go) != 0) {
        perror("pthread_cond_destroy north_go");
        exit(-1);
    }
    if (pthread_cond_destroy(&env.south_go) != 0) {
        perror("pthread_cond_destroy south_go");
        exit(-1);
    }
    if (pthread_cond_destroy(&env.west_go) != 0) {
        perror("pthread_cond_destroy west_go");
        exit(-1);
    }
    if (pthread_cond_destroy(&env.east_go) != 0) {
        perror("pthread_cond_destroy east_go");
        exit(-1);
    }
}
