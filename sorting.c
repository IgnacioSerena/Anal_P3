/**
 *
 * Descripcion: Implementation of sorting functions
 *
 * Fichero: sorting.c
 * Autor: Carlos Aguirre
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */

#include "sorting.h"
#include "permutations.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>




/***************************************************/
/* Function: merge Date: 8-11-2023                 */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que combina los elementos de la tabla y  */
/* ordena los elementos de una tabla de menor a    */
/* mayor                                           */
/*                                                 */
/* Input:                                          */
/* int* tabla: tabla que queremos ordenar          */
/* ip: primera posición a partir de la cual se     */
/* ordena el array                                 */
/* iu: última posición hasta la que se ordena el   */
/* array                                           */
/* imedio: índice del punto medio de la tabla      */
/*                                                 */
/* Output:                                         */
/* int: número de operaciones básicas que se       */
/* ejecutan en la funcion or ERR en caso de error  */
/***************************************************/
int merge(int *tabla, int ip, int iu, int imedio)
{
  int first = ip, mid = imedio + 1, index = 0, i, ob = 0, *taux;

  taux = (int *)malloc((iu - ip + 1) * sizeof(int));
  if (!taux)
    return ERR;

  while (first <= imedio && mid <= iu)
  {
    if (tabla[first] < tabla[mid])
    {
      taux[index] = tabla[first];
      first++;
    }
    else
    {
      taux[index] = tabla[mid];
      mid++;
    }
    ob++;
    index++;
  }

  if (first > imedio)
  {
    while (mid <= iu)
    {
      taux[index] = tabla[mid];
      mid++;
      index++;
    }
  }
  else if (mid > iu)
  {
    while (first <= imedio)
    {
      taux[index] = tabla[first];
      first++;
      index++;
    }
  }

  for (i = ip, index = 0; i <= iu; i++, index++)
  {
    tabla[i] = taux[index];
  }

  free(taux);

  return ob;
}

/***************************************************/
/* Function: mergeSort Date: 8-11-2023             */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que ordena de menor a mayor una seccion  */
/* del array delimitada por ip e iu                */
/*                                                 */
/* Input:                                          */
/* int* tabla: tabla que queremos ordenar          */
/* ip: primera posición a partir de la cual se     */
/* ordena el array                                 */
/* iu: última posición hasta la que se ordena el   */
/* array                                           */
/*                                                 */
/* Output:                                         */
/* int: número de operaciones básicas que se       */
/* ejecutan en la funcion o ERR caso de error      */
/***************************************************/
int mergesort(int *tabla, int ip, int iu)
{
  int mid, ob = 0;

  if (ip >= iu || !tabla)
    return 0;

  mid = (ip + iu) / 2;

  ob += mergesort(tabla, ip, mid);
  ob += mergesort(tabla, mid + 1, iu);

  ob += merge(tabla, ip, iu, mid);

  return ob;
}

/*/***************************************************/
/* Function: median Date: 15-11-2023               */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/* Rutina que devuelve el valor de ip en el        */
/* argumento pos                                   */
/*                                                 */
/*                                                 */
/* Input:                                          */
/* int* tabla: tabla que queremos ordenar          */
/* ip: primera posición a partir de la cual se     */
/* ordena el array                                 */
/* iu: última posición hasta la que se ordena el   */
/* array                                           */
/* pos: posición a la que introducir el valor      */
/* correspondiente                                 */
/* Output: 0                                       */
/*                                                 */
/*                                                 */
/***************************************************/
int median(int *tabla, int ip, int iu, int *pos)
{
  *pos = ip;

  return 0;
}

