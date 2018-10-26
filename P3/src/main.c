/**
 *
 * Practica 3
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "red-black-tree.h"
#include "linked-list.h"

#define MAXLINE      400
#define MAGIC_NUMBER 0x0133C8F9

/**
 *
 *  Menu
 *
 */

void split_fn(char* string, int col, char* result);



int menu()
{
    char choice[5];
    int opcio;

    printf("\n\nMenu\n\n");
    printf(" 1 - Creacio de l'arbre\n");
    printf(" 2 - Emmagatzemar arbre a disc\n");
    printf(" 3 - Llegir arbre de disc\n");
    printf(" 4 - Consultar informacio de l'arbre\n");
    printf(" 5 - Sortir\n\n");
    printf("   Escull opcio: ");

    fgets(choice, 5, stdin);
    opcio = atoi(choice);

    return opcio;
}

void split_fn(char *string, int col, char* result){
    int index = 0;
    int i = 0;
    char delim = ',';
    int found = 0;

    while (i < strlen(string) && !found) {
        
        if (string[i] == delim) {
            // Comma found
            col--;
            // If we've reached our desired position before, we've
            // retrieved the entire string.
            if (col < 0) {
                found = 1;
            }
        } else {
            // Normal character
            if (col == 0) {
                // If we've reached our desired position, add character
                // to output string.
                result[index] = string[i];
                index++;
            }
            result[index] = 0;
        }
        i++;
    }
}

/**
 *
 *  Main procedure
 *
 */

