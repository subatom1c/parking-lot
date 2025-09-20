/**
 * @file sorting.c
 * @brief Includes functions for sorting
 * @author ist109872
*/


// Includes
#include <stdio.h>
#include <string.h>
#include "parque.h"
#include "carros.h"
#include "hashtable.h"
#include "datas.h"
#include "sorting.h"

/**
 * @brief compareString function, compares strings via alphabetical order
 *
 * @param string1 string to be compared
 * @param string2 string to be compared
 *
 * @return 1 if string1 comes first alphabetically, -1 if string2 comes first,
 * 0 if strings are equal
 */
int compareString(char * string1, char * string2){
    int counter = 0;
    while (*string1 != '\0' && *string2 != '\0'){
        if (string1[counter] > string2[counter])
            return 1;
        else if (string1[counter] < string2[counter])
            return -1;
        counter++;
    }
    return 0;
}


/**
 * @brief compare function, compares entries via flags, such as entries,
 *                                                      exits and names
 *
 * @param first entry to be compared
 * @param second entry to be compared
 *
 * @return Positive or negative, depending on which parameter you compare
 */
int compare(Entry *first, Entry *second) {
    // Compare via exits date
    if (first->compare_exit){
        if (first->exit_year != second->exit_year)
            return first->exit_year - second->exit_year;

        if (first->exit_month != second->exit_month)
            return first->exit_month - second->exit_month;

        if (first->exit_day != second->exit_day)
            return first->exit_day - second->exit_day;

        if (first->exit_hour != second->exit_hour)
            return first->exit_hour - second->exit_hour;

        return first->exit_minute - second->exit_minute;
    }

    // Compare via entry date
    if (first->park_name == NULL ||
        strcmp(first->park_name, second->park_name) == 0) {
        if (first->entry_year != second->entry_year)
            return first->entry_year - second->entry_year;

        if (first->entry_month != second->entry_month)
            return first->entry_month - second->entry_month;

        if (first->entry_day != second->entry_day)
            return first->entry_day - second->entry_day;

        if (first->entry_hour != second->entry_hour)
            return first->entry_hour - second->entry_hour;

        return first->entry_minute - second->entry_minute;
    }
    // Compare name
    return compareString(first->park_name, second->park_name);

}

/**
 * @brief Merges two organized halves of an array
 *
 * @param arr Array of entries to be sorted
 * @param left Start index of the first half
 * @param middle End index of the first half
 * @param right End index of the second half
 */
void merge(Entry arr[], int left, int middle, int right) {
    int temp1 = middle - left + 1;
    int temp2 = right - middle;

    Entry lf[temp1], rt[temp2];

    for (int i = 0; i < temp1; i++)
        lf[i] = arr[left + i];
    for (int j = 0; j < temp2; j++)
        rt[j] = arr[middle + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < temp1 && j < temp2) {
        if (compare(&lf[i], &rt[j]) <= 0) {
            arr[k] = lf[i];
            i++;
        } else {
            arr[k] = rt[j];
            j++;
        }
        k++;
    }

    while (i < temp1) {
        arr[k] = lf[i];
        i++;
        k++;
    }

    while (j < temp2) {
        arr[k] = rt[j];
        j++;
        k++;
    }
}

/**
 * Sorts an array using the merge sort algorithm.
 *
 * @param arr Array to sort.
 * @param left Start index for sorting.
 * @param right End index for sorting.
 */
void mergeSort(Entry arr[], int left, int right) {
    if (left < right){
        int mid = left + (right-left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}


