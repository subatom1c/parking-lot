/**
 * @file parque.c
 * @brief Includes functions about parks
 * @author ist109872
*/

// Includes:
#include "parque.h"
#include "carros.h"
#include "hashtable.h"
#include "datas.h"
#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines:
/**
 * MAXPARQUES: Max number of parks
 * HASHSIZE: HashTable Size
 * INITIALSIZE: Initial size for memory allocations
*/
#define MAXPARQUES 20
#define HASHSIZE 53
#define INITIALSIZE 10

// Functions:

/**
 * @brief p function.
 *
 * 'p' (no parameters) - Shows a list of every park in the system followed by
 * its capacity and available seats
 *
 * 'p' (5 parameters) - Creates a park
 *
 * @param head A pointer to the head of all parks
 * @param parks_number Number of parks in the system
 *
 * When p (5 argument) is called it creates a park with 5 parameters such as
 *  - name: Park's name
 *  - capacity: Park's capacity
 * - val_first_15: The cost of parking for the first 15 minutes.
 * - val_15_after_hour: The cost of parking for each additional minute
 * after the first 15 minutes.
 * - max_daily_val: The maximum daily parking cost.
 *
 * @return parks_number Update number of parks in the system
 */
int p(List *parkList, int parks_number){
    char* name;
    int capacity, error;
    float val_first_15, val_15_after_hour, max_dialy_val;
    int character = getchar();

    if (character == '\n') // No parameters
        show_parks(parkList->head);
    else {
        read_park_name(&name);
        scanf("%d %f %f %f", &capacity, &val_first_15,
              &val_15_after_hour, &max_dialy_val);

        error = check_errors(parkList->head, name, parks_number, capacity,
                             val_first_15, val_15_after_hour, max_dialy_val);
        if (!error) {
            create_park(name, capacity, val_first_15,
                        val_15_after_hour, max_dialy_val, parkList);
            parks_number++;
        }
        getchar();
        free(name);
    }
    return parks_number;
}

/**
 * @brief show_parks function.
 *
 * 'show_parks' (1 argument) - Shows all parks in the system
 * @param head A pointer to the head of all parks
 *
 * @details Goes through every park in the linked list and prints its name,
 * capacity and empty seats
 */
void show_parks(Park *head){
    Park *aux = head;
    int counter = 0;
    while (aux != NULL){
        printf("%s %d %d\n", aux->name, aux->capacity,
               aux->empty_seats);
        aux = (Park *) aux->next;
        counter++;

    }
}

/**
 * @brief create_park function.
 *
 * 'create_park' (6 arguments) - Creates a park
 *
 * @param park_name Park's name
 * @param capacity Park's capacity
 * @param val_first_15: The cost of parking for the first 15 minutes.
 * @param val_15_after_hour: The cost of parking for each additional minute
 * after the first 15 minutes.
 * @param max_daily_val: The maximum daily parking cost.
 * @param head A pointer to the head of all parks
 *
 * Inserts a park into the linked list of parks
 */
void create_park(char* park_name, int capacity, float val_first_15,
                 float val_15_after_hour, float max_daily_val, List *parkList){
    Park * new_park = (Park*) malloc(sizeof(Park));
    new_park->name = (char*) malloc((strlen(park_name) + 1) * sizeof(char));
    init_hashtable(new_park);

    // Initialize parks values
    strcpy(new_park->name, park_name);
    new_park->name[strlen(park_name)] = '\0';
    new_park->capacity = capacity;
    new_park->val_first_15 = val_first_15;
    new_park->val_15_after_hour = val_15_after_hour;
    new_park->max_daily_val = max_daily_val;
    new_park->next = NULL;
    new_park->empty_seats = capacity;
    new_park->numberValuesHash = 0;
    new_park->hashtable_size = INITIALSIZE;

    // If it is the first park
    if (parkList->head == NULL) {
        parkList->head = new_park;
        parkList->tail = new_park;
    } else {
        parkList->tail->next = new_park;
        parkList->tail = new_park;
    }
    // Initialize car list
    new_park->car_head = NULL;
}


/**
 * @brief check_errors function.
 *
 * 'check_errors' (7 arguments) - Checks for any erros in parameters to create
 *                               a park
 *
 * @param head A pointer to the head of all parks
 * @param park_name Park's name
 * @param parks_number Number of parks in the system
 * @param capacity Park's capacity
 * @param val_first_15: The cost of parking for the first 15 minutes.
 * @param val_15_after_hour: The cost of parking for each additional
 * minute after the first 15 minutes.
 * @param max_daily_val: The maximum daily parking cost.
 *
 * @return Error found
 */
int check_errors(Park * head, char* park_name, int parks_number, int capacity,
                 float val_first_15, float val_15_after_hour,
                 float max_daily_val){
    Park * aux = head;

    // Checks if park already exists
    while (aux != NULL) {
        if (strcmp(aux->name, park_name) == 0){
            printf("%s: parking already exists.\n", park_name);
            return 1;
        }
        aux = aux->next;
    }
    // Checks if capacity is positive
    if (capacity <= 0) {
        printf("%d: invalid capacity.\n", capacity);
        return 1;
    }
    // Checks if revenue values are valid
    if (val_first_15 >= val_15_after_hour ||
        val_15_after_hour >= max_daily_val ||
        val_first_15 <= 0 || val_15_after_hour <= 0 || max_daily_val <= 0){
        printf("invalid cost.\n");
        return 1;
    }
    // Checks if number of parks is greater than the max
    if (parks_number >= MAXPARQUES){
        printf("too many parks.\n");
        return 1;
    }
    return 0;
}


/**
 * @brief park_exists function.
 *
 * 'park_exists' (2 arguments) - Checks if a park already exists in the
 *                                system.
 *
 * @param head A pointer to the head of all parks
 * @param park_name Park's name
 *
 * @return Park found
 */
int park_exists(Park * head, char * park_name){
    Park * parque_auxiliar = head;
    // Loop to check every park
    while (parque_auxiliar != NULL){
        if (strcmp(parque_auxiliar->name, park_name) == 0)
            return 1;
        parque_auxiliar = parque_auxiliar->next;
    }
    printf("%s: no such parking.\n", park_name);
    return 0;
}


/**
 * @brief removePark function
 *
 * 'removePark' (2 argument) - Removes a park from the system
 *
 * @param parkList Pointer to List with head and tail of a park
 * @param park_name Park's name
 *
 * @details We find the park in the linked list via park_name, while
 * adding all the parks names into an array of entries and then sorting it
 */
void removePark(List *parkList, char *park_name) {
    Park *crtPark = parkList->head;
    Park *aux = NULL;
    Park *prev = NULL; // Previous park
    int count = 0, size = INITIALSIZE;
    if (parkList->head == NULL) return; // If no parks
    // Entries for prints
    Entry *entries = (Entry *) malloc(sizeof(Entry) * size);

    if (strcmp(parkList->head->name, park_name) == 0){
        aux = parkList->head;
        parkList->head = parkList->head->next;
        if (parkList->tail == aux)
            parkList->tail = NULL;
        removeParkAux(aux);

        Park * head_aux = parkList->head;
        while (head_aux != NULL){
            entries[count].park_name = head_aux->name;
            entries[count].compare_exit = 0;
            count++;
            head_aux = head_aux->next;
        }
    } else {
        // Find park
        while (crtPark != NULL && strcmp(crtPark->name, park_name) != 0) {
            entries[count].park_name = crtPark->name;
            entries[count].compare_exit = 0;
            prev = crtPark;
            crtPark = crtPark->next;
            count++;
        }
        if (crtPark != NULL && crtPark->next != NULL) {
            aux = crtPark;
            prev->next = crtPark->next;
            removeParkAux(aux);

            while (prev->next != NULL){
                entries[count].park_name = prev->next->name;
                entries[count].compare_exit = 0;
                count++;
                prev = prev->next;
            }
            entries[count].park_name = prev->name;
            entries[count].compare_exit = 0;
        } else if (crtPark != NULL && crtPark->next == NULL){
            aux = crtPark;
            prev->next = NULL;
            parkList->tail = prev;
            removeParkAux(aux);
        }
    }
    // Sort entries by alphabetical order
    mergeSort(entries, 0, count - 1);
    for (int i = 0; i < count; i++) {
        printf("%s\n", entries[i].park_name);
    }
    free(entries);
}

