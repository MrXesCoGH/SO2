#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "red-black-tree.h"
#include "linked-list.h"

#define MAXVALUE 10

//valgrind --leak-check=full ./practica2 dades.csv aeroports.csv LAX

//crear arbre
/**
 * Funció auxiliar per crear tots els nodes de l'arbre
 * 
 * Rep com a parametres un punter al arbre i un punter de char amb el nom del fitxer a llegir
 * 
 */

void crear_arbre(rb_tree *tree, char*  arg){
    
    char str[10]; 
    
    node_data *n_data;
    FILE* fp;

    fp= fopen(arg,"r");
    if (fp == NULL){
        printf("Fitxer d'aeroports erroni.");
        exit(1);
    }
    /* Llegim primera linea*/
    fgets(str,10,fp);
    

    /* --- Inserim els nodes en l'arbre --- */
    
    while(fgets(str,10,fp) != NULL){
        // Reservem strlen(str) per tindre lloc pel caracter final de linea a memoria 
        str[strlen(str) - 1] = '\0';
        n_data = malloc(sizeof(node_data));
        n_data->key = malloc( (strlen(str) +1)*sizeof(char));
        strcpy(n_data->key,str);
        n_data->num_destins = 0;
        n_data->l = (list *) malloc (sizeof(list));
        init_list(n_data->l);
        insert_node(tree, n_data);
    }
    fclose(fp);
}


/**
 * Insereix informacio de fitxer dades a l'arbre 
 * 
 * 
 */

void inserir_arbre(rb_tree *tree, char*  arg){

    char str[400]; 
    node_data *n_data;
    FILE *fp;
    
    fp = fopen(arg,"r");
    if (fp == NULL){
        printf("Fitxer de dades erroni.");
        exit(1);
    }
    /* Ignorem primera linea del fitxer */
    fgets(str,400,fp);

    while (fgets(str, 100, fp) != NULL) {
        
        int i = 0;
        int col = 1;
        int longitud=0;        
        char origin[4];
        char destination[4];
        char delaystring[10];
        
        memset(origin, '\0', 4);
        memset(destination, '\0', 4);
        memset(delaystring, '\0', 10);      
        while(i<strlen(str)){
            if (str[i] == ','){
                col++; 
                longitud = 0;
            }else{
                if (col == 15){
                    delaystring[longitud] = str[i];
                    longitud++;
                }
                else if (col == 17){
                    origin[longitud] = str[i];   
                    longitud++;
                }
                else if (col == 18){
                    destination[longitud] = str[i];
                    longitud++;
                }
            }
            i++;
        }

        //printf("%s,%s,%s\n", origin, destination, delaystring);
        list_data *l_data;
        //search tree node
        n_data = find_node(tree, origin);
        if (n_data) {
            //add elements to tree lists
            l_data = find_list(n_data->l, destination);
            if (l_data != NULL) {
            	//printf("add destination %s to origin airport list %s.\n", origin, destination);
            	l_data->num_times++; 
                if (delaystring[0] != 'N'){       
                	l_data->delay+= atoi(delaystring);
                }
            } else {
                /* If the key is not in the list, allocate memory for the data and
                * insert it in the list and add 1 to number of destinations of the node*/
                //printf("add destination %s to origin airport list %s.\n", origin, destination);         
                l_data = malloc(sizeof(list_data));
                
                l_data->key = malloc( (strlen(destination) +1)*sizeof(char));
                
                strcpy(l_data->key,destination);
                l_data->delay = 0;
                if (delaystring[0] != 'N'){               
                    l_data->delay = atoi(delaystring);
                }
                l_data->num_times = 1;
                insert_list(n_data->l, l_data);

                n_data->num_destins++;
            }
        }      
        memset(origin,'\0', strlen(origin));
        memset(destination, '\0', strlen(destination));
        memset(delaystring, '\0', strlen(delaystring));
        
    } 
    fclose(fp);
}

void estadistiques(rb_tree *tree, char*  arg){
	node_data *n_data;

    n_data = find_node(tree,"LAX");

    if (n_data){
    	printf("LAX found\n");
    	printf("%d\n",n_data->num_destins);
    } 
    //calcular estadistiques
    n_data = find_node(tree,arg);    
    if(n_data){
        printf("Aeroport %s trobat\n", n_data->key);
        
        list_item *l_item;
        list_data *l_data;
        l_item = n_data->l->first;
        
        int i = 0;
        while (l_item != NULL) {
            i++;
            float median;
            l_data = l_item->data;     
            median = (float)l_data->delay/(float)l_data->num_times;
            printf("    %s -- %.3f minuts .\n", n_data->key, median );
            // l_data->key, median, l_data->num_times, l
            l_item = l_item->next;
        }
    }
}

void max_destinations_arbre(rb_tree *tree){
	node_data *n_data;
    n_data = max_tree(tree);
    printf("Aeroport %s trobat amb %d destins\n", n_data->key, n_data->num_destins);
}


int main(int argc, char *argv[]){
	
    rb_tree *tree;
    /* Control d'arguments */
    if (argc != 4){
        printf("Arguments erronis: <dades.csv> <aeroports.csv> <origen>\n");
        exit(1);
    }

    tree = (rb_tree *) malloc(sizeof(rb_tree));

    init_tree(tree);
    
    /* Obrim fitxer d'aeroports, llegim dades i guardem a l'abre */
    crear_arbre(tree, argv[2]);

    /* Inserim dades en estructura arbre-llista */
    inserir_arbre(tree, argv[1]);

    /* estadistiques */
    estadistiques(tree, argv[3]);
    /* node amb mes destins */
    
    max_destinations_arbre(tree);

    delete_tree(tree);
    free(tree);
  return 0;
}
