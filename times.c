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
    if (!metodo || n_perms <= 0 || N < 0 || !ptime)
    {
        return ERR;
    }
    ini = clock();
    for (i = 0; i < n_perms; i++)
    {
        int ob_actual = 0;
        ob_actual = metodo(permutations[i], 0, N - 1);
        if (ob_actual < 0)
        {
            for (i = 0; i < n_perms; i++)
            {
                free(permutations[i]);
            }
            free(permutations);
            return ERR;
        }
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
short generate_sorting_times(pfunc_sort method, char *file, int num_min, int num_max, int incr, int n_perms)
{
    int num, i, n_times;
    PTIME_AA times;
    if (!method || !file || num_min < 0 || incr < 0 || n_perms <= 0)
    {
        return ERR;
    }
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
    if (save_time_table(file, times, n_times) == ERR)
    {
        free(times);
        return ERR;
    }
    /* Liberar la memoria asignada para los resultados de tiempo*/
    free(times);
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
short save_time_table(char *file, PTIME_AA ptime, int n_times)
{
    FILE *fp = fopen(file, "w");
    int i;
    if (!file || !ptime || n_times < 0)
        return ERR;
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