/***************************************************/
/* Function: median_avg Date: 15-11-2023           */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que asigna a pos la posición media entre */
/* ip e iu                                         */
/*                                                 */
/* Input:                                          */
/* int* tabla: tabla que queremos ordenar          */
/* ip: primera posición a partir de la cual se     */
/* ordena el array                                 */
/* iu: última posición hasta la que se ordena el   */
/* array                                           */
/* pos: posición a la que introducir el valor      */
/* correspondiente                                 */
/* Output: 0                                       */
/*                                                 */
/*                                                 */
/***************************************************/
int median_avg(int *tabla, int ip, int iu, int *pos)
{
  *pos = (ip + iu) / 2;

  return 0;
}



/***************************************************/
/* Function: median_stat Date: 16-11-2023          */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que elige el valor medio entre 3         */
/* posiciones                                      */
/* de memoria y delvuelve la posicion de valor     */
/* intermedio                                      */
/* Input:                                          */
/* int* tabla: tabla que queremos ordenar          */
/* ip: primera posición a partir de la cual se     */
/* ordena el array                                 */
/* iu: última posición hasta la que se ordena el   */
/* array                                           */
/* pos: posición a la que introducir el valor      */
/* correspondiente                                 */
/* Output: 3                                       */
/*                                                 */
/*                                                 */
/***************************************************/
int median_stat(int *tabla, int ip, int iu, int *pos)
{
  int medio = (ip + iu) / 2;

  if (tabla[ip] < tabla[medio])
  {
    if (tabla[ip] > tabla[iu]){
      (*pos) = ip;
    }

    else if (tabla[iu] > tabla[medio]){
      (*pos) = iu;
    }
    else{
      (*pos) = medio;
    }
    return 3;
  }

  else{

    if(tabla[ip] < tabla[iu])
      (*pos) = ip;

    else if (tabla[medio] < tabla[iu])
      (*pos) = iu;

    else 
      (*pos) = medio;

    return 3;
  }

  return 3;
}


/***************************************************/
/* Function: partition  Date: 15-11-2023           */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que parte y ordena la tabla de menor a   */
/* mayor del array delimitada por ip e iu          */
/*                                                 */
/* Input:                                          */
/* int* tabla: tabla que queremos ordenar          */
/* int ip: primera posición a partir de la cual se */
/* ordena el array                                 */
/* int iu: última posición hasta la que se ordena  */
/* el array                                        */
/* int* pos: posición del pivote                   */
/*                                                 */
/* Output:                                         */
/* int: número de operaciones básicas que se       */
/* ejecutan en la funcion o ERR caso de error      */
/***************************************************/
int partition(int *tabla, int ip, int iu, int *pos)
{
  int k, i = 0, ob = 0;

  ob += median_stat(tabla, ip, iu, pos);
  k = tabla[*pos];
  swap(&tabla[ip], &tabla[(*pos)]);
  (*pos) = ip;

  for (i = ip + 1; i <= iu; i++)
  {
    ob++;
    if (tabla[i] < k)
    {
      (*pos)++;
      swap(&tabla[i], &tabla[(*pos)]);
    }
  }

  swap(&tabla[ip], &tabla[(*pos)]);

  return ob;
}


/***************************************************/
/* Function: quickSort Date: 15-11-2023            */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que ordena de menor a mayor una seccion  */
/* del array delimitada por ip e iu                */
/*                                                 */
/* Input:                                          */
/* int* tabla: tabla que queremos ordenar          */
/* ip: primera posición a partir de la cual se     */
/* ordena el array                                 */
/* int iu: última posición hasta la que se ordena  */
/* el array                                        */ 
/*                                                 */
/* Output:                                         */
/* int: número de operaciones básicas que se       */
/* ejecutan en la funcion o ERR caso de error      */
/***************************************************/
int quicksort(int *tabla, int ip, int iu)
{
  int pos = 0, ob = 0;


  if (ip > iu)
  {
    return ERR;
  }

  if (ip == iu)
  {
    return 0;
  }
   
  ob += partition(tabla, ip, iu, &pos);
  
  if (ip < pos - 1){
    ob += quicksort(tabla, ip, pos - 1);
  }

  if (pos + 1 < iu){
    ob += quicksort(tabla, pos + 1, iu);
  }

  return ob;
}