/**
 * @brief removeParkAux function.
 *
 * 'removeParkAux' (1 argument) - Removes a park from system
 *
 * @param Park Pointer to park that is going to be removed
 */
void removeParkAux(Park* park){
    for (int i = 0; i < park->numberValuesHash; i++){
        int index = park->occupied_indexes[i];
        while (park->hashtable[index] != NULL) {
            while (park->hashtable[index]->head_entries != NULL) {
                CarEntry *aux = park->hashtable[index]->head_entries;
                park->hashtable[index]->head_entries =
                        park->hashtable[index]->head_entries->next_entry;
                free(aux);
            }
            Car * aux = park->hashtable[index];
            park->hashtable[index] = park->hashtable[index]->next_hashtable;
            free(aux);
        }
    }
    free(park->name);
    free(park->occupied_indexes);
    free(park);
}


/**
 * @brief read_park_name function.
 *
 * 'read_park_name' (1 argument) - Reads a parks name from stdin
 *
 * @param park_name Pointer to parks name
 *
 * @return Last Character Read
 * @details Reads characters using getchar until \n is reached or after the
 * flag marks (number of marks read) reaches 2 and we get a space
 */
char read_park_name(char ** park_name) {
    int size = INITIALSIZE, character, marks = 0, counter = 0;
    *park_name = (char*)malloc(size * sizeof(char));
    while (((character = getchar()) != ' ' || marks != 2) && character!='\n'){
        if (character == '"')
            marks++;
        else
        if (marks == 0 && character == ' ')
            break;
        else
            (*park_name)[counter++] = (char) character;

        if (counter >= size - 1) {
            size *= 2;
            *park_name = realloc(*park_name, size * sizeof(char) + 1);
        }
    }
    (*park_name)[counter] = '\0';
    return character;
}


/**
 * @brief show_park_revenue (6 arguments )function, shows a park revenue on a
 * certain day or in general, based on a flag provided
 *
 * @param head A pointer to the head of all parks
 * @param park_name Park's name
 * @param day Chosen day to show revenue
 * @param month Chosen month to show revenue
 * @param year: Chosen year to show revenue
 * @param flag: Flag to know, what to print
 *
 * @details Look for park using name, using an array of entries for each car
 * and a array of ints to keep track of how many entries are there for each
 * car (counter), we go through the occupied_indexes of the hashtable using
 * its size and we get every entry of every car in the system then we sort
 * it and print it out
 */
