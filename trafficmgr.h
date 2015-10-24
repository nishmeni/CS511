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

void cleanexit();
void argerror();
int check_match(char*, char*);
void init(char*);


#endif
