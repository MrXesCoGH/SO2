#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "/arbre-binari/red-black-tree.h"
#include "/linked-list/linked-list.h"

void process(String aeroports_path, String dades_path, String origin){
  tree *aeroports_tree;
  node_data *n_data;
  /* Allocate memory for tree */
  aeroports_tree = (rb_tree *) malloc(sizeof(rb_tree));
  /* Initialize the tree */
  init_tree(aeroports_tree);

  // Reading file process.
  FILE *fp;
	char str[100];

	fp = fopen(aeroports_path,"r");
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
        int h = strlen(str);
		    //turn the \n to 0
    		str[h-1] = '\0';
        //This search if the str value is already inside the tree.
        n_data = find_node(aeroports_tree, str);

        if (n_data != NULL) {
          /* If the key is in the tree increment 'num' */
          n_data->num_vegades++;
        } else {

          list d_list;
          d_list = (list*) malloc(sizeof(list));
          init_list(d_list);

          /* If the key is not in the tree, allocate memory for the data
           * and insert in the tree */
          n_data = malloc(sizeof(node_data));
          /* This is the key by which the node is indexed in the tree */
          n_data->key = str;
          /* This is additional information that is stored in the tree */
          n_data->num_vegades = 1;
          n_data->list = d_list;
  	      insert_node(aeroports_tree, n_data);
	}

	fclose(fp);

	fp = fopen(dades_path,"r");
	if(fp == NULL){
		perror("Error al obrir el fitxer");
		return(-1);
	}

	if (fgets (str,100,fp) != NULL){
		puts(str);
	}

	int MIDA_DADES = atoi(str); //This int stores the total number of lines that We read.
	int i = 0; //This is just a simple counter used to iterate in the following loop.
  char **dades_list = malloc(MIDA_DADES*sizeof(char*));

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
        list_data l_data = malloc(sizeof(list_data)); //this creates the list item.
        //l_data is a tuple of the values of the columns 15,17,18 of the dades.csv
        //This creates a new item in the list where it'll store the tuple.
    	  insert_list(d_list,l_data);
	}

	fclose(fp);
}

int main(int argc, char **argv[]){

  /*
  * This gets the arguments entered when calling the program.
  * In this case the arguments are 4: program name, dades.csv, aeroports.csv and <origen>.
  * Anything else will trigger an error, for example, if the parameters
  * introduced are less than the needed will trigger the error of lacking param.
  */

  if(argc == 4){
    //This reads every file needed in order to get the data we're going to use.
    //argv[1]: aeroports.csv path
    //argv[2]: dades.csv path
    //argv[3]: origen
    process(argv[1], argv[2], argv[3]);

  }

  // In case there are less parameters than needed.
  else if (argc < 4){
    printf("ERR: Falten parametres");
  }

  // In case there are more parameters than needed.
  else{
    printf("ERR: Hi han parametres de mes");
  }

  /* Delete the tree */
  delete_tree(tree);
  free(tree);

  /* Delete the list */
  delete_list(l);
  free(l);







}
