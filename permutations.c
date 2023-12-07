/**
 *
 * Descripcion: Implementation of function that generate permutations
 *
 * File: permutations.c
 * Autor: Carlos Aguirre
 * Version: 1.1
 * Fecha: 21-09-2019
 *
 */

#include "permutations.h"
#include <stdlib.h>
#include <stdio.h>

void swap(int * a, int * b)
{
  int aux;

  /*Cambiamos a que elemento apunta cada puntero*/
  aux = *a;
  *a = *b;
  *b = aux;

}

/***************************************************/
/* Function: random_num Date:                      */
/* Authors:                                        */
/*                                                 */
/* Rutine that generates a random number           */
/* between two given numbers                       */
/*                                                 */
/* Input:                                          */
/* int inf: lower limit                            */
/* int sup: upper limit                            */
/* Output:                                         */
/* int: random number                              */
/***************************************************/
int random_num(int inf, int sup)
{
  int random;

  /*Control de errores*/
  if (sup < inf)
    return -1;

  /*No hace falta porque está declarado en exercise1.c
      //Inicia el generador de numeros aleatorios
       srand(time(NULL));
  */

  /*Genera un numero aleatorio este los limites superior e inferior*/
  /*int random = inf + rand() % (sup - inf + 1);*/

  random = inf + (int)((sup - inf + 1.0) * rand() / (RAND_MAX + 1.0));

  return random;
}

/***************************************************/
/* Function: generate_perm Date:                   */
/* Authors:                                        */
/*                                                 */
/* Rutine that generates a random permutation      */
/*                                                 */
/* Input:                                          */
/* int n: number of elements in the permutation    */
/* Output:                                         */
/* int *: pointer to integer array                 */
/* that contains the permitation                   */
/* or NULL in case of error                        */
/***************************************************/
int *generate_perm(int N)
{
  int *perm, i;

  /*Reserva memoria para el array*/
  perm = (int *)malloc(N * sizeof(int));
  if (!perm)
    return NULL;
  
  /*Rellenamos el array con numeros hasta N*/
  for (i = 0; i < N; i++)
    perm[i] = i + 1;

  /*Cambiamos de orden los elementos del array*/
  for (i = 0; i < N; i++)
    swap(&perm[i], &perm[random_num(i, N - 1)]);
  

  return perm;
}

/***************************************************/
/* Function: generate_permutations Date:           */
/* Authors:                                        */
/*                                                 */
/* Function that generates n_perms random          */
/* permutations with N elements                    */
/*                                                 */
/* Input:                                          */
/* int n_perms: Number of permutations             */
/* int N: Number of elements in each permutation   */
/* Output:                                         */
/* int**: Array of pointers to integer that point  */
/* to each of the permutations                     */
/* NULL en case of error                           */
/***************************************************/

int **generate_permutations(int n_perms, int N)
{
  int **perms, i;

  /*Reserva memoria para el array*/
  perms = (int **)malloc(n_perms * sizeof(int*));
  if (!perms)
    return NULL;

  /*Genera n_perms permutaciones*/
  for (i = 0; i < n_perms; i++)
  {
    perms[i] = generate_perm(N); 
    if(!perms[i])
      return NULL;
  }

  return perms;
}