#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arbre-binari/red-black-tree.h>
#include <linked-list/linked-list.h>

void lectura(String path){
   //The matrix where is going to be stored the data from the path param.
  char **str_matrix;

  // Reading file process.
  FILE *fp;
	char str[100];

	fp = fopen(path,"r");
	if(fp == NULL){
		perror("Error al obrir el fitxer");
		return(-1);
	}

	if (fgets (str,100,fp) != NULL){
		puts(str);
	}

	int MIDA = atoi(str); //This int stores the total number of lines that We read.
	int i = 0; //This is just a simple counter used to iterate in the following loop.

	str_matrix = malloc(MIDA * sizeof(char *)); //We make the first malloc using number of lines stored previously.

	while(fgets(str, MIDA, fp) != NULL && i < MIDA){
        //Here it's stored the length of the char array.
        int h = strlen(str);

		    //turn the \n to 0
    		str[h-1] = '\0';

        //This creates a row in the matrix where it'll store the string.
    		str_matrix[i] = (char*) malloc(h*sizeof(char));

        //This copy the string inside the row of the matrix of chars created before.
    		strcpy(str_matrix[i],str);
    		i++;
	}

	fclose(fp);

  return str_matrix;
}

int main(int argc, char **argv[]){

  /*
  * This gets the arguments entered when calling the program.
  * In this case the arguments are 4: program name, dades.csv, aeroports.csv and <origen>.
  * Anything else will trigger an error, for example, if the parameters
  * introduced are less than the needed will trigger the error of lacking param.
  */

  char **aeroports_matrix;
  char **dades_matrix;
  char *origen;

  if(argc == 4){
      aeroports_matrix = lectura(argv[1]); //This has the matrix of the aeroports.csv
      dades_matrix = lectura(argv[2]); //This has the matrix of the dades.csv
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




}
