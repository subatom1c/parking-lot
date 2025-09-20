/**
 * @file parque.h
 * @brief Definitions and functions prototypes about parks
 * @author ist109872
 */

// Includes
#include <stdio.h>

// Defines:
/**
 * MAXPARQUES: Max number of parks
 * HASHSIZE: HashTable Size
*/
#define MAXPARQUES 20
#define HASHSIZE 53

/**
 * @struct Park
 * @brief Represents a park
 *
 * This structure has all the necessary information about a park,
 * including its capacity, the revenue, linked lists and a
 * hashtable to store cars and car entries
 */
typedef struct Park {
    char *name;
    int *occupied_indexes; // Array with hashtable indexes
    int capacity, empty_seats, numberValuesHash, hashtable_size;
    float val_first_15, val_15_after_hour, max_daily_val;
    struct Car *car_head; // Pointer to a linked list of cars in the park
    struct Park *next; // Pointer for linking parks in a list
    struct Car *hashtable[HASHSIZE]; // Hashtable for cars
} Park;

/**
 * @struct List
 * @brief Has the head and tail of a linked list of parks
 */
typedef struct {
    Park *head; // Head of a linked list of parks
    Park *tail; // Tail of the linked list
} List;

// Function prototypes
int p(List *parkList, int parks_number);
void show_parks(Park *head);
void create_park(char *park_name, int capacity, float val_first_15,
                 float val_15_after_hour, float max_daily_val, List *parkList);
int check_errors(Park *head, char *nome, int parks_number, int capacity,
                 float val_first_15, float val_15_after_hour,
                 float max_daily_val);
int park_exists(Park *head, char *park_name);
void removePark(List *parkList, char *park_name);
char read_park_name(char **park_name);
void show_park_revenue(Park *head, char *park_name, int day,
                       int month, int year, int flag);
void removeParkAux(Park* park);
void f(List* park_list, int last_entry);
int r(List * park_list, int parks_number);
void q(List* park_list);
