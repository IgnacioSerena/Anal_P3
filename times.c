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
#include "permutations.h"
#include <assert.h>

short average_search_time(pfunc_search metodo, pfunc_key_generator generator, int order, int N, int n_times, PTIME_AA ptime)
{
    PDICT pdict;
    int *permutation = NULL, *keys_to_search = NULL;
    clock_t start_time, end_time;
    int total_ob = 0;
    int min_ob = N + 1;
    int max_ob = 0;
    int i, ob, pos;

    if (N <= 0 || !ptime)
        return ERR;

    /*Creamos el diccionario*/
    pdict = init_dictionary(N, order);
    if (!pdict)
        return ERR;

    /*Creamos la permutacion*/
    permutation = generate_perm(N);
    if (!permutation)
    {
        free_dictionary(pdict);
        return ERR;
    }

    /*Insercion de elementos*/
    if (massive_insertion_dictionary(pdict, permutation, N) == ERR)
    {
        free(permutation);
        free_dictionary(pdict);
        return ERR;
    }

    /*Reserva de memoria para la tabla de busqueda*/
    keys_to_search = (int *)malloc(sizeof(int) * n_times * N);
    if (!keys_to_search)
    {
        free(permutation);
        free_dictionary(pdict);
        return ERR;
    }

    /*Llenado de tabla*/
    generator(keys_to_search, n_times * N, N);

    /*Medición de tiempos*/
    start_time = clock();

    for (i = 0; i < n_times * N; i++)
    {
        ob = search_dictionary(pdict, keys_to_search[i], &pos, metodo);
        total_ob += ob;
        if (ob < min_ob)
        {
            min_ob = ob;
        }
        if (ob > max_ob)
        {
            max_ob = ob;
        }
    }

    end_time = clock();

    /*Rellenamos la estructura ptime.*/
    ptime->N = N;
    ptime->n_elems = n_times * N;
    ptime->time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    ptime->average_ob = (double)total_ob / (n_times * N);
    ptime->min_ob = min_ob;
    ptime->max_ob = max_ob;

    /*Liberar memoria*/
    free(keys_to_search);
    free(permutation);
    free_dictionary(pdict);

    return OK;
}

short generate_search_times(pfunc_search metodo, pfunc_key_generator generator, int order, char *file, int num_min, int num_max, int incr, int n_times)
{

    int i;
    PTIME_AA ptime_array;

    if (!metodo || !generator || !file || num_min < 0 || num_max < 0 || incr <= 0 || n_times <= 0)
        return ERR;

    ptime_array = (PTIME_AA)malloc(sizeof(TIME_AA) * n_times);
    if (!ptime_array)
        return ERR;

    for (i = 0; i < n_times; i++)
    {
        PTIME_AA ptime = (PTIME_AA)malloc(sizeof(TIME_AA));
        if (!ptime)
        {
            free(ptime_array);
            return ERR;
        }

        if (average_search_time(metodo, generator, order, num_min, n_times, ptime) != OK)
        {
            free(ptime_array);
            free(ptime);
            return ERR;
        }

        ptime_array[i] = *ptime;
        free(ptime);
    }

    if (save_time_table(file, ptime_array, n_times) != OK)
    {
        free(ptime_array);
        return ERR;
    }

    free(ptime_array);
    return OK;
}

/***************************************************/
/* Function: average_sorting_time Date: 18-10-2023 */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que imprime en un fichero los datos de   */
/* la estructura ptime                             */
/*                                                 */
/* Input:                                          */
/* char* file: fichero donde se guardaran los      */
/* resultados                                      */
/* PTIME_AA ptime: estructura donde de estan los   */
/* datos a imprimir                                */
/* n_times: el número de permutaciones distintas   */
/*                                                 */
/* Output:                                         */
/* short: OK en caso de que todo funcione          */
/* correctamente o ERR en caso de error            */
/***************************************************/
short save_time_table(char *file, PTIME_AA ptime, int N)
{
    FILE *fp = fopen(file, "w");
    int i;
    if (!file || !ptime || N < 0)
        return ERR;
    if (fp == NULL)
    {
        perror("Error al abrir el archivo");
        return ERR;
    }
    /*Imprimimos en el fichero los datos del array times*/
    for (i = 0; i < N; i++)
        fprintf(fp, "%d %f %f %d %d\n", ptime[i].N, ptime[i].time, ptime[i].average_ob, ptime[i].max_ob, ptime[i].min_ob);
    fclose(fp);
    return OK;
}

/***************************************************/
/* Function: average_sorting_time Date: 18-10-2023 */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que mide el tiempo y la cantidad de      */
/* operaciones básicas que se ejecutan al hacer    */
/* una función metodo a un array permutado de      */
/* n_perms de N elementos y lo guarda en una       */
/* estructura ptime                                */
/*                                                 */
/* Input:                                          */
/* pfunc_sort metodo: función de la que vamos a    */
/* medir el tiempo y numero de obs                 */
/* int n_perms: número de permutaciones            */
/* int N: número de elementos en cada permutación  */
/* PTIME_AA ptime: estructura donde se guardaran   */
/* los datos                                       */
/*                                                 */
/* Output:                                         */
/* short: OK en caso de que todo funcione          */
/* correctamente o ERR en caso de error            */
/***************************************************/

/***************************************************/
/* Function: generate_sorting_time Date: 18-10-2023*/
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que mide el tiempo y la cantidad de      */
/* operaciones básicas que se ejecutan al hacer    */
/* una función metodo a un array permutado de      */
/* n_perms de N elementos y lo guarda en una       */
/* estructura ptime                                */
/*                                                 */
/* Input:                                          */
/* int incr: incremento entre cada permutación     */
/* int num_min: número de donde empieza a hacer    */
/* las permutaciones                               */
/* int num_max: número de donde termina de hacer   */
/* las permutaciones                               */
/* char* file: fichero donde se guardaran los      */
/* resultados                                      */
/* int n_perms: número de permutaciones            */
/*                                                 */
/* Output:                                         */
/* short: OK en caso de que todo funcione          */
/* correctamente o ERR en caso de error            */
/***************************************************/

/***************************************************/
/* Function: average_sorting_time Date: 18-10-2023 */
/* Authors: Marcos Muñoz Merchan e Ignacio Serena  */
/* Montiel                                         */
/*                                                 */
/* Rutina que imprime en un fichero los datos de   */
/* la estructura ptime                             */
/*                                                 */
/* Input:                                          */
/* char* file: fichero donde se guardaran los      */
/* resultados                                      */
/* PTIME_AA ptime: estructura donde de estan los   */
/* datos a imprimir                                */
/* n_times: el número de permutaciones distintas   */
/*                                                 */
/* Output:                                         */
/* short: OK en caso de que todo funcione          */
/* correctamente o ERR en caso de error            */
/***************************************************/
