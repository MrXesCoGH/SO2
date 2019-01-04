
/* Codigo escrito por Lluis Garrido */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include <pthread.h>
#include "ficheros-csv.h"

//creating the thread locker
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int waiting = 0, count = 0, send = 1, r = 0, w = 0;

//mutex for the producer and consumer
pthread_mutex_t mutex_prod_consum = PTHREAD_MUTEX_INITIALIZER;

//the producer and consumer itselfs
pthread_t prod, consum[N_THREADS];

//conditions for the producer and consumer
pthread_cond_t cond_producer, cond_consumer;

//buffer used by the producer and the consumer. It's shared.
struct buffer *buff_pc;

struct param{
    FILE *fp;
    rb_tree *tree;
};

/**
 *
 * Esta funcion crea el arbol a partir de los datos de los aeropuertos y de los ficheros de retardo
 *
 */
rb_tree *create_tree(char *str_airports, char *str_dades)
{
    FILE *fp;
    rb_tree *tree;

    pthread_t th;
    struct param *p;
    int err;

    struct timeval tv1, tv2;

    gettimeofday(&tv1, NULL);

    /* Reservamos memoria */
    tree = (rb_tree *) malloc(sizeof(rb_tree));

    /* Abrimos el fichero con la lista de aeropuertos */
    fp = fopen(str_airports, "r");

    if (!fp) {

        printf("Could not open file '%s'\n", str_airports);
        exit(EXIT_FAILURE);

    }

    /* Leemos los datos de ficheros de aeropuertos */
    init_tree(tree);

    read_airports(tree,fp);

    fclose(fp);

    /* Abrimos el fichero con los datos de los vuelos */
    fp = fopen(str_dades, "r");

    if (!fp) {

        printf("Could not open file '%s'\n", str_dades);
        exit(EXIT_FAILURE);

    }


    char line[MAXCHAR];

    if(ftell(fp) == 0){
      fgets(line,MAXCHAR,fp);
    }

    //Inicializamos el buffer:

    buff_pc = malloc(sizeof(struct buffer));

    for (int i = 0; i< N_THREADS; i++){
      buff_pc->cell[i]=malloc(sizeof(struct cell));
    }

    //Estos son dos estructuras que almacenan datos que usaran los productores
    //y consumidores.
    struct read_par *rpar;
    struct process_par *ppar;

    //Inicializamos el struct de lectura
    rpar = malloc(sizeof(struct read_par));
    rpar->fp = fp;
    rpar->tx = 1;
    rpar->buff = buff_pc;

    pthread_create(&prod, NULL, read_file, (void *)rpar);
    ppar = malloc(sizeof(struct process_par));

    for(int i = 0; i<N_THREADS; i++){
      ppar->tree = tree;
      ppar->tx = 1;
      ppar->end = 1;
      ppar->buff = buff_pc;
      pthread_create(&consum[i],NULL,process_file,(void *)ppar);
    }

    pthread_join(prod,NULL);

    for(int i = 0; i<N_THREADS;i++){
      pthread_join(consum[i],NULL);
    }

    fclose(fp);

    free(rpar);
    free(ppar);

    for(int i = 0; i< N_THREADS; i++){
      free(buff_pc->cell[i]);
    }

    free(buff_pc);
    /* Se leen los datos y se introducen en el arbol. En este caso, como
    utilizamos los productores y los consumidores no hace falta llamar a este
    metodo. */
    //read_airports_data(tree,fp);

    /*
    * Ya que no utilizamos la funcion de read_aiports_data es necesario contar
    * el tiempo de ejecucion de otra manera, en este caso es coger la parte del
    * codigo que contaba el tiempo y ponerlo a continuacion.
    */

    /* Tiempo cronologico */
    gettimeofday(&tv2, NULL);

    /* Tiempo para la creacion del arbol */
    printf("Tiempo para crear el arbol: %f segundos\n",
            (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec));

    return tree;
}


void tree_filling_thread(rb_tree *tree, FILE *fp, int num_airports, node_data *n_data, char *line){
    int i = 0;

    while (i < num_airports)
    {
        fgets(line, 5, fp);
        line[3] = '\0';

        /* Reservamos memoria para el nodo */
        n_data = malloc(sizeof(node_data));

        /* Copiamos los datos al nodo */
        n_data->key = malloc(sizeof(char) * 4);

        pthread_mutex_t node_mutex = PTHREAD_MUTEX_INITIALIZER;
        n_data->mutex = node_mutex;

        strcpy(n_data->key, line);

        /* Inicializamos la lista */
        n_data->l = malloc(sizeof(list));
        init_list(n_data->l);

        /* Suponemos que los nodos son unicos, no hace falta
         * comprobar si ya existen previamente.
         */
        insert_node(tree, n_data);
        i++;
    }
    printf("Tree structure created.\n");
}


