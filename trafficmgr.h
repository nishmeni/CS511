#ifndef _TRAFFICMGR_H_
#define _TRAFFICMGR_H_

#include "q.h"
#include "cart.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <regex.h>

typedef struct arg_s {
  char direction;
} arg_t;

typedef struct s_env {

  int n;
  int s;
  int e;
  int w;

} t_env;

t_env gl_env;
void cleanexit();
void argerror();
int check_match(char*, char*);
void init(char*);
void *cart(void*);


#endif
