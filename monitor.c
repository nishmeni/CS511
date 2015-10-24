#include "monitor.h"
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

void monitor_arrive(struct cart_t *cart) {
    /* wait for mutex lock */
    if (pthread_mutex_lock(&env.lock) != 0) {
        perror("pthread_mutex_lock");
	exit(EXIT_FAILURE);
    }

    switch (cart->dir) {
        case 'n':

	  if (!q_cartIsWaiting('n')) {
	    if (pthread_cond_wait(&env.north_go, &env.lock) != 0) {
	      perror("pthread_cond_wait drain_thread");                               
	      exit(EXIT_FAILURE);
	    }
	  }
	  break;
        
        case 's':
	  
	  if (!q_cartIsWaiting('s')) {
	    if (pthread_cond_wait(&env.south_go, &env.lock) != 0) {
	      perror("pthread_cond_wait drain_thread");                   
	      exit(EXIT_FAILURE);
	    }
	  }
	  break;
    
        case 'e':

	  if (!q_cartIsWaiting('e')) {
	    if (pthread_cond_wait(&env.east_go, &env.lock) != 0) {
	      perror("pthread_cond_wait drain_thread");
	      exit(EXIT_FAILURE);
	    }
	  }
	  break;
        case 'w':

	  if (!q_cartIsWaiting('w')) {
	    if (pthread_cond_wait(&env.west_go, &env.lock) != 0) {
	      perror("pthread_cond_wait drain_thread");
	      exit(EXIT_FAILURE);
	    }
	  }
	  break;	  
    }
    
}

void monitor_cross(struct cart_t *cart) {
  
  printf("Cart %i is crossing from direction %c\n", cart->num, cart->dir);
  q_cartHasEntered(cart->dir);

}

void monitor_leave(struct cart_t *cart) {
if (pthread_mutex_lock(&env.lock) != 0) {
        perror("pthread_mutex_lock");
	exit(EXIT_FAILURE);
    }

    switch (cart->dir) {
      
       case 'n':
	 if(q_cartIsWaiting('w'))
	   pthread_cond_signal(&env.west_go);
	 else if(q_cartIsWaiting('s'))
	   pthread_cond_signal(&env.south_go);
	 else if(q_cartIsWaiting('e'))
	   pthread_cond_signal(&env.east_go);
	 else if(q_cartIsWaiting('n'))
	   pthread_cond_signal(&env.north_go);
	 break;

       case 's':
	 if(q_cartIsWaiting('e'))
	   pthread_cond_signal(&env.east_go);
	 else if(q_cartIsWaiting('n'))
	   pthread_cond_signal(&env.north_go);
	 else if(q_cartIsWaiting('w'))
	   pthread_cond_signal(&env.west_go);
	 else if(q_cartIsWaiting('s'))
	   pthread_cond_signal(&env.south_go);
	 break;
	 
       case 'e':
	 if(q_cartIsWaiting('n'))
	   pthread_cond_signal(&env.north_go);
	 else if(q_cartIsWaiting('w'))
	   pthread_cond_signal(&env.west_go);
	 else if(q_cartIsWaiting('s'))
	   pthread_cond_signal(&env.south_go);
	 else if(q_cartIsWaiting('e'))
	   pthread_cond_signal(&env.east_go);
	 break;
	 
       case 'w':
	 if(q_cartIsWaiting('s'))
	   pthread_cond_signal(&env.south_go);
	 else if(q_cartIsWaiting('e'))
	   pthread_cond_signal(&env.east_go);
	 else if(q_cartIsWaiting('n'))
	   pthread_cond_signal(&env.north_go);
	 else if(q_cartIsWaiting('w'))
	   pthread_cond_signal(&env.west_go);
	 break;


    }

    q_deleteOne(cart->dir);
    pthread_mutex_unlock(&env.lock);
    usleep(1000);
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
