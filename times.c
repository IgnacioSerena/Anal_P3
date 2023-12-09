/**
 *
 * Descripcion: Implementation of time measurement functions
 *
 * Fichero: times.c
 * Autor: Ignacio Serena Montiel e Marcos Muñoz Merchan
 * Version: 1.0
 * Fecha: 08-11-2023
 *
 */

#include "times.h"
#include "permutations.h"
#include <assert.h>

/***************************************************/
/* Function: average_search_time Date: 08-11-2023  */
/* Authors:Ignacio Serena Montiel e Marcos Muñoz   */
/* Merchan                                         */
/*                                                 */
/* Rutina que calcula el tiempo promedio de        */
/* búsqueda y otras métricas asociadas al realizar */
/* búsquedas en un diccionario utilizando un       */
/* algoritmo de búsqueda específico. También puede */
/* generar estadísticas sobre el número promedio de*/
/* operaciones básicas realizadas durante la       */
/* búsqueda.                                       */
/*                                                 */
/* Input:                                          */
/* pfunc_sort metodo: puntero a la función de      */
/* búsqueda que se utilizará                       */
/* pfunc_key_generator generator: puntero a la     */
/* función generadora de claves                    */
/* int order:  Indica si el diccionario está       */
/* ordenado (SORTED) o no (NOT_SORTED).            */
/* int N: número de elementos en cada permutación  */
/* int n_times: Número de veces que se realizará   */
/* la búsqueda                                     */
/* PTIME_AA ptime: estructura donde se guardaran   */
/* los datos                                       */
/*                                                 */
/* Output:                                         */
/* short: OK en caso de que todo funcione          */
/* correctamente o ERR en caso de error            */
/***************************************************/

short average_search_time(pfunc_search metodo, pfunc_key_generator generator, int order, int N, int n_times, PTIME_AA ptime)
{
    PDICT pdict;
    int *permutation = NULL, *keys_to_search = NULL;
    clock_t start_time, end_time;
    int total_ob = 0;
    int min_ob = N + 1;
    int max_ob = 0;
    int i, ob, pos;

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
    keys_to_search = (int *)malloc(sizeof(int) * (n_times * N));
    if (!keys_to_search)
    {
        free(permutation);
        free_dictionary(pdict);
        return ERR;
    }

    /*Llenado de tabla*/
    generator(keys_to_search, n_times * N, N);
    if (keys_to_search[0] < 0)
    {
        fprintf(stderr, "Error generando claves\n");
        free(keys_to_search);
        free(permutation);
        free_dictionary(pdict);
        return ERR;
    }

    /*Medición de tiempos*/
    start_time = clock();

    for (i = 0; i < (n_times * N); i++)
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
    ptime->time = ((double)(end_time - start_time)) / (n_times * CLOCKS_PER_SEC);
    ptime->average_ob = (double)total_ob / (n_times * N);
    ptime->min_ob = min_ob;
    ptime->max_ob = max_ob;

    /*Liberar memoria*/
    free(keys_to_search);
    free(permutation);
    free_dictionary(pdict);

    return OK;
}

/***************************************************/
/* Function: generate_search_time Date: 08-11-2023 */
/* Authors:Ignacio Serena Montiel e Marcos Muñoz   */
/* Merchan                                         */
/*                                                 */
/* Rutina que automatiza la toma de tiempos para   */
/* realizar mediciones de rendimiento de las       */
/* funciones de búsqueda en un diccionario.        */
/*                                                 */
/* Input:                                          */
/* pfunc_sort metodo: puntero a la función de      */
/* búsqueda que se utilizará                       */
/* pfunc_key_generator generator: puntero a la     */
/* función generadora de claves                    */
/* int order:  Indica si el diccionario está       */
/* char *file: nombre del fichero a escribir       */
/* ordenado (SORTED) o no (NOT_SORTED).            */
/* int num_min: tamaño mínimo del diccionario      */
/* int num_max: tamaño máximo del diccionario      */
/* int n_times: Número de veces que se realizará   */
/* la búsqueda                                     */
/*                                                 */
/* Output:                                         */
/* short: OK en caso de que todo funcione          */
/* correctamente o ERR en caso de error            */
/***************************************************/

short generate_search_times(pfunc_search metodo, pfunc_key_generator generator, int order, char *file, int num_min, int num_max, int incr, int n_times)
{
    int i, num;
    PTIME_AA ptime_array;

    ptime_array = (PTIME_AA)malloc(n_times * sizeof(TIME_AA));
    if (!ptime_array)
        return ERR;

    for (num = num_min, i = 0; num <= num_max; num += incr, i++)
    {
        if (average_search_time(metodo, generator, order, num, n_times, &ptime_array[i]) != OK)
        {
            free(ptime_array);
            return ERR;
        }
    }

    if (save_time_table(file, ptime_array, ((num_max - num_min) / incr) + 1) != OK)
    {
        free(ptime_array);
        return ERR;
    }

    free(ptime_array);
    return OK;
}

/***************************************************/
/* Function: save_time_table      Date: 08-11-2023 */
/* Authors:Ignacio Serena Montiel e Marcos Muñoz   */
/* Merchan                                         */
/*                                                 */
/* Rutina que imprime en un fichero los datos de   */
/* la estructura ptime                             */
/*                                                 */
/* Input:                                          */
/* char* file: fichero donde se guardaran los      */
/* resultados                                      */
/* PTIME_AA ptime: estructura donde de estan los   */
/* datos a imprimir                                */
/* N: Número de elementos en la estructura ptime   */
/*                                                 */
/* Output:                                         */
/* short: OK en caso de que todo funcione          */
/* correctamente o ERR en caso de error            */
/***************************************************/
short save_time_table(char *file, PTIME_AA ptime, int N)
{
    FILE *fp = fopen(file, "w");
    int i;

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
