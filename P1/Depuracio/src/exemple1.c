#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int *a;
  
  a = (int *) malloc(sizeof(int) * 10);
  
  printf("Abans assignacio\n");
  a[15] = 15; 
  printf("Despres assignacio\n");
  
  printf("Valor a[15]: %d\n", a[15]);
  
  free(a);
}

/* En la posicion a[15] no se puede almacenar el valor (15)
  porque no esta comprendida dentro del malloc de arriba
  y entonces el valor pasa a ser arbitrario. */