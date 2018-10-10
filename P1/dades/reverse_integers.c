#include <stdio.h>
#include <stdlib.h>

int compara(const void *p1, const void *p2){
	/**
	* The function compara, it has been modified in order to compare the two
	* elements and return 1 if the first element is lower than the second or
	* return -1 in the opposite case. In other cases it returns 0. 
	*/
	
	int *num1, *num2;

	num1 = (int *) p1;
	num2 = (int *) p2;

	if(*num1 < *num2)
		return 1;
	else if(*num1 > *num2)
		return -1;
	else
		return 0;
}

int main(){
	int *inters; //This creates the vector where we'll be storing the integers.
	
	/* 
	* Next, We read the file where there are the ints
	* to sort and load them in a char array.
	*/

	FILE *fp;
	char str[100];

	fp = fopen("integers.txt","r");
	if(fp == NULL){
		perror("Error al obrir el fitxer");
		return(-1);
	}

	if( fgets (str,100,fp) != NULL){
		puts(str);
	}
	
	int MIDA = atoi(str); //This gets the full size of the char array after the last read.
	inters = malloc(MIDA * sizeof(int)); // Using the size that We got before We allocate the vector created before.

	/*
	* Because in the char array there are loaded the integers in string format they need to 
	* be transformed to integers and, by using the atoi function, we solve that problem.
	* After making the atoi We store the integer inside the dynamic vector in the i position.
	*/

	for(int i = 0;  i < MIDA; i++){
		if( fgets (str,100,fp) != NULL){
			puts(str);
		}
		inters[i] = atoi(str); 
	}


	fclose(fp);

	//Next, using the qsort function, with the vector, the size We got before, the size of ints and
	//a call to the compara function, We sort the integers inside the dynamic vector.

	qsort(inters, MIDA, sizeof(int), compara);

	//After the sort We print the values in the dynamic vector.

	for(int g = 0; g < MIDA; g++){
		printf("%d ", inters[g]);
		printf("\n");
	}

	//Finally, We need to free the dynamic memory that was reserved for the vector.
	free(inters);
	

}

	

	