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
void writeTreeData(node_data *n_data, FILE *fp);
void writeTree(rb_tree *tree, FILE *fp);
void writeTreeRecursive(node *n, FILE *fp);
void writeTreeList(list *l, FILE *fp);
int countTreeRecursive(node *n);

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

void writeTreeList(list *l, FILE *fp){
    list_item *actual;
    actual = l->first;
    
    while (actual != NULL){
        list_data *data = actual->data;
        
        if(data != NULL){
            fwrite(data->key, sizeof(char), 3,fp);
            fwrite(&data->num_flights, sizeof(int), 1, fp);
            fwrite(&data->delay, sizeof(int), 1, fp);
        }
        
        actual = actual->next;
    }    
}

void writeTreeData(node_data *n_data, FILE *fp){
    
    fwrite(n_data->key, sizeof(char),3,fp);
    
    fwrite(&n_data->num_destinations, sizeof(int),1,fp);
    
    writeTreeList(n_data->l,fp);
}

void writeTreeRecursive(node *n, FILE *fp){
      if(n->right != NIL)
          writeTreeRecursive(n->right, fp);
      if(n->left != NIL)
          writeTreeRecursive(n->left, fp);
     
      writeTreeData(n->data, fp);
}

void writeTree(rb_tree *tree, FILE *fp){
    if(tree->root != NIL)
        writeTreeRecursive(tree->root, fp);
}

int countTreeRecursive(node *n){
    int rightCount, leftCount;

    rightCount = 0;
    leftCount = 0;

    if (n->right != NIL)
      rightCount = countTreeRecursive(n->right);

    if (n->left != NIL)
      leftCount = countTreeRecursive(n->left);

    return ((int)1) + rightCount + leftCount;
}


/**
 *
 *  Main procedure
 *
 */

