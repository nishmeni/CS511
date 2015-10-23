#include "q.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


void cleanexit(){

  q_shutdown;
  exit(EXIT_FAILURE);
}

void argerror(){

  printf("Correct Usage: ./trafficmgr <d1d2d3d4...>\n Where dn E [n,s,e,w] is the direction of the nth cart.\n");
  cleanexit();
}

void init(char* arg){
  int i;
  q_init;

  for(i = 0; arg[i] != '\0'; i++){
    switch(arg[i]){

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
    default:
      argerror();
    }
  }
}

int main(int argc, char** argv){

  if(argc == 2){
    init(argv[1]);

  }
  else {
    argerror();
  }

  cleanexit();
  return 0;
}
