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

  /*
  * Now that the files were read, now we need to read the columns of the
  * dades.csv in order to obtain the especified data we need to use for
  * the program. In this case, the columns are:  15, 17, 18.
  * Each column has an especified data:
  *   - col. 15: Delay to the arriving airport, in minutes (int)
  *   - col. 17: Origin airport, IATA Code.
  *   - col. 18: Destiny airport, IATA Code.
  */

  char *column_15;
  char *column_17;
  char *column_18;

  for( int row_num = 0 ; row_num < len(dades_matrix); ++row_num){
    // First of all, I get the rows of the dades_matrix were I stored the data
    // of dades.csv.
    char *rows = dades_matrix[row_num];
    // Then, for the rows array, I get the value of the 15,17,18 columns and
    // store them in another array of chars that will be used to get the values.
    column_15[row_num] = rows[15];
    column_17[row_num] = rows[17];
    column_18[row_num] = rows[18];

  }

  





}
