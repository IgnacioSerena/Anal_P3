/**
 *
 * Descripcion: Implementation of time measurement functions
 *
 * Fichero: times.c
 * Autor: Carlos Aguirre Maeso
 * Version: 1.0
 * Fecha: 16-09-2019
 *
 */

#include "times.h"
#include "sorting.h"
#include "permutations.h"
#include <assert.h>

/***************************************************/
/* Function: average_sorting_time Date:            */
/*                                                 */
/* Your documentation                              */
/***************************************************/
short average_sorting_time(pfunc_sort metodo, int n_perms, int N, PTIME_AA ptime)
{
  /* Generar las permutaciones*/
  int **permutations = generate_permutations(n_perms, N), i;

  /* Inicializar las variables de tiempo*/
  int min_ob = INT_MAX;
  int max_ob = 0;
  double total_ob = 0;

  /* Medir el tiempo de ejecución y contar las veces que se ejecuta la OB*/
  clock_t ini, fin;
  
  ini = clock();

  for (i = 0; i < n_perms; i++)
  {
    int ob_actual = 0;

    ob_actual = metodo(permutations[i], 0, N - 1);
    /*assert(ob_actual > 0);*/
    if (ob_actual == ERR)
      return ERR;

    total_ob += ob_actual;

    if (ob_actual < min_ob)
      min_ob = ob_actual;

    if (ob_actual > max_ob)
      max_ob = ob_actual;
  }

  fin = clock();

  /*Asignamos valores a la estructura*/
  ptime->N = N;
  ptime->n_elems = n_perms;
  ptime->time = (double)(fin - ini) / (n_perms * CLOCKS_PER_SEC);
  ptime->average_ob = total_ob / n_perms;
  ptime->min_ob = min_ob;
  ptime->max_ob = max_ob;

  /* Liberar la memoria de las permutaciones*/
  for (i = 0; i < n_perms; i++)
  {
    free(permutations[i]);
  }
  free(permutations);

  return OK;
}

/***************************************************/
/* Function: generate_sorting_times Date:          */
/*                                                 */
/* Your documentation                              */
/***************************************************/
short generate_sorting_times(pfunc_sort method, char *file, int num_min, int num_max, int incr, int n_perms)
{
  int num, i, n_times;
  PTIME_AA times;

  n_times = (num_max - num_min) / incr + 1;

  /*Reservamos memoria para un array de estructuras*/
  times = (PTIME_AA)malloc(n_times * sizeof(TIME_AA));
  if (times == NULL)
  {
    perror("Error al asignar memoria para times");
    return ERR;
  }

  /*Este bucle recorre todos los valores a permutar, desde el num_min al num_max,con el incremento indicado*/
  for (num = num_min, i = 0; num <= num_max; num += incr, i++)
  {
    short resultado = average_sorting_time(method, n_perms, num, &times[i]);

    if (resultado == ERR)
    {
      free(times);
      return ERR;
    }

  }
  
  /*Guardo en un fichero los datos del times*/
  save_time_table(file, times, n_times);

  /* Liberar la memoria asignada para los resultados de tiempo*/
  free(times);
  return OK;
}

/***************************************************/
/* Function: save_time_table Date:                 */
/*                                                 */
/* Your documentation                              */
/***************************************************/
short save_time_table(char *file, PTIME_AA ptime, int n_times)
{
  FILE *fp = fopen(file, "w");
  int i;

  if (fp == NULL)
  {
    perror("Error al abrir el archivo");
    return ERR;
  }

  /*Imprimimos en el fichero los datos del array times*/
  for (i = 0; i < n_times; i++)
    fprintf(fp, "%d %f %f %d %d\n", ptime[i].N, ptime[i].time, ptime[i].average_ob, ptime[i].max_ob, ptime[i].min_ob);

  fclose(fp);

  return OK;
}