int main(int argc, char **argv)
{
    char str1[MAXLINE], str2[MAXLINE];
    int opcio, magicNumber, num_nodes;

    FILE *fp;

    rb_tree *airports_tree;
                   
    /* Allocate memory for tree */
    airports_tree = (rb_tree *) malloc(sizeof(rb_tree));
    
    /* Initialize the tree */
    init_tree(airports_tree);

    node_data *n_data;
    int appeared = 0; //Boolean statement

    char str[5];

    if (argc != 1)
        printf("Opcions de la linia de comandes ignorades\n");

    do {
        opcio = menu();
        printf("\n\n");

       /* Feu servir aquest codi com a pantilla */

        switch (opcio) {
            case 1:
               if(airports_tree){
                    printf("\nFreeing the actual tree\n");
                    delete_tree(airports_tree);
                    free(airports_tree);
                }            
               
                /* Allocate memory for tree */
                airports_tree = (rb_tree *) malloc(sizeof(rb_tree));
                
                /* Initialize the tree */
                init_tree(airports_tree);
                
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

                        str[3] =0;

                        /* If the key is not in the tree, allocate memory for the data
                        * and insert in the tree */
                        n_data->key = malloc((strlen(str)+1)*sizeof(char));
                        strcpy(n_data->key, str);

                        n_data->num_destinations = 0;

                        //Destinations list
                        n_data->l =  malloc(sizeof(list));
                        init_list(n_data->l);

                        insert_node(airports_tree, n_data);

                    }
                }

                fclose(fp);

                printf("Introdueix fitxer de dades: ");
                fgets(str2, MAXLINE, stdin);
                str2[strlen(str2)-1]=0;

                char *origin, *destination, *delay;

                origin = ((char*) malloc(sizeof(char)*4));
                delay = ((char*) malloc(sizeof(char)*10));
                destination =((char*) malloc(sizeof(char)*5));

                float delay_f;
                list_data *l_data;

                fp = fopen(str2,"r");
                if(fp == NULL){
                    perror("Error al obrir el fitxer");
                    return(-1);
                }

                fgets(str,400,fp);

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

                    //Here it's stored the length of the char array.
                    int h = strlen(str);

                    //turn the \n to 0
                    str[h-1] = '\0';

                    //this uses the splitting function to obtain the values of the delay, the origin and
                    //the destination of the airports.

                    split_fn(str,14, delay);

                    split_fn(str,16, origin);

                    split_fn(str,17, destination);

                    //this searchs if the node is already inside the tree.

                    n_data = find_node(airports_tree, origin);

                    if(n_data != NULL){
                        
                        l_data = find_list(n_data->l,destination);

                        if(l_data != NULL){
                            //In case that the found l_data is inside the list of the node, then
                            //the number of times that flight appears has to increase by 1.
                            l_data->num_flights++;

                            //After that, if the flight also has delay, the total delay has to increase too.
                            if(strcmp(delay,"NA") != 0) {
                                delay_f = atoi(delay);
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
                                l_data->delay = atoi(delay);
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
                if(!airports_tree){
                    printf("ERR: There's no tree in memory");
                    break;
                }

                magicNumber = MAGIC_NUMBER;

                printf("Introdueix el nom de fitxer en el qual es desara l'arbre: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                fp = fopen(str1,"w");

                if(!fp){
                  printf("ERR: Failed at opening the file\n" );
                  exit(EXIT_FAILURE);
                }

                fwrite(&magicNumber, sizeof(int), 1, fp);

                num_nodes = countTreeRecursive(airports_tree->root);

                fwrite(&num_nodes, sizeof(int),1,fp);

                writeTree(airports_tree,fp);
                
                fclose(fp);

                break;

            case 3:
                if(airports_tree){
                    printf("\nFreeing the actual tree\n");
                    delete_tree(airports_tree);
                    free(airports_tree);
                }

                printf("Introdueix nom del fitxer que conte l'arbre: ");
                fgets(str1, MAXLINE, stdin);
                str1[strlen(str1)-1]=0;

                printf("Reading tree from file %s \n", str1);

                fp = fopen(str1,"r");

                if(!fp){
                    printf("ERR: Unable to open the file\n");
                    exit(EXIT_FAILURE);
                }

                fread(&magicNumber,sizeof(int),1,fp);

                if (magicNumber != MAGIC_NUMBER){
                    printf("ERR: Incorrect magic number");
                    exit(EXIT_FAILURE);
                }else{
                    printf("\nCorrect MAGIC NUMBER\n");
                }

                fread(&num_nodes, sizeof(int), 1, fp);
                if(num_nodes <= 0){
                    printf("ERR: Number of nodes are 0 or less");
                    exit(EXIT_FAILURE);
                }else{
                    printf("Num nodes: %d \n", num_nodes);
                }
                
                airports_tree = (rb_tree *) malloc(sizeof(rb_tree));
                init_tree(airports_tree);
                                
                if(!airports_tree){
                    printf("\nERR: Unable to create the tree.\n");
                    exit(EXIT_FAILURE);
                }

                int nDestinations, j;
                
                for (i=0;i<num_nodes; i++){
                    n_data = malloc(sizeof(*n_data));
                    n_data->key = calloc(1,sizeof(char)*4);
                    
                    n_data->l = malloc(sizeof(list));
                    init_list(n_data->l);
                    
                    fread(n_data->key, sizeof(char), 3, fp);
                    n_data->key[3] = 0;  
                    
                    //number of destinations
                    fread(&nDestinations, sizeof(int), 1,fp);                    
                    
                    for(j=0; j<nDestinations; j++){
                        l_data = malloc(sizeof(* l_data)); 
                        l_data->key = calloc(1,sizeof(char)*4);
                        
                        
                        fread(l_data->key, sizeof(char), 3, fp);
                        fread(&l_data->num_flights, sizeof(int), 1, fp);
                        fread(&l_data->delay, sizeof(int), 1, fp);
                        
                        l_data->key[3] = 0;
                        
                        insert_list(n_data->l, l_data);
                    }
                    n_data->num_destinations = nDestinations;
                    insert_node(airports_tree, n_data);

                }

                fclose(fp);

                break;

            case 4:
                printf("Introdueix aeroport per cercar retard o polsa enter per saber l'aeroport amb mes destinacions: ");
                fgets(str1, MAXLINE, stdin);

                if(str1[0] == '\n'){
                    n_data = search(airports_tree);
                    if (n_data)
                        printf("\nAirport with more destinations: %s, number: %d \n\n", n_data->key, n_data->num_destinations);
                    else
                        printf("No he trobat l'aeroport amb mes destinacions\n");

                }else{
                    str1[strlen(str1)-1]=0;

                    printf("\nMedian delay for the airport %s:\n", str1);

                    n_data = find_node(airports_tree, str1);

                    if(n_data){

                        list_item *l_item;

                        l_item = n_data->l->first;

                        i=0;
                        while(l_item != NULL){
                            i++;
                            float median;
                            l_data = l_item->data;

                            median = (float)l_data->delay/(float)l_data->num_flights;

                            printf("   %s --> %s, Median Delay: %.3f minutes\n", n_data->key,l_data->key, median);
                            l_item = l_item->next;

                        }
                    }
                }

                break;

            case 5:

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
