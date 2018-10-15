#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arbre-binari/red-black-tree.h>
#include <linked-list/linked-list.h>

void lectura(String path){
  char **str_matrix;

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

int main(int argc, char *argv[]){

  /*
  * This gets the arguments entered when calling the program.
  * In this case the arguments are 3: dades.csv, aeroports.csv and <origen>
  */

  if(argc == 3){
      aeroports_matrix = lectura(argv[0]);
      dades_matrix = lectura(argv[1]);
      origen = argv[2];
  }

}
