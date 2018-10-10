#include <stdio.h>
#include <stdlib.h>

int compara(const void *p1, const void *p2){

	/**
	* The function compara, it has been modified in order to compare the two
	* elements and returns 1 if the first element is lower than the second or
	* returns -1 in the opposite case. In other cases it returns 0. 
	*/

	double *num1, *num2;

	num1 = (double *) p1;
	num2 = (double *) p2;

	if(*num1 < *num2)
		return -1;
	else if(*num1 > *num2)
		return 1;
	else
		return 0;
}

int main(){
	double *dble; //This creates the vector where we'll be storing the doubles.

	/* 
	* Next, We read the file where there are the doubles
	* to sort and load them in a char array.
	*/
	
	FILE *fp;
	char str[100];

	fp = fopen("float.txt","r");
	if(fp == NULL){
		perror("Error al obrir el fitxer");
		return(-1);
	}

	if( fgets (str,100,fp) != NULL){
		puts(str);
	}
	
	int MIDA = atoi(str); //This gets the full size of the char array after the last read.
	dble = malloc(MIDA * sizeof(double)); // Using the size that We got before We allocate the vector created before.

	/*
	* Because in the char array there are loaded the numbers in string format they need to 
	* be transformed to doubles and, by using the atof function, we solve that problem.
	* After making the atof We store the doubles inside the dynamic vector in the i position.
	*/

	for(int i = 0;  i < MIDA; i++){
		if( fgets (str,100,fp) != NULL){
			puts(str);
		}
		dble[i] = atof(str);
	}

	fclose(fp);

	//Next, using the qsort function, with the vector, the size We got before, the size of doubles list and
	//a call to the compara function, We sort the doubles stored inside the dynamic vector.

	qsort(dble, MIDA, sizeof(double), compara);

	//After the sort We print the values in the dynamic vector.

	for(int g = 0; g < MIDA; g++){
		printf("%lf ", dble[g]);
		printf("\n");
	}
	
	//Finally, We need to free the dynamic memory that was reserved for the vector.

	free(dble);

}