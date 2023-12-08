/**
 *
 * Description: Implementation of functions for search
 *
 * File: search.c
 * Author: Carlos Aguirre and Javier Sanz-Cruzado
 * Version: 1.0
 * Date: 14-11-2016
 *
 */

#include "search.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

/**
 *  Key generation functions
 *
 *  Description: Receives the number of keys to generate in the n_keys
 *               parameter. The generated keys go from 1 to max. The
 * 				 keys are returned in the keys parameter which must be
 *				 allocated externally to the function.
 */

/**
 *  Function: uniform_key_generator
 *               This function generates all keys from 1 to max in a sequential
 *               manner. If n_keys == max, each key will just be generated once.
 */
void uniform_key_generator(int *keys, int n_keys, int max)
{
  int i;

  for (i = 0; i < n_keys; i++)
    keys[i] = 1 + (i % max);

  return;
}

/**
 *  Function: potential_key_generator
 *               This function generates keys following an approximately
 *               potential distribution. The smaller values are much more
 *               likely than the bigger ones. Value 1 has a 50%
 *               probability, value 2 a 17%, value 3 the 9%, etc.
 */
void potential_key_generator(int *keys, int n_keys, int max)
{
  int i;

  for (i = 0; i < n_keys; i++)
  {
    keys[i] = .5 + max / (1 + max * ((double)rand() / (RAND_MAX)));
  }

  return;
}

PDICT init_dictionary(int size, char order)
{
  PDICT pdict = (PDICT)malloc(sizeof(DICT));
  if (pdict == NULL)
    return NULL;

  pdict->size = size;
  pdict->n_data = 0;
  pdict->order = order;

  pdict->table = (int *)malloc(size * sizeof(int));
  if (pdict->table == NULL)
  {
    free(pdict);
    return NULL;
  }

  return pdict;
}

void free_dictionary(PDICT pdict)
{
  free(pdict->table);
  free(pdict);
}

int insert_dictionary(PDICT pdict, int key)
{
  int j;

  if (pdict->order == SORTED)
  {
    j = pdict->n_data;

    while (j >= 0 && pdict->table[j] > key)
    {
      pdict->table[j + 1] = pdict->table[j];
      j--;
    }

    pdict->table[j + 1] = key;
  }
  else
  {
    if (pdict->n_data < pdict->size)  
    {
      pdict->table[pdict->n_data] = key;
      ++pdict->n_data;
    }
    else
      return ERR; 
  }

  return OK;
}


int massive_insertion_dictionary(PDICT pdict, int *keys, int n_keys)
{
  int i;

  for (i = 0; i < n_keys; ++i)
  {
    if (insert_dictionary(pdict, keys[i]) != 0)
      return ERR;
  }

  return OK;
}

/***************************************************/
/* Function: search_dictionary Date: 22-11-2023    */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que buscara una clave en el diccionario  */
/* definido por pdict usando la rutina indicada    */
/* por method                                      */
/*                                                 */
/* Input:                                          */
/* PDICT pdict: estructura diccionario             */
/* int key: elemento a buscar                      */
/* int *ppos: puntero a la posición del diccionario*/
/* pfunc_search method: funcion con algoritmo de   */
/* ordenacion                                      */
/*                                                 */
/* Output:                                         */
/* int: número de operaciones básicas que se       */
/* ejecutan en la funcion or ERR en caso de error  */
/***************************************************/
int search_dictionary(PDICT pdict, int key, int *ppos, pfunc_search method)
{
  return method(pdict->table, 0, pdict->n_data - 1, key, ppos);
}

/* Search functions of the Dictionary ADT */

/***************************************************/
/* Function: bin_search Date: 22-11-2023           */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que buscara una clave en una tabla       */
/* usando el algoritmo de busqueda binaria         */
/*                                                 */
/* Input:                                          */
/* int *table: estructura diccionario              */
/* int F: primer elemento de la tabla              */
/* int L: ultimo elemento de la tabla              */
/* int key: elemento a buscar                      */
/* int *ppos: puntero a la posición del diccionario*/
/* pfunc_search method: funcion con algoritmo de   */
/* ordenacion                                      */
/*                                                 */
/* Output:                                         */
/* int: número de operaciones básicas que se       */
/* ejecutan en la funcion or ERR en caso de error  */
/***************************************************/
int bin_search(int *table, int F, int L, int key, int *ppos)
{
  int mid, ob = 0, flag = 0;

  if (F <= L)
  {
    mid = F + (L - F) / 2;

    if (table[mid] == key)
    {
      flag = 1;
      *ppos = mid;
      return ob++;
    }
    else if (table[mid] < key)
    {
      ob++;
      return bin_search(table, mid + 1, L, key, ppos);
    }
    else
    {
      ob++;
      return bin_search(table, F, mid - 1, key, ppos); 
    }
  }

  if(flag == 0)
    *ppos = NOT_FOUND;

  return ob;
}

/***************************************************/
/* Function: lin_search Date: 22-11-2023           */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que buscara una clave en una tabla       */
/* usando el algoritmo de busqueda lineal          */
/*                                                 */
/* Input:                                          */
/* int *table: estructura diccionario              */
/* int F: primer elemento de la tabla              */
/* int L: ultimo elemento de la tabla              */
/* int key: elemento a buscar                      */
/* int *ppos: puntero a la posición del diccionario*/
/* pfunc_search method: funcion con algoritmo de   */
/* ordenacion                                      */
/*                                                 */
/* Output:                                         */
/* int: número de operaciones básicas que se       */
/* ejecutan en la funcion or ERR en caso de error  */
/***************************************************/
int lin_search(int *table, int F, int L, int key, int *ppos)
{
  int i, ob = 0;

  for (i = F; i <= L; i++, ob++)
  {
    if (table[i] == key)
    {
      *ppos = i;
      return ob;
    }
  }

  *ppos = NOT_FOUND;

  return ob;
}

/***************************************************/
/* Function: lin_auto_search Date: 22-11-2023      */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que buscara una clave en una tabla       */
/* usando el algoritmo de busqueda lineal          */
/*                                                 */
/* Input:                                          */
/* int *table: estructura diccionario              */
/* int F: primer elemento de la tabla              */
/* int L: ultimo elemento de la tabla              */
/* int key: elemento a buscar                      */
/* int *ppos: puntero a la posición del diccionario*/
/* pfunc_search method: funcion con algoritmo de   */
/* ordenacion                                      */
/*                                                 */
/* Output:                                         */
/* int: número de operaciones básicas que se       */
/* ejecutan en la funcion or ERR en caso de error  */
/***************************************************/
int lin_auto_search(int *table, int F, int L, int key, int *ppos)
{
  if (F <= L)
  {
    if (table[F] == key)
    {
      *ppos = F;
      return 1;
    }
    else
    {
      return lin_auto_search(table, F + 1, L, key, ppos);
    }
  }

  *ppos = -1;
  return 0;
}
