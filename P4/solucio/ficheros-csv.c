
/* Codigo escrito por Lluis Garrido */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "ficheros-csv.h"

/**
 *
 * Esta funcion crea el arbol a partir de los datos de los aeropuertos y de los ficheros de retardo
 *
 */

rb_tree *create_tree(char *str_airports, char *str_dades)
{
    FILE *fp;
    rb_tree *tree;

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
    read_airports(tree, fp); 
    fclose(fp);

    /* Abrimos el fichero con los datos de los vuelos */
    fp = fopen(str_dades, "r");
    if (!fp) {
        printf("Could not open file '%s'\n", str_dades);
        exit(EXIT_FAILURE);
    }

    /* Se leen los datos y se introducen en el arbol */
    read_airports_data(tree, fp);
    fclose(fp);

    return tree;
}


/**
 *
 * Esta funcion lee la lista de los aeropuertos y crea el arbol 
 *
 */

void read_airports(rb_tree *tree, FILE *fp) 
{
    int i, num_airports;
    char line[MAXCHAR];

    /*
     * eow es el caracter de fin de palabra
     */
    char eow = '\0';

    node_data *n_data;

    fgets(line, 100, fp);
    num_airports = atoi(line);

    i = 0;
    while (i < num_airports)
    {
        fgets(line, 100, fp);
        line[3] = eow; 

        /* Reservamos memoria para el nodo */
        n_data = malloc(sizeof(node_data));

        /* Copiamos los datos al nodo */
        n_data->key = malloc(sizeof(char) * 4);
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
                    word =(char*)malloc(sizeof(char)*(len));
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

/**
 *
 * Esta funcion lee los datos de los vuelos y los inserta en el 
 * arbol (previamente creado)
 *
 */

void read_airports_data(rb_tree *tree, FILE *fp) {
    char line[MAXCHAR];
    int invalid;

    flight_information fi;

    node_data *n_data;
    list_data *l_data;

    struct timeval tv1, tv2;

    /* Leemos la cabecera del fichero */
    fgets(line, MAXCHAR, fp);

    /* Tiempo cronologico */
    gettimeofday(&tv1, NULL);
    
    while (fgets(line, MAXCHAR, fp) != NULL)
    {
        invalid = extract_fields_airport(line, &fi);

        if (!invalid) {
            n_data = find_node(tree, fi.origin);

            if (n_data) {
                l_data = find_list(n_data->l, fi.destination);

                if (l_data) {
                    l_data->numero_vuelos += 1;
                    l_data->retardo_total += fi.delay;
                } else {
                    l_data = malloc(sizeof(list_data));

                    l_data->key = malloc(sizeof(char) * 4);
                    strcpy(l_data->key, fi.destination);

                    l_data->numero_vuelos = 1;
                    l_data->retardo_total = fi.delay; 

                    insert_list(n_data->l, l_data);
                }

            } else {
                printf("ERROR: aeropuerto %s no encontrado en el arbol.\n", fi.origin);
                exit(1);
            }
        }
    }

    /* Tiempo cronologico */
    gettimeofday(&tv2, NULL);

    /* Tiempo para la creacion del arbol */
    printf("Tiempo para crear el arbol: %f segundos\n",
            (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
            (double) (tv2.tv_sec - tv1.tv_sec));
}