/**
 *
 * Esta funcion lee la lista de los aeropuertos y crea el arbol
 *
 */

void read_airports(rb_tree *tree, FILE *fp)
{
    int num_airports;
    char line[MAXCHAR];

    node_data *n_data = NULL;

    fgets(line, 5, fp);

    num_airports = atoi(line);

    tree_filling_thread(tree,fp,num_airports,n_data,line);

}

/**
 * Función que permite leer todos los campos de la línea de vuelo: origen,
 * destino, retardo.
 *
 */

static int extract_fields_airport(char *line, flight_information *fi) {

    /*Recorre la linea por caracteres*/
    char caracter;
    /* i sirve para recorrer la linea
     * iterator es para copiar el substring de la linea a char
     * coma_count es el contador de comas
     */
    int i, iterator, coma_count;
    /* start indica donde empieza el substring a copiar
     * end indica donde termina el substring a copiar
     * len indica la longitud del substring
     */
    int start, end, len;
    /* invalid nos permite saber si todos los campos son correctos
     * 1 hay error, 0 no hay error pero no hemos terminado
     */
    int invalid = 0;
    /*
     * eow es el caracter de fin de palabra
     */
    char eow = '\0';
    /*
     * contenedor del substring a copiar
     */
    char *word;
    /*
     * Inicializamos los valores de las variables
     */
    start = 0;
    end = -1;
    i = 0;
    coma_count = 0;
    /*
     * Empezamos a contar comas
     */
    do {
        caracter = line[i++];
        if (caracter == ',') {
            coma_count ++;
            /*
             * Cogemos el valor de end
             */
            end = i;
            /*
             * Si es uno de los campos que queremos procedemos a copiar el substring
             */
            if(coma_count == ATRASO_LLEGADA_AEROPUERTO ||
                    coma_count == AEROPUERTO_ORIGEN ||
                    coma_count == AEROPUERTO_DESTINO){
                /*
                 * Calculamos la longitud, si es mayor que 1 es que tenemos
                 * algo que copiar
                 */
                len = end - start;
                if (len > 1) {
                    /*
                     * Alojamos memoria y copiamos el substring
                     */
                    word = (char*) malloc(sizeof(char)*(len));
                    for(iterator = start; iterator < end-1; iterator ++){
                        word[iterator-start] = line[iterator];
                    }
                    /*
                     * Introducimos el caracter de fin de palabra
                     */
                    word[iterator-start] = eow;
                    /*
                     * Comprobamos que el campo no sea NA (Not Available)
                     */
                    if (strcmp("NA", word) == 0)
                        invalid = 1;
                    else {
                        switch (coma_count) {
                            case ATRASO_LLEGADA_AEROPUERTO:
                                fi->delay = atoi(word);
                                break;
                            case AEROPUERTO_ORIGEN:
                                strcpy(fi->origin, word);
                                break;
                            case AEROPUERTO_DESTINO:
                                strcpy(fi->destination, word);
                                break;
                            default:
                                printf("ERROR in coma_count\n");
                                exit(1);
                        }
                    }

                    free(word);

                } else {
                    /*
                     * Si el campo esta vacio invalidamos la linea entera
                     */

                    invalid = 1;
                }
            }
            start = end;
        }
    } while (caracter && invalid==0);

    return invalid;
}

/*
 *  This is the thread that fills the tree with the elements of the airports file and the data file.
 */

void tree_filler(char* origin,char* destination,int delay, struct param *parameters){

    node_data *n_data;
    list_data *l_data;

    n_data = find_node(parameters->tree, origin);

    if (n_data) {

        pthread_mutex_lock(&(n_data->mutex));

        l_data = find_list(n_data->l, destination);

        if (l_data) {
            l_data->numero_vuelos += 1;
            l_data->retardo_total += delay;
        } else {
            l_data = malloc(sizeof(list_data));

            l_data->key = malloc(sizeof(char) * 4);
            strcpy(l_data->key, destination);

            l_data->numero_vuelos = 1;
            l_data->retardo_total = delay;


            insert_list(n_data->l, l_data);

        }
        pthread_mutex_unlock(&(n_data->mutex));

    } else {
        printf("ERROR: aeropuerto %s no encontrado en el arbol.\n", origin);
        exit(1);
    }
}

