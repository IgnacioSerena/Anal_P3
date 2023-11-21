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
/* Function: SelectSort    Date:                   */
/* Your comment                                    */
/***************************************************/
int SelectSort(int *array, int ip, int iu)
{
  int ob = 0;

  /*printf("%d %d\n", ip, iu);
  assert(ip < iu);*/
  /*Busca al mínimo elemento del array, lo cambia de posición
  con el primer elemento y reduce el tamaño del array*/
  while (ip < iu)
  {
    int m = min(array, ip, iu);
    swap(&array[ip], &array[m]);
    ip++;
    ob += iu - ip;
  }

  return ob;
}

int SelectSortInv(int *array, int ip, int iu)
{
  int ob = 0;

  /*Busca el elemento mínimo elemento del array, lo cambia de posición
  con el últmimo elemento y reduce el tamaño del array*/
  while (iu > ip)
  {
    int m = min(array, ip, iu);
    swap(&array[iu], &array[m]);
    iu--;
    ob += iu - ip;
  }

  return ob;
}

int min(int *array, int ip, int iu)
{
  /*Iguala privisionalmente el mínimo a la primera posición del array*/
  int min = ip, i;

  /*Compara todos los elementos del array con el mínimo y asigna un nuevo
  mínimo en caso de haberlo*/
  for (i = ip + 1; i <= iu; i++)
  {
    if (array[min] > array[i])
      min = i;
  }

  return min;
}

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
      ob++;
    }
    else
    {
      taux[index] = tabla[mid];
      mid++;
    }
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

int median(int *tabla, int ip, int iu, int *pos)
{
  *pos = ip;

  return 0;
}

int median_avg(int *tabla, int ip, int iu, int *pos)
{
  *pos = (ip + iu) / 2;

  return 0;
}

int median_stat(int *tabla, int ip, int iu, int *pos)
{
  int medio = (ip + iu) / 2;

  if (((tabla[ip] <= tabla[medio]) && (tabla[medio] <= tabla[iu])) ||
      ((tabla[iu] <= tabla[medio]) && (tabla[medio] <= tabla[ip])))
    *pos = medio;

  else if (((tabla[medio] <= tabla[ip]) && (tabla[ip] <= tabla[iu])) ||
           ((tabla[iu] <= tabla[ip]) && (tabla[ip] <= tabla[medio])))
    *pos = ip;

  else
    *pos = iu;

  return 3;
}
/*
int median_stat(int *tabla, int ip, int iu, int *pos)
{
  int medio = (ip + iu) / 2;

  if ((tabla[ip] <= tabla[medio]) && (tabla[medio] <= tabla[iu]))
  {
    if ((tabla[iu] <= tabla[medio]) && (tabla[medio] <= tabla[ip]))
      *pos = medio;
  }

  else if ((tabla[medio] <= tabla[ip]) && (tabla[ip] <= tabla[iu]))
  {
    if ((tabla[iu] <= tabla[ip]) && (tabla[ip] <= tabla[medio]))
      *pos = ip;
  }

  else
    *pos = iu;

  return 3;
}
*/
int partition(int *tabla, int ip, int iu, int *pos)
{
  int k, i = 0, ob = 0;

  ob = median(tabla, ip, iu, pos);

  k = tabla[*pos];
  swap(&tabla[ip], &tabla[*pos]);
  *pos = ip;

  for (i = ip + 1; i <= iu; i++)
  {
    if (tabla[i] < k)
    {
      (*pos)++;
      swap(&tabla[i], &tabla[*pos]);
    }
  }

  swap(&tabla[ip], &tabla[*pos]);
  ob += i;

  return ob;
}

int quicksort(int *tabla, int ip, int iu)
{
  int *pos = NULL, ob = 0;

  pos = (int *)malloc(sizeof(int));
  if (!pos)
  {
    fprintf(stderr, "ERROR DE RESERVA DE MEMORIA\n");
    return -1;
  }

  if (ip > iu)
  {
    free(pos);
    return ERR;
  }
  else if (ip == iu)
  {
    free(pos);
    return 0;
  }
  else
  {
    ob += partition(tabla, ip, iu, pos);
    if (ip < *pos - 1)
      ob += quicksort(tabla, ip, *pos);
    else if (*pos + 1 < iu)
      ob += quicksort(tabla, *pos + 1, iu);
  }

  free(pos);

  return ob;
}
