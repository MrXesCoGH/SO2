#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compara(const void *p1, const void *p2){

	/**
		The function compara does a comparation
		between one string and another and returns a value that
		indicates which string is "shorter" and which "longer" using
		the ascii value of them.
	*/
	char *str1, *str2;

	str1 = *((char **) p1);
	str2 = *((char **) p2);

	return strcmp(str1,str2);
	
}

int main(){
	char **str_matrix; //We create a char matrix where We'll store the strings.
		
	/*
	* We need to get the strings stored in the file. By the following process,
	* We'll get the strings of the files and storing them in an char array.
	*/

	FILE *fp;
	char str[100];

	fp = fopen("strings.txt","r");
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

	while(fgets(str,MIDA, fp) != NULL && i < MIDA){
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

	//After storing in the matrix the strings, We sort them using the qsort and the compara function We created.

	qsort(str_matrix, MIDA, sizeof(char **), compara);

	for(int g = 0; g < MIDA; g++){
		printf("%s ", str_matrix[g]);
		printf("\n");

		/*
		* After printing the strings, the vector position that stored the string needs
		* to be freed.
		*/
        free(str_matrix[g]);
	}

	// Finally, We free the rest of the allocated positions assigned by the first malloc.
	free(str_matrix);

}
