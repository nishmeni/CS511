#include "trafficmgr.h"

void cleanexit() {
  q_delete('n');
  q_delete('s');
  q_delete('e');
  q_delete('w');
  q_shutdown;
  exit(EXIT_FAILURE);
}

void argerror() {
    printf("Correct Usage: ./trafficmgr <d1d2d3d4...>\n Where dn E [n,s,e,w] is the direction of the nth cart.\n");
    cleanexit();
}

/*
 *  Checks if the given string matches the defined synax for command line args
 *  arg - string to check
 */
int check_match(char *arg, char *reg) {
    regex_t regex;
    int reti;
    char msgbuf[100];

    /* Compile regular expression */
    reti = regcomp(&regex, reg, 0);

    if (reti) {
      fprintf(stderr, "Could not compile regex\n");
      exit(1);
    }

    /* Execute regular expression */
    reti = regexec(&regex, arg, 0, NULL, 0);

    if (!reti) {
        /* match found */
        regfree(&regex);
        return 1;
    } else if (reti == REG_NOMATCH) {
        /* no match found */
        printf("Expression not matched: %s\n", arg);
        regfree(&regex);
        return -1;
    } else {
        /* expression match failed */
        regerror(reti, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        regfree(&regex);
	exit(EXIT_FAILURE);
    }

}

void init(char* arg) {
    int i;
    q_init;
    gl_env.n = 0;
    gl_env.s = 0;
    gl_env.e = 0;
    gl_env.w = 0;

    for(i = 0; arg[i] != '\0'; i++){
        switch (arg[i]) {

	    case 'n':
	      gl_env.n = 1;
	      q_putCart('n');
	      break;
            case 's':
	      gl_env.s = 1;
	      q_putCart('s');
	      break;
            case 'e':
	      gl_env.e = 1;
	      q_putCart('e');
	      break;
            case 'w':
	      gl_env.w = 1;
	      q_putCart('w');
	      break;
        }
    }
}



void *cart(void* args){

  arg_t* actual;
  char direction;
  actual = (arg_t*) args;
  direction = actual->direction;
  struct cart_t *cart;

  fprintf(stderr, "thread for direction %c starts\n", direction);
  cart = q_getCart(direction);

  while (cart != NULL) {
    fprintf(stderr, "thread for direction %c gets cart %i\n", direction, cart->num);
    monitor_arrive(cart);
    monitor_cross(cart);
    monitor_leave(cart);
    cart = q_getCart(direction);
  }

  fprintf(stderr, "thread for direction %c exits\n", direction);

}

int main(int argc, char** argv) {

  int rc;
  arg_t n,s,e,w;
  pthread_t north, south, east, west;

  if (argc == 2 && check_match(argv[1],"^[nsew]*$") > 0) {
    init(argv[1]);
    monitor_init();

    if(gl_env.n){
      n.direction = 'n';
      rc = pthread_create(&north, NULL, cart, (void*)&n);
      if(rc){
	perror("Error creating North thread");
	cleanexit();
      }
    }

    if(gl_env.s){
      s.direction = 's';
      rc = pthread_create(&south, NULL, cart, (void*)&s);
      if(rc){
	perror("Error creating South thread");
	cleanexit();
      }
    }

    if(gl_env.e){
      e.direction = 'e';
      rc = pthread_create(&east, NULL, cart, (void*)&e);
      if(rc){
	perror("Error creating East thread");
	cleanexit();
      }
    }

    if(gl_env.w){
      w.direction = 'w';
      rc = pthread_create(&west, NULL, cart, (void*)&w);
      if(rc){
	perror("Error creating West thread");
	cleanexit();
      }
    }

    if(gl_env.n)
      pthread_join(north, NULL);
    if(gl_env.s)
      pthread_join(south, NULL);
    if(gl_env.e)
      pthread_join(east, NULL);
    if(gl_env.w)
      pthread_join(west, NULL);

    monitor_shutdown();

  } else {

    argerror();
  }


  cleanexit();
}
