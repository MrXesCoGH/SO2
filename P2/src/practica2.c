#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "/arbre-binari/red-black-tree.h"
#include "/linked-list/linked-list.h"

void lectura(String aeroports_path, String dades_path, rb_tree *a_tree, list *d_list){
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

	int MIDA = atoi(str); //This int stores the total number of lines that We read.
	int i = 0; //This is just a simple counter used to iterate in the following loop.

	while(fgets(str, MIDA, fp) != NULL && i < MIDA){
        //Here it's stored the length of the char array.
        int h = strlen(str);
		    //turn the \n to 0
    		str[h-1] = '\0';
        //This creates a new node in the tree where it'll store the string.
    	  insert_node(a_tree, str);
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

	int MIDA = atoi(str); //This int stores the total number of lines that We read.
	int i = 0; //This is just a simple counter used to iterate in the following loop.

	while(fgets(str, MIDA, fp) != NULL && i < MIDA){
        //Here it's stored the length of the char array.
        int h = strlen(str);
		    //turn the \n to 0
    		str[h-1] = '\0';
        //This creates a new item in the list where it'll store the tuple.
    	  insert_node(a_tree, str);
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

  rb_tree *aeroports_tree;
  list *dades_list;
  char *origen;

  if(argc == 4){
    /* Allocate memory for tree */
    aeroports_tree = (rb_tree *) malloc(sizeof(rb_tree));
    /* Initialize the tree */
    init_tree(aeroports_tree);

    dades_list = (list *) malloc(sizeof(dades_list));
    init_list(dades_list);

    //This reads every file needed in order to get the data we're going to use.
    lectura(argv[1], argv[2], &aeroports_tree, &dades_list);
    origen = argv[3]; //This is the origin aeroport especified by the user.
  }

  // In case there are less parameters than needed.
  else if (argc < 4){
    printf("ERR: Falten parametres");
  }

  // In case there are more parameters than needed.
  else{
    printf("ERR: Hi han parametres de mes");
  }

  /*
  * Now that the files were read, now we need to read the columns of the
  * dades.csv in order to obtain the especified data we need to use for
  * the program. In this case, the columns are:  15, 17, 18.
  * Each column has an especified data:
  *   - col. 15: Delay to the arriving airport, in minutes (int)
  *   - col. 17: Origin airport, IATA Code.
  *   - col. 18: Destiny airport, IATA Code.
  */


  /* Delete the tree */
  delete_tree(tree);
  free(tree);

  /* Delete the list */
  delete_list(l);
  free(l);







}
