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
        printf("Expression matched: %s\n", arg);
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


    for(i = 0; arg[i] != '\0'; i++){
        switch (arg[i]) {

	    case 'n':
	      q_putCart('n');
	      break;
            case 's':
	      q_putCart('s');
	      break;
            case 'e':
	      q_putCart('e');
	      break;
            case 'w':
	      q_putCart('w');
	      break;
        }
    }
}



void* cart(void* args){
 
  arg_t* actual;
  char direction;
  actual = (arg_t*) args;
  direction = actual->direction;
  struct cart_t *cart;

  fprintf(stderr, "thread for direction %c starts\n", direction); 
  cart = q_getCart(direction); 
  
  while (cart != NULL) { 
    fprintf(stderr, "thread for direction %c gets cart %i\n", direction, cart->num); 
    //monitor_arrive(cart); 
    //monitor_cross(cart); 
    //monitor_leave(cart); 
    cart = q_getCart(direction); 
  } 
  
  fprintf(stderr, "thread for direction %c exits\n", direction);

}

int main(int argc, char** argv) {

  if (argc == 2 && check_match(argv[1],"^[nsew]*$") > 0) {
    init(argv[1]);




  } else { 
    
    argerror();
  }
  

  cleanexit();  
}
