#pragma once

#include "linked-list.h"
#include "red-black-tree.h"

#define MAXCHAR 500

#define ATRASO_LLEGADA_AEROPUERTO 15
#define AEROPUERTO_ORIGEN 17
#define AEROPUERTO_DESTINO 18

#define NUM_LINES 10000
#define N_THREADS 4

typedef struct flight_information {
    char origin[4];
    char destination[4];
    int delay;
} flight_information;


typedef struct read_par{
  FILE *fp;
  int tx; //sending indicator
  struct buffer *buff;
} read_par;

typedef struct process_par{
  rb_tree *tree;
  int tx; //sending indicator
  int end;
  struct buffer *buff;
} process_par;

struct buffer{
  int size;
  struct cell *cell[N_THREADS];
};

struct cell{
  char str[1000];
  int size;
}

rb_tree *create_tree(char *str_airports, char *str_dades);
void read_airports(rb_tree *tree, FILE *fp);
void read_airports_data(rb_tree *tree, FILE *fp);


//functions for the producer and the consumers 
void *producer(void *args);
void *consumer(void *args);
void *read_file(void *args);
void *process_file(void *args);