void show_park_revenue(Park *head, char *park_name,
                       int day, int month, int year, int flag) {

    Park *crtCar = head;
    if (crtCar == NULL) return;
    // Find park
    while (crtCar != NULL && strcmp(crtCar->name, park_name) != 0)
        crtCar = crtCar->next;

    int sameCarCnt, carCnt = 0;
    int sameCarCntSize = INITIALSIZE, counter_size = INITIALSIZE;
    Entry **entries = (Entry **) malloc(counter_size * sizeof(Entry*));
    int *counter = (int *) malloc(counter_size * sizeof(int));

    for (int i = 0; i < crtCar->numberValuesHash; i++) {
        int index = crtCar->occupied_indexes[i];
        if (crtCar->hashtable[index] != NULL) {
            Car *car_head = crtCar->hashtable[index];
            while (car_head != NULL) { // Go through linked list in hashtable
                if (carCnt + 2 >= counter_size) {
                    // counter and entries grow with equal velocity
                    counter_size *= 2;
                    entries = realloc(entries, counter_size * sizeof(Entry*));
                    counter = realloc(counter, counter_size * sizeof(int));
                }

                sameCarCnt = 0;
                entries[carCnt] = (Entry*)malloc
                        (sameCarCntSize * sizeof(Entry));
                if (!car_head->mainCar_in_park) { // Head car not in park
                    entries[carCnt][sameCarCnt].park_name = NULL;
                    strcpy(entries[carCnt][sameCarCnt].license_plate,
                           car_head->licensePlate);
                    entries[carCnt][sameCarCnt].entry_day =
                            car_head->entry_date.day;
                    entries[carCnt][sameCarCnt].entry_month =
                            car_head->entry_date.month;
                    entries[carCnt][sameCarCnt].entry_year =
                            car_head->entry_date.year;
                    entries[carCnt][sameCarCnt].entry_hour =
                            car_head->entry_time.hour;
                    entries[carCnt][sameCarCnt].entry_minute =
                            car_head->entry_time.minute;
                    entries[carCnt][sameCarCnt].exit_day =
                            car_head->exit_date.day;
                    entries[carCnt][sameCarCnt].exit_month =
                            car_head->exit_date.month;
                    entries[carCnt][sameCarCnt].exit_year =
                            car_head->exit_date.year;
                    entries[carCnt][sameCarCnt].exit_hour =
                            car_head->exit_time.hour;
                    entries[carCnt][sameCarCnt].exit_minute =
                            car_head->exit_time.minute;
                    sameCarCnt++;
                }
                // Iterate through every entry of car
                CarEntry *head_entradas = car_head->head_entries;
                while (head_entradas != NULL) {
                    if (sameCarCnt + 1 >= sameCarCntSize) {
                        sameCarCntSize *= 2;
                        Entry *temp = realloc(entries[carCnt],
                                              sameCarCntSize * sizeof(Entry));
                        entries[carCnt] = temp;
                    }
                    // Update only if car is not inside a park
                    if (!head_entradas->entry_in_park) {
                        strcpy(entries[carCnt][sameCarCnt].license_plate,
                               car_head->licensePlate);
                        entries[carCnt][sameCarCnt].park_name = NULL;
                        entries[carCnt][sameCarCnt].entry_day =
                                head_entradas->entry_date.day;
                        entries[carCnt][sameCarCnt].entry_month =
                                head_entradas->entry_date.month;
                        entries[carCnt][sameCarCnt].entry_year =
                                head_entradas->entry_date.year;
                        entries[carCnt][sameCarCnt].entry_hour =
                                head_entradas->entry_time.hour;
                        entries[carCnt][sameCarCnt].entry_minute =
                                head_entradas->entry_time.minute;
                        entries[carCnt][sameCarCnt].exit_day =
                                head_entradas->exit_date.day;
                        entries[carCnt][sameCarCnt].exit_month =
                                head_entradas->exit_date.month;
                        entries[carCnt][sameCarCnt].exit_year =
                                head_entradas->exit_date.year;
                        entries[carCnt][sameCarCnt].exit_hour =
                                head_entradas->exit_time.hour;
                        entries[carCnt][sameCarCnt].exit_minute =
                                head_entradas->exit_time.minute;
                        sameCarCnt++;
                    }
                    head_entradas = head_entradas->next_entry;
                }
                counter[carCnt] = sameCarCnt;
                carCnt++;
                car_head = car_head->next_hashtable;
            }
        }
    }

    // Initialize flag for mergesort compare exits
    for (int j = 0; j < carCnt; j++)
        for (int i = 0; i < counter[j]; i++)
            entries[j][i].compare_exit = 1;

    int array_size = 0;
    for (int j = 0; j < carCnt; j++)
        array_size += counter[j];

    // New flattened array
    Entry *entries_array = (Entry *) malloc(sizeof(Entry) * array_size);
    int count = 0;

    // Flattening and counting
    for (int j = 0; j < carCnt; j++){
        for (int i = 0; i < counter[j]; i++){
            entries_array[count] = entries[j][i];
            count++;
        }
    }

    if (count > 0)
        mergeSort(entries_array, 0, count - 1);

    // If flag we print only in a certain day
    if (flag) {
        for (int j = 0; j < count; j++) {
            float faturacao = calculate_revenue(crtCar,
                                                entries_array[j].entry_day,
                                                entries_array[j].entry_month,
                                                entries_array[j].entry_year,
                                                entries_array[j].entry_hour,
                                                entries_array[j].entry_minute,
                                                entries_array[j].exit_day,
                                                entries_array[j].exit_month,
                                                entries_array[j].exit_year,
                                                entries_array[j].exit_hour,
                                                entries_array[j].exit_minute);
            if (day == entries_array[j].exit_day &&
                month == entries_array[j].exit_month
                && year == entries_array[j].exit_year) {
                printf("%s %02d:%02d %0.2f\n", entries_array[j].license_plate,
                       entries_array[j].exit_hour,
                       entries_array[j].exit_minute,
                       faturacao);
            }
        }
    } else { // Print every day

        int day_aux;
        int month_aux;
        int year_aux;
        if (count > 0){
            day_aux =entries_array[0].exit_day;
            month_aux = entries_array[0].exit_month;
            year_aux = entries_array[0].exit_year;
        }
        float revenue = 0;

        // Iterate through every entry, checking its date, if date changes
        // We stop adding to revenue and update new date
        for (int i = 0; i < count; i++) {
            if (i == 0 || day_aux != entries_array[i].exit_day
                || month_aux != entries_array[i].exit_month
                || year_aux != entries_array[i].exit_year){
                if (i != 0) {
                    printf("%02d-%02d-%04d %0.2f\n", day_aux, month_aux,
                           year_aux, revenue);
                }
                revenue = calculate_revenue(crtCar,
                                            entries_array[i].entry_day,
                                            entries_array[i].entry_month,
                                            entries_array[i].entry_year,
                                            entries_array[i].entry_hour,
                                            entries_array[i].entry_minute,
                                            entries_array[i].exit_day,
                                            entries_array[i].exit_month,
                                            entries_array[i].exit_year,
                                            entries_array[i].exit_hour,
                                            entries_array[i].exit_minute);
                day_aux = entries_array[i].exit_day;
                month_aux = entries_array[i].exit_month;
                year_aux = entries_array[i].exit_year;
            } else { // Add until day changes
                revenue += calculate_revenue(crtCar,
                                             entries_array[i].entry_day,
                                             entries_array[i].entry_month,
                                             entries_array[i].entry_year,
                                             entries_array[i].entry_hour,
                                             entries_array[i].entry_minute,
                                             entries_array[i].exit_day,
                                             entries_array[i].exit_month,
                                             entries_array[i].exit_year,
                                             entries_array[i].exit_hour,
                                             entries_array[i].exit_minute);
            }
        }

        if (count > 0) {
            printf("%02d-%02d-%04d %0.2f\n", day_aux, month_aux, year_aux,
                   revenue);
        }
    }
    // Frees
    for (int j = 0; j < carCnt; j++) {
        for (int i = 0; i < counter[j]; i++)
            entries[j][i].compare_exit = 0;
        free(entries[j]);
    }

    free(entries_array);
    free(counter);
    free(entries);
}

