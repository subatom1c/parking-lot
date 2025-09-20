/**
 * @file sorting.h
 * @brief Definitions and function prototypes about sorting
 * @author ist109872
 */

// Includes
#include <stdio.h>

/**
 * @struct Entry
 * @brief Represents an entry
 *
 * This struct is used to organize information through various flags
 * headCar_in_park: Main car in the hashtable is in park
 * entry_in_park: That specific entry is the car in the park
 * compare_exit: Flag to compare via exits
 */
typedef struct {
    char * park_name;
    char license_plate[9];
    int entry_day, entry_month, entry_year, entry_hour, entry_minute;
    int headCar_in_park, entry_in_park, compare_exit; // Flags
    int exit_day, exit_month, exit_year, exit_hour, exit_minute;
} Entry;

// Functions Prototypes
int compare(Entry *first, Entry *second);
void merge(Entry arr[], int left, int middle, int right);
void mergeSort(Entry arr[], int left, int right);
int compareString(char * string1, char * string2);


