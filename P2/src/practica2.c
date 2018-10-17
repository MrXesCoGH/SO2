#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "arbre-binari/red-black-tree.h"
#include "linked-list/linked-list.h"


/*Splitting function that collects the strings from the chain*/
char *split_fn(char *chain, int col){
    char *splited = malloc(sizeof(char)*50);
    int actual, column = 0;
   	
   	/*
   	* For all the chain, it search for the commas, in case of finding one,
   	* then it goes to the next position but if the actual position is equals
   	* to the parameter one, then it returns the string that was between the commas. 
   	*/
    for(int i= 0; i<strlen(chain); i++){
        if(chain[i] == ','){
            column++;
            if(column == col){
                return splited;                
            }else{
                strcpy(splited," ");                
            }            
        }
        else{
            actual = strlen(splited);
            splited[actual] = chain[i];
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
        rb_tree *airports_tree;
        node_data *n_data;
        
        /* Allocate memory for tree */
        airports_tree = (rb_tree *) malloc(sizeof(rb_tree));
        /* Initialize the tree */
        init_tree(airports_tree);

        //This integers acts as a boolean
        int appeared = 0;
        
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

        int i = 0; //This is just a simple counter used to iterate in the following loop.

        while(fgets(str, 5, fp) != NULL && i < 5){
            //Here it's stored the length of the char array.
            
            s = malloc(sizeof(char)*4);
            strcpy(s, str);
            s[3] = '\0';
            
            //This search if the str value is already inside the tree.
           	if(!appeared){
           		appeared = 1;
            } else {

                list* d_list;
                d_list = (list*) malloc(sizeof(list));
                init_list(d_list);

                /* If the key is not in the tree, allocate memory for the data
                * and insert in the tree */
                n_data = malloc(sizeof(node_data));
                n_data->l = d_list;
                
                /* This is the key by which the node is indexed in the tree */
                n_data->key = str;
                /* This is additional information that is stored in the tree */
                n_data->num_destinations = 1;
                insert_node(airports_tree, n_data);
            }
        }

        fclose(fp);
        
        char *origin, *destination, *delay;
        float delay_f;
        list_data *l_data;

        fp = fopen((char*)argv[2],"r");
        if(fp == NULL){
            perror("Error al obrir el fitxer");
            return(-1);
        }

        if (fgets (str,100,fp) != NULL){
            puts(str);
        }

        int DATA_SIZE = atoi(str); //This int stores the total number of lines that We read.        
        i = 0;
        
        /*
        * Now that the dades file is opened, I need to read the columns.
        * In this case, the columns are:  15, 17, 18.
        * Each column has an especified data:
        *   - col. 15: Delay to the arriving airport, in minutes (int)
        *   - col. 17: Origin airport, IATA Code.
        *   - col. 18: Destiny airport, IATA Code.
        */
        
        while(fgets(str, DATA_SIZE, fp) != NULL && i < DATA_SIZE){
            //Here it's stored the length of the char array.
            int h = strlen(str);
            
            //turn the \n to 0
            str[h-1] = '\0';
            
            //this uses the splitting function to obtain the values.
            delay = split_fn(str,15);
            origin= split_fn(str,17);
            destination  = split_fn(str,18);      
            
            //this searchs if the node is already inside the tree.
            
            n_data = find_node(airports_tree, origin);
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
        
        fclose(fp);
        
        printf("Median delay for the airport %s:\n", (char*)argv[3]);

        n_data = find_node(airports_tree, (char*)argv[3]);
        list_item *l_item = n_data->l->first;
        
        while(l_item){
            l_data = l_item->data;
            printf("   %s --> %.3f minutes\n", l_data->key, l_data->delay/l_data->num_flights);
            l_item = l_item->next;
            
        }

        n_data = search(airports_tree);
        printf("Airport with more destinations: %s, number: %d", n_data->key, n_data->num_destinations);

        /* Delete the tree */
        delete_tree(airports_tree);
        free(airports_tree);
        
    }    

    // In case there are less parameters than needed.
    else if (argc < 4){
        printf("ERR: There are lacking arguments");
    }

    // In case there are more parameters than needed.
    else{
        printf("ERR: There are more arguments than needed");
    }
    
    return 0;
}