/**
 * @brief f function.
 *
 * 'f' (1 parameter) - Shows revenue of a park
 *
 * 'f' (4 parameters) - Shows revenue of a park in a date
 *
 * @param park_list A pointer to the linked list
 * @param last_entry Date/time of last action
 */
void f(List * park_list, int last_entry){
    int character, flag = 1, day, month, year, date_error = 0;
    char * name_aux;
    getchar();
    character = read_park_name(&name_aux);
    if (character == '\n')
        flag = 0;
    else {
        scanf("%d-%d-%d", &day, &month, &year);
        getchar();
        int temp = conversor(day, month, year, 0, 0);
        if (temp > last_entry || !verifyDate(day, month, year, 0, 0)) {
            printf("invalid date.\n");
            date_error = 1;
        }
    }

    if (!date_error && park_exists(park_list->head, name_aux))
        show_park_revenue(park_list->head, name_aux, day, month,
                          year , flag);
    free(name_aux);
}


/**
 * @brief r function.
 *
 * @param park_list A pointer to the linked list
 * @param parks_number Number of parks in the system
 *
 * @return New number of parks
 */
int r(List * park_list, int parks_number){
    char * name_aux;
    getchar();
    read_park_name(&name_aux);
    if (park_exists(park_list->head, name_aux)) {
        removePark(park_list, name_aux);
        parks_number--;
    }
    free(name_aux);
    return parks_number;
}

/**
 * @brief q function.
 *
 * @param park_list A pointer to the linked list
 *
 * @details Used to stop the program, used also to free all parks
 */
void q(List* park_list){
    int character = getchar();
    if (character != '\n')
        printf("\n");
    while (park_list->head != NULL){
        Park * aux = park_list->head;
        park_list->head = park_list->head->next;
        removeParkAux(aux);
    }
    free(park_list);
}