void sub_thread(void *arguments)
{
    char line[MAXCHAR];
    int invalid, current_line;

    flight_information fi;

    char fi_list[NUM_LINES][MAXCHAR];

    struct param *parameters = (struct param *) arguments;

    int reading = 1;

    while (reading)
    {
        current_line = 0;
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < NUM_LINES; i++){

            if(fgets(line, MAXCHAR, parameters->fp) != NULL){

                strcpy(fi_list[i],line);

                current_line = i;
            }else{
                current_line = i;
                reading = 0;
                break;
            }
        }

        pthread_mutex_unlock(&mutex);

        for(int i = 0; i < current_line; i++){
            invalid = extract_fields_airport(fi_list[i], &fi);

            if (!invalid) {
                tree_filler(fi.origin, fi.destination, fi.delay, parameters);
            }
        }
    }
}

/**
 *
 * Esta funcion lee los datos de los vuelos y los inserta en el
 * arbol (previamente creado)
 *
 */

void read_airports_data(rb_tree *tree, FILE *fp) {
    char line[MAXCHAR];
    int i;

    struct timeval tv1, tv2;

    /* Leemos la cabecera del fichero */
    fgets(line, MAXCHAR, fp);

    pthread_t threads[N_THREADS];
    struct param *parameters;

    /* Tiempo cronologico */
    gettimeofday(&tv1, NULL);

    parameters = malloc(sizeof(struct param));
    parameters->fp = fp;
    parameters->tree = tree;

    for(i = 0; i<N_THREADS; i++){

        pthread_create(&(threads[i]), NULL,(void *) sub_thread, (void *) parameters);

    }

    for(i= 0;i<N_THREADS;i++){
        pthread_join(threads[i],NULL);
    }

    /* Tiempo cronologico */
    gettimeofday(&tv2, NULL);

    /* Tiempo para la creacion del arbol */
    printf("Tiempo para crear el arbol: %f segundos\n",
            (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec));

    free(parameters);
}


// Funcion que define el funcionamiento los productores
void *producer(void *args){
  char line[MAXCHAR];
  struct read_par *par = (struct reader *) args;

  pthread_mutex_lock(&mutex_prod_consum);

  if(count == N_THREADS){
    pthread_cond_wait(&cond_producer, &mutex_prod_consum);
  }

  if(fgets(line, MAXCHAR, par->fp) != NULL){
    strcpy(par->buff->cell[w]->str,line);
  }

  if (feof(par->fp)){
    par->tx = 0;
    send = 0;
  }

  w = (w+1) % N_THREADS;
  count++;
  pthread_cond_broadcast(&cond_consumer);
  pthread_mutex_unlock(&mutex_prod_consum);
}

// Funcion que define el funcionamiento de los consumidores
void *consumer(void *args){
  int invalid;

  flight_information fi;

  node_data *n_data;
  list_data *l_data;

  struct process_par *par = (struct processor *) args;

  char *n_lines;

  pthread_mutex_lock(&mutex_prod_consum);
  while(count == 0){
    pthread_cond_wait(&cond_consumer, &mutex_prod_consum);
  }

  if(send){
    n_lines = malloc(sizeof(char) * (strlen(par->buff->cell[r]->str)+1));
    n_lines[strlen(par->buff->cell[r]->str)-1] = '\0';

    strcpy(n_lines, par->buff->cell[r]->str);
    r = (r+1)% N_THREADS;
    count--;

    pthread_cond_broadcast(&cond_producer);
  }else{
    par->end = 0;
  }

  pthread_mutex_unlock(&mutex_prod_consum);

  if(send){
    invalid = extract_fields_airport(n_lines, &fi);

    if(!invalid){
      n_data = find_node(par->tree, fi.origin);

      if(n_data){
         pthread_mutex_lock(&n_data->mutex);

         l_data = find_list(n_data->l, fi.destination);

         if(l_data){
           l_data->numero_vuelos += 1;
           l_data->retardo_total += fi.delay;
         }else{

            l_data = malloc(sizeof(list_data));
            l_data->key = malloc(sizeof(char) * 4);
            strcpy(l_data->key, fi.destination);

            l_data->numero_vuelos = 1;
            l_data->retardo_total = fi.delay;

            insert_list(n_data->l, l_data);

         }

         pthread_mutex_unlock(&n_data->mutex);

      }else{
         printf("ERR: Airport %s not found.\n", fi.origin);
         exit(EXIT_FAILURE);
      }
    }
    free(n_lines);
  }
}

// Esta funcion hace la llamada a los consumidores
void *process_file(void *args){

  pthread_t tid;

  struct process_par *par = (struct processor *) args;
  while(par->end == 1 && send){
    pthread_create(&tid,NULL,consumer,(void *) args);
    pthread_join(tid,NULL);
  }

}

// Esta funcion hace la llamada a los productores
void *read_file(void *args){

  pthread_t tid;

  struct read_par *par = (struct reader *) args;

  while(!feof(par->fp) && par->tx == 1){
    pthread_create(&tid,NULL,producer,(void *)args);
    pthread_join(tid,NULL);
  }
}