int main(int argc, char **argv)
{
    char str1[MAXLINE], str2[MAXLINE];
    int opcio;

    FILE *fp;

    rb_tree *airports_tree;

    /* Allocate memory for tree */
    airports_tree = (rb_tree *) malloc(sizeof(rb_tree));
    /* Initialize the tree */
    init_tree(airports_tree);

    node_data *n_data;
    int appeared = 0; //Boolean statement

    char str[5];
    char *s;

    if (argc != 1)
        printf("Opcions de la linia de comandes ignorades\n");

    do {
        opcio = menu();
        printf("\n\n");

       /* Feu servir aquest codi com a pantilla */

        switch (opcio) {
            case 1:
                printf("Introdueix fitxer que conte llistat d'aeroports: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                fp = fopen(str1,"r");
                if(fp == NULL){
                    perror("Error al obrir el fitxer");
                    return(-1);
                }

                if (fgets (str,5,fp) != NULL){
                    puts(str);
                }

                int i = 0; //This is just a simple counter used to iterate in the following loop.

                while(fgets(str, 5, fp) != NULL && i < 5){

                    //This search if the str value is already inside the tree.
                   	if(!appeared){
                   		  appeared = 1;
                    } else {
                        n_data = malloc(sizeof(node_data));

                        //Here it's stored the length of the char array.
                        s = malloc(sizeof(char)*4);
                        str[3] ='\0';
                        strcpy(s, str);
                       
                        /* If the key is not in the tree, allocate memory for the data
                        * and insert in the tree */
                        n_data->key = malloc((strlen(str)+1)*sizeof(char));
                        n_data->key = s;

                        n_data->num_destinations = 0;

                        //Destinations list
                        n_data->l = (list*) malloc(sizeof(list));
                        init_list(n_data->l);

                        insert_node(airports_tree, n_data);
                    }
                }

                fclose(fp);

                printf("Introdueix fitxer de dades: ");
                fgets(str2, MAXLINE, stdin);
                str2[strlen(str2)-1]=0;

                /* Falta codi */
                char *origin, *destination, *delay;
                
                origin = ((char*) malloc(sizeof(char)*4));
                delay = ((char*) malloc(sizeof(char)*10));
                destination =((char*) malloc(sizeof(char)*4));

                float delay_f;
                list_data *l_data;

                fp = fopen(str2,"r");
                if(fp == NULL){
                    perror("Error al obrir el fitxer");
                    return(-1);
                }

                fgets(str,400,fp); 

                //int DATA_SIZE = atoi(str); //This int stores the total number of lines that We read.
                i = 0;

                /*
                * Now that the dades file is opened, I need to read the columns.
                * In this case, the columns are:  15, 17, 18.
                * Each column has an especified data:
                *   - col. 15: Delay to the arriving airport, in minutes (int)
                *   - col. 17: Origin airport, IATA Code.
                *   - col. 18: Destiny airport, IATA Code.
                */ 
                
                while(fgets(str, MAXLINE , fp) != NULL){
                    printf("primera linea: %s \n", str);
                    
                    //Here it's stored the length of the char array.
                    int h = strlen(str);

                    //turn the \n to 0
                    str[h-1] = '\0';

                    //this uses the splitting function to obtain the values of the delay, the origin and
                    //the destination of the airports.
                    
                    split_fn(str,15, delay);
                    
                    split_fn(str,17, origin);
                    
                    split_fn(str,18, destination);
                    
                    printf("Origen: %s \n", origin);
                    printf("delay: %s \n", delay);
                    printf("destination: %s \n",destination);
                    
                    //this searchs if the node is already inside the tree.

                    n_data = find_node(airports_tree, origin);
                    
                    if(n_data != NULL){
                        printf("Airport found \n");
                        
                        l_data = find_list(n_data->l,destination);

                        if(l_data != NULL){
                            //In case that the found l_data is inside the list of the node, then
                            //the number of times that flight appears has to increase by 1.
                            l_data->num_flights++;

                            //After that, if the flight also has delay, the total delay has to increase too.
                            if(strcmp(delay,"NA") != 0) {
                                delay_f = atof(delay);
                                l_data->delay = l_data->delay + delay_f;
                            }
                        }else{
                            //In the case the found l_data is null, then a new one has to be created, because a new flight
                            //has to be added to the flights list.
                            l_data = malloc(sizeof(list_data));
                            
                            //In the case of this strlen, the string has a \0 at the end which has to be considered.
                            l_data->key = malloc((strlen(destination)+1)*sizeof(char));

                            strcpy(l_data->key, destination);
                            
                            //Since is the first flight for the airport, then the total number of flights starts as 1.
                            l_data->num_flights = 1;

                            //Just as before, if the flight has delay, it has to be added too.
                            if(strcmp(delay, "NA")!=0){
                                l_data->delay = atof(delay);
                            }
                            
                            else{
                            //In this case, the flight has no delay so the total value of the delays happens to be 0.
                            l_data->delay = 0;
                            
                            }

                            //Then this new flight has to be added to the flight list and the number of destinations of the airport
                            //increases by 1.
                            insert_list(n_data->l, l_data);
                            n_data->num_destinations++;
                        }
                    }
                }
                
                free(origin);
                free(delay);
                free(destination);

                fclose(fp);

                break;

            case 2:
                printf("Introdueix el nom de fitxer en el qual es desara l'arbre: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                /* Falta codi */
                fp = fopen(str,"w");

                if(!fp){
                  printf("ERR: Failed at opening the file\n" );
                  exit(EXIT_FAILURE);
                }

                //Mirar como se escribe un fichero.

                break;

            case 3:
                printf("Introdueix nom del fitxer que conte l'arbre: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                /* Falta codi */

                break;

            case 4:
                printf("Introdueix aeroport per cercar retard o polsa enter per saber l'aeroport amb mes destinacions: ");
                fgets(str1, MAXLINE, stdin);

                printf("String entrado: %s",str1);
                //10 is the ASCII code of the enter key (\n).
                if(str1[0] == '\n'){
                    printf("Enter\n");
                    n_data = search(airports_tree);
                    if (n_data)
                        printf("\nAirport with more destinations: %s, number: %d \n\n", n_data->key, n_data->num_destinations);
                    else 
                        printf("No he trobat l'aeroport amb mes destinacions\n");

                }else{
                    str1[strlen(str1)-1]=0;

                  printf("\nMedian delay for the airport %s:\n", str1);

                  n_data = find_node(airports_tree, str1);
                  list_item *l_item = n_data->l->first;

                  while(l_item != NULL){
                      
                      l_data = l_item->data;
                      printf("   %s --> %.3f minutes\n", l_data->key, (l_data->delay/l_data->num_flights));
                      l_item = l_item->next;

                  }
                }

                break;

            case 5:

                /* Falta codi */
                delete_tree(airports_tree);
                free(airports_tree);

                printf("\nSortint...\n");

                break;

            default:
                printf("Opcio no valida\n");

        } /* switch */
    }
    while (opcio != 5);

    return 0;
}
