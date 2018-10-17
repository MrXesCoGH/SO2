#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "arbre-binari/red-black-tree.h"
#include "linked-list/linked-list.h"

char *split_fn(char *cadena, int col){
    char *splited = malloc(sizeof(char)*50);
    int actual, columna = 0;
    
    for(int i= 0; i<strlen(cadena); i++){
        if(cadena[i] == ','){
            columna++;
            if(columna == col){
                return splited;                
            }else{
                strcpy(splited," ");                
            }            
        }
        else{
            actual = strlen(splited);
            splited[actual] = cadena[i];
            splited[actual+1] ='\0';
        }
        
    } 
    return splited;
}

int main(int argc, char **argv[]){

    /*
    * This gets the arguments entered when calling the program.
    * In this case the arguments are 4: program name, dades.csv, aeroports.csv and <origen>.
    * Anything else will trigger an error, for example, if the parameters
    * introduced are less than the needed will trigger the error of lacking param.
    */

    if(argc == 4){
        rb_tree *aeroports_tree;
        node_data *n_data;
        
        /* Allocate memory for tree */
        aeroports_tree = (rb_tree *) malloc(sizeof(rb_tree));
        /* Initialize the tree */
        init_tree(aeroports_tree);

        // Reading file process.
        FILE *fp;
        char str[100];
        char *s;

        fp = fopen((char*)argv[1],"r");
        if(fp == NULL){
            perror("Error al obrir el fitxer");
            return(-1);
        }

        if (fgets (str,100,fp) != NULL){
            puts(str);
        }

        int MIDA_AERO = atoi(str); //This int stores the total number of lines that We read.
        int i = 0; //This is just a simple counter used to iterate in the following loop.

        while(fgets(str, MIDA_AERO, fp) != NULL && i < MIDA_AERO){
            //Here it's stored the length of the char array.
            //int h = strlen(str);
            //turn the \n to 0
            //str[h-1] = '\0';
            
            s = malloc(sizeof(char)*4);
            strcpy(s, str);
            s[3] = '\0';
            
            //This search if the str value is already inside the tree.
            n_data = find_node(aeroports_tree, s);

            if (n_data != NULL) {
                /* If the key is in the tree increment 'num' */
                n_data->num_vegades++;
            } else {

                list* d_list;
                d_list = (list*) malloc(sizeof(list));
                init_list(d_list);

                /* If the key is not in the tree, allocate memory for the data
                * and insert in the tree */
                n_data = malloc(sizeof(node_data));
                n_data->l = malloc(sizeof(list));
                
                /* This is the key by which the node is indexed in the tree */
                n_data->key = str;
                /* This is additional information that is stored in the tree */
                n_data->num_vegades = 1;
                n_data->l = d_list;
                insert_node(aeroports_tree, n_data);
            }
        }

        fclose(fp);
        
        char *origin, *destination;
        int delay;

        fp = fopen((char*)argv[2],"r");
        if(fp == NULL){
            perror("Error al obrir el fitxer");
            return(-1);
        }

        if (fgets (str,100,fp) != NULL){
            puts(str);
        }

        int MIDA_DADES = atoi(str); //This int stores the total number of lines that We read.        
        i = 0;
        /*
        * Now that the dades file is opened, I need to read the columns.
        * In this case, the columns are:  15, 17, 18.
        * Each column has an especified data:
        *   - col. 15: Delay to the arriving airport, in minutes (int)
        *   - col. 17: Origin airport, IATA Code.
        *   - col. 18: Destiny airport, IATA Code.
        */
        
        

        while(fgets(str, MIDA_DADES, fp) != NULL && i < MIDA_DADES){
            //Here it's stored the length of the char array.
            int h = strlen(str);
            
            //turn the \n to 0
            str[h-1] = '\0';
            
            delay = atoi(split_fn(str,15));
            origin= split_fn(str,17);
            destination  = split_fn(str,18);      
            
            //this searchs if the node is already inside the tree.
            
            n_data = find_node(aeroports_tree, origin);
        
            if(n_data){
                //in case it's inside, then the number of times appeared is rised by 1
                n_data->num_vegades++;            
            }else{
                
                list* d_list;
                d_list = (list*) malloc(sizeof(list));
                init_list(d_list);

                /* If the key is not in the tree, allocate memory for the data
                * and insert in the tree */
                n_data = malloc(sizeof(node_data));
                n_data->l = malloc(sizeof(list));
                
                n_data = malloc(sizeof(node_data));
                n_data->key = origin;
                n_data->num_vegades = 1;
                n_data->l = d_list;
                
                insert_node(aeroports_tree,n_data);
            }
               
            list *lista = n_data->l;
            
            list_data *l_data= malloc(sizeof(list_data)); //this creates the list item
            
            l_data = find_list(lista,destination);
            
            //This creates a new item in the list where it'll store the tuple.
            insert_list(n_data->l,l_data);
        }

        fclose(fp);

        /* Delete the tree */
        delete_tree(aeroports_tree);
        free(aeroports_tree);
        
    }    

    // In case there are less parameters than needed.
    else if (argc < 4){
        printf("ERR: Falten parametres");
    }

    // In case there are more parameters than needed.
    else{
        printf("ERR: Hi han parametres de mes");
    }
    
    return 0;
}
