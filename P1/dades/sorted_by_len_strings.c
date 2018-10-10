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

	int value1, value2;
	char *str1, *str2;

	str1 = *((char **) p1);
	str2 = *((char **) p2);
    
    value1 = strlen(str1);
    value2 = strlen(str2);

	if (value1 < value2)
        return -1;
    else if(value1 > value2)
        return 1;
    else
        return 0;
	
}

int main(){
	char **str_matrix;  //We create a char matrix where We'll store the strings.
	
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
	
	str_matrix = malloc(MIDA * sizeof(char *)); //we make the first malloc using number of lines stored previously

	while(fgets(str,MIDA, fp) != NULL && i < MIDA){
        //Here it's stored the length of the string.
        int h = strlen(str); 								
		
		//This removes the \n at the end of the line.
		str[h-1] = '\0'; 									
		
		//this creates a row in the matrix where it'll store the string.
		str_matrix[i] = (char*) malloc(h*sizeof(char)); 

		//this copy the string inside the row of the matrix of chars
		strcpy(str_matrix[i],str);							
		
		i++; 
	}

	fclose(fp);

	//After storing in the matrix the strings, We sort them using the qsort and the compara function We created.

	qsort(str_matrix, MIDA, sizeof(char *), compara);

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
