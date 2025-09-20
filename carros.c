/**
 * @file carros.c
 * @brief Includes functions about cars
 * @author ist109872
*/

// Includes
#include "parque.h"
#include "carros.h"
#include "datas.h"
#include "hashtable.h"
#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Defines:
/**
 * INITIALSIZE: Initial size for memory allocations
*/
#define INITIALSIZE 10

/**
 * @brief register_car(8 arguments) registers a car into a park
 *
 * @param head A pointer to the head of all parks
 * @param park_name Name of the park
 * @param licensePlate Car's license plate
 * @param day Day of entry
 * @param month Month of entry
 * @param year Year of entry
 * @param hour Hour of entry
 * @param minute Minute of entry
 *
 * @details Find the park with the name and then put the war in the end
 * of the linked list of cars, also register it into the hashtable for quicker
 * access
 */
void register_car(Park * head, const char* park_name, const char*licensePlate,
                  int day, int month, int year, int hour, int minute){
    // Find park
    Park * Park = head;
    while (Park->next != NULL && strcmp(Park->name, park_name) != 0)
        Park = Park->next;

    // Initialize Car
    Car *new_car = (Car*) malloc(sizeof(Car));
    strcpy(new_car->licensePlate, licensePlate);
    new_car->mainCar_in_park = 1;
    new_car->car_in_park = 0;
    Park->empty_seats -= 1;
    new_car->next_hashtable = NULL;

    printf("%s %d\n", Park->name, Park->empty_seats);

    // Get index from hash function
    int index = hash(licensePlate);

    // Try to insert the car into the hashtable
    if (insert(Park, new_car) == 0){
        int flag_cmp = 0; // Checks if car already registered
        Car * crtCar = Park->hashtable[index];

        if (strcmp(crtCar->licensePlate, new_car->licensePlate) == 0) {
            flag_cmp = 1;
        } else {
            // Look for car
            while (crtCar->next_hashtable != NULL) {
                if (strcmp(crtCar->next_hashtable->licensePlate,
                           new_car->licensePlate) == 0) {
                    flag_cmp = 1;
                    crtCar = crtCar->next_hashtable;
                    break;
                }
                crtCar = crtCar->next_hashtable;
            }
        }
        if (flag_cmp == 1) { // Car found
            free(new_car);
            // If 1st entry, we put in head_entries
            if (crtCar->head_entries == NULL){
                crtCar->head_entries = (CarEntry *)malloc(sizeof(CarEntry));
                crtCar->head_entries->entry_date.day = day;
                crtCar->head_entries->entry_date.month = month;
                crtCar->head_entries->entry_date.year = year;
                crtCar->head_entries->entry_time.hour = hour;
                crtCar->head_entries->entry_time.minute = minute;
                crtCar->head_entries->next_entry = NULL;
                crtCar->car_in_park = 1;
                crtCar->head_entries->entry_in_park = 1;
                crtCar->mainCar_in_park = 0;
            } else {
                // Insert it in the end of entries linked list
                if (crtCar->head_entries->next_entry == NULL){
                    crtCar->head_entries->next_entry = (CarEntry*)malloc
                            (sizeof(CarEntry));
                    crtCar->head_entries->next_entry->entry_date.day = day;
                    crtCar->head_entries->next_entry->entry_date.month = month;
                    crtCar->head_entries->next_entry->entry_date.year = year;
                    crtCar->head_entries->next_entry->entry_time.hour = hour;
                    crtCar->head_entries->next_entry->entry_time.minute=minute;
                    crtCar->head_entries->next_entry->next_entry = NULL;
                    crtCar->car_in_park = 1;
                    crtCar->head_entries->next_entry->entry_in_park = 1;
                    crtCar->mainCar_in_park = 0;

                } else {
                    CarEntry *current = crtCar->head_entries;
                    while (current->next_entry != NULL) {
                        current = current->next_entry;
                    }
                    current->next_entry = (CarEntry*)malloc(sizeof(CarEntry));
                    current->next_entry->entry_date.day = day;
                    current->next_entry->entry_date.month = month;
                    current->next_entry->entry_date.year = year;
                    current->next_entry->entry_time.hour = hour;
                    current->next_entry->entry_time.minute = minute;
                    current->next_entry->next_entry = NULL;
                    crtCar->car_in_park = 1;
                    current->next_entry->entry_in_park = 1;
                    crtCar->mainCar_in_park = 0;

                }
            }


        } else {
            // No car registered with that license plate
            crtCar->next_hashtable = new_car;
            strcpy(crtCar->next_hashtable->licensePlate, licensePlate);
            crtCar->next_hashtable->mainCar_in_park = 1;
            crtCar->next_hashtable->car_in_park = 0;
            crtCar->next_hashtable->entry_date.day = day;
            crtCar->next_hashtable->entry_date.month = month;
            crtCar->next_hashtable->entry_date.year = year;
            crtCar->next_hashtable->entry_time.hour = hour;
            crtCar->next_hashtable->entry_time.minute = minute;
            crtCar->next_hashtable->next_hashtable = NULL;
            crtCar->next_hashtable->head_entries = NULL;
        }

    } else {
        // If first hashtable entry, allocate memory
        if (Park->numberValuesHash == 0)
            Park->occupied_indexes = (int*)malloc
                    (sizeof(int) * Park->hashtable_size);

        Park->occupied_indexes[Park->numberValuesHash] = index;
        Park->numberValuesHash++;

        // If close to memory limit allocate more memory
        if (Park->numberValuesHash + 2 >= Park->hashtable_size){
            Park->hashtable_size*=2;
            Park->occupied_indexes = realloc(Park->occupied_indexes,
                                             sizeof(int)*Park->hashtable_size);

        }
        new_car->entry_date.year = year;
        new_car->entry_date.month = month;
        new_car->entry_date.day = day;
        new_car->entry_time.hour = hour;
        new_car->entry_time.minute = minute;
    }
}


/**
 * @brief CheckLicPlate(1 argument) Checks if a license plate is valid
 *
 * @param licensePlate Car's license plate
 *
 * @returns if License is valid
 */
int CheckLicPlate(const char* licensePlate){
    // flags to check if license has atleast 1 pair of numbers and 1 pair
    // of letters and counter for beggining of each pair
    int flag_letter = 0, flag_number = 0, counter = 0;
    for (int i = 0; licensePlate[i] != '\0'; i++){
        if (licensePlate[i] != '-' && counter % 2 == 0){
            if (isupper(licensePlate[i])){
                if (!isupper(licensePlate[i + 1])) {
                    printf("%s: invalid licence plate.\n", licensePlate);
                    return 0;
                }
                flag_letter = 1;
            } else if (isdigit(licensePlate[i])){
                if (!isdigit(licensePlate[i + 1])){
                    printf("%s: invalid licence plate.\n", licensePlate);
                    return 0;
                }
                flag_number = 1;
            } else {
                printf("%s: invalid licence plate.\n", licensePlate);
                return 0;
            }
        }
        if (licensePlate[i] != '-')
            counter++;
    }

    if (flag_letter == 0 || flag_number == 0){
        printf("%s: invalid licence plate.\n", licensePlate);
        return 0;
    }
    return 1;
}


/**
 * @brief CheckEntryErrors(9 argument) Checks for entry errors
 *
 * @param head A pointer to the head of all parks
 * @param park_name Name of the park
 * @param licensePlate Car's license plate
 * @param day Day of entry
 * @param month Month of entry
 * @param year Year of entry
 * @param hour Hour of entry
 * @param minute Minute of entry
 * @param lastEntry lastEntry of a car
 *
 * @returns Error found
 */
int CheckEntryErrors(Park * head, const char * park_name,
                     const char* licensePlate, int day, int month,
                     int year, int hour, int minute, int lastEntry){
    Park * cntPark = head;
    int flag = 0; // Checks if park exists
    while (cntPark != NULL){
        if (strcmp(cntPark->name, park_name) == 0) {
            flag = 1;
            break;
        }
        cntPark = cntPark->next;
    }
    if (flag == 0) {
        printf("%s: no such parking.\n", park_name);
        return 1;
    }

    if (!CheckLicPlate(licensePlate))
        return 1;

    if (cntPark->empty_seats == 0) {
        printf("%s: parking is full.\n", park_name);
        return 1;
    }

    Park * aux = head;
    if (car_inside_park(aux, licensePlate)){
        printf("%s: invalid vehicle entry.\n", licensePlate);
        return 1;
    }

    if (!verifyDate(day, month, year, hour, minute)){
        printf("invalid date.\n");
        return 1;
    }

    int entry = conversor(day, month, year, hour, minute);
    if (entry < lastEntry){
        printf("invalid date.\n");
        return 1;
    }
    return 0;
}


/**
 * @brief register_exit(8 argument) Register a car exit
 *
 * @param head A pointer to the head of all parks
 * @param park_name Name of the park
 * @param licensePlate Car's license plate
 * @param day Day of entry
 * @param month Month of entry
 * @param year Year of entry
 * @param hour Hour of entry
 * @param minute Minute of entry
 *
 * @details We find the park using its name and we register the car exit
 * from the linked list of cars, after that we update the hashtable to
 * also register that exit
 */
void register_exit(Park * head, const char * park_name,
                   const char * licensePlate, int day,
                   int month, int year, int hour, int minute){
    Park * crtPark = head;
    while (crtPark->next != NULL && strcmp(park_name, crtPark->name) != 0)
        crtPark = crtPark->next;

    crtPark->empty_seats++;

    int index = hash(licensePlate);
    float revenue;
    Car * cntCar = crtPark->hashtable[index];
    // Look for car in the hashtable

    if (strcmp(cntCar->licensePlate, licensePlate) != 0)
        while(cntCar->next_hashtable != NULL) {
            if (strcmp(cntCar->next_hashtable->licensePlate, licensePlate)==0){
                cntCar = cntCar->next_hashtable;
                break;
            }
            cntCar = cntCar->next_hashtable;
        }
    if (strcmp(cntCar->licensePlate, licensePlate) != 0)
        cntCar = cntCar->next_hashtable;


    // If it only has one entry, update values according and print out
    // needed info
    if (cntCar->head_entries == NULL) {
        cntCar->exit_date.year = year;
        cntCar->exit_date.month = month;
        cntCar->exit_date.day = day;
        cntCar->exit_time.hour = hour;
        cntCar->exit_time.minute = minute;
        cntCar->mainCar_in_park = 0;
        revenue = calculate_revenue(crtPark, cntCar->entry_date.day,
                                    cntCar->entry_date.month,
                                    cntCar->entry_date.year,
                                    cntCar->entry_time.hour,
                                    cntCar->entry_time.minute,
                                    cntCar->exit_date.day,
                                    cntCar->exit_date.month,
                                    cntCar->exit_date.year,
                                    cntCar->exit_time.hour,
                                    cntCar->exit_time.minute);
        printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d %.2f\n",
               licensePlate, cntCar->entry_date.day, cntCar->entry_date.month,
               cntCar->entry_date.year,cntCar->entry_time.hour,
               cntCar->entry_time.minute, cntCar->exit_date.day,
               cntCar->exit_date.month, cntCar->exit_date.year,
               cntCar->exit_time.hour, cntCar->exit_time.minute, revenue);

    } else { // If it's not first entry
        // Find the last entry and update it, printing info
        if (cntCar->head_entries->next_entry == NULL){
            cntCar->head_entries->exit_date.year = year;
            cntCar->head_entries->exit_date.month = month;
            cntCar->head_entries->exit_date.day = day;
            cntCar->head_entries->exit_time.hour = hour;
            cntCar->head_entries->exit_time.minute = minute;
            cntCar->mainCar_in_park = 0;
            cntCar->car_in_park = 0;
            cntCar->head_entries->entry_in_park = 0;
            revenue = calculate_revenue(crtPark,
                                        cntCar->head_entries->entry_date.day,
                                        cntCar->head_entries->entry_date.month,
                                        cntCar->head_entries->entry_date.year,
                                        cntCar->head_entries->entry_time.hour,
                                        cntCar->head_entries->entry_time.
                                                minute,
                                        cntCar->head_entries->exit_date.day,
                                        cntCar->head_entries->exit_date.month,
                                        cntCar->head_entries->exit_date.year,
                                        cntCar->head_entries->exit_time.hour,
                                        cntCar->head_entries->exit_time
                                                .minute);
            printf("%s %02d-%02d-%04d %02d:%02d "
                   "%02d-%02d-%04d %02d:%02d %.2f\n",
                   licensePlate,cntCar->head_entries->entry_date.day,
                   cntCar->head_entries->entry_date.month,
                   cntCar->head_entries->entry_date.year,
                   cntCar->head_entries->entry_time.hour,
                   cntCar->head_entries->entry_time.minute,
                   cntCar->head_entries->exit_date.day,
                   cntCar->head_entries->exit_date.month,
                   cntCar->head_entries->exit_date.year,
                   cntCar->head_entries->exit_time.hour,
                   cntCar->head_entries->exit_time.minute, revenue);

        } else {
            CarEntry * temp = cntCar->head_entries;
            while (temp->next_entry != NULL)
                temp = temp->next_entry;

            temp->exit_date.year = year;
            temp->exit_date.month = month;
            temp->exit_date.day = day;
            temp->exit_time.hour = hour;
            temp->exit_time.minute = minute;
            cntCar->mainCar_in_park = 0;
            cntCar->car_in_park = 0;
            temp->entry_in_park = 0;
            revenue = calculate_revenue(crtPark, temp->entry_date.day,
                                        temp->entry_date.month,
                                        temp->entry_date.year,
                                        temp->entry_time.hour,
                                        temp->entry_time.minute,
                                        temp->exit_date.day,
                                        temp->exit_date.month,
                                        temp->exit_date.year,
                                        temp->exit_time.hour,
                                        temp->exit_time.minute);
            printf("%s %02d-%02d-%04d %02d:%02d "
                   "%02d-%02d-%04d %02d:%02d %.2f\n",
                   licensePlate,temp->entry_date.day, temp->entry_date.month,
                   temp->entry_date.year,temp->entry_time.hour,
                   temp->entry_time.minute,temp->exit_date.day,
                   temp->exit_date.month, temp->exit_date.year,
                   temp->exit_time.hour, temp->exit_time.minute, revenue);

        }
    }

}

/**
 * @brief CheckExitErrors(9 argument) Checks for exit errors
 *
 * @param head A pointer to the head of all parks
 * @param park_name Name of the park
 * @param licensePlate Car's license plate
 * @param day Day of entry
 * @param month Month of entry
 * @param year Year of entry
 * @param hour Hour of entry
 * @param minute Minute of entry
 * @param lastEntry lastEntry of a car
 *
 * @returns Error found
 */

int CheckExitErrors(Park * head, const char * park_name,
                    const char* licensePlate, int day, int month,
                    int year, int hour, int minute, int last_entry){
    Park * crtPark = head;
    int flag = 0; // Checks if there is the parking in the system
    while (crtPark != NULL){
        if (strcmp(crtPark->name, park_name) == 0) {
            flag = 1;
            break;
        }
        crtPark = crtPark->next;
    }
    if (flag == 0) {
        printf("%s: no such parking.\n", park_name);
        return 1;
    }
    if (!CheckLicPlate(licensePlate)){
        return 1;
    }

    if (!car_inside_park(crtPark, licensePlate)){
        printf("%s: invalid vehicle exit.\n", licensePlate);
        return 1;
    }

    if (!verifyDate(day, month, year, hour, minute)){
        printf("invalid date.\n");
        return 1;
    }

    int entry = conversor(day, month, year, hour, minute);

    if (entry < last_entry){
        printf("invalid date.\n");
        return 1;
    }

    return 0;
}

/**
 * @brief list_entries(2 argument) Lists entries and exits
 * of a car in every park
 *
 * @param head A pointer to the head of all parks
 * @param licensePlate Car's license plate
 *
 * @details We go through every park and using the hash function to get
 * the index of where the car would be in the hashtable we go through
 * all cars until we find it (in case of colisions)
 */
void list_entries(Park * head , const char * matricula) {
    Park *crtPark = head;
    int index = hash(matricula);
    int counter = 0, size = INITIALSIZE;
    Entry *entries = (Entry *) malloc(size * sizeof(Entry));
    // Look in every park
    while (crtPark != NULL) {
        if (crtPark->hashtable[index] != NULL) {
            Car *crtCar = crtPark->hashtable[index];
            // Look for car in hashtable given the index (in case of colision)
            while (crtCar != NULL) {
                if (strcmp(crtCar->licensePlate, matricula) == 0) {
                    if (counter + 1 >= size) {
                        size *= 2;
                        entries = realloc(entries, size * sizeof(Entry));
                    }

                    entries[counter].park_name = (char *) malloc(
                            strlen(crtPark->name) + 1);
                    strcpy(entries[counter].park_name, crtPark->name);
                    entries[counter].park_name[strlen(crtPark->name)] = '\0';
                    entries[counter].entry_day = crtCar->entry_date.day;
                    entries[counter].entry_month = crtCar->entry_date.month;
                    entries[counter].entry_year = crtCar->entry_date.year;
                    entries[counter].entry_hour = crtCar->entry_time.hour;
                    entries[counter].entry_minute = crtCar->entry_time.minute;
                    entries[counter].exit_day = crtCar->exit_date.day;
                    entries[counter].exit_month = crtCar->exit_date.month;
                    entries[counter].exit_year = crtCar->exit_date.year;
                    entries[counter].exit_hour = crtCar->exit_time.hour;
                    entries[counter].exit_minute = crtCar->exit_time.minute;
                    entries[counter].headCar_in_park =
                            crtCar->mainCar_in_park; // 1st entry still inside
                    entries[counter].entry_in_park = 0;
                    entries[counter].compare_exit = 0;
                    counter++;

                    // If car is found in hashtable, we go through every
                    // one of its entries also in the hashtable
                    CarEntry *head_entry = crtCar->head_entries;
                    while (head_entry != NULL) {
                        if (counter + 1 >= size) {
                            size *= 2;
                            entries = realloc(entries,
                                              size * sizeof(Entry));
                        }
                        entries[counter].park_name = (char *) malloc(
                                strlen(crtPark->name) + 1);
                        strcpy(entries[counter].park_name, crtPark->name);
                        entries[counter].entry_day=
                                head_entry->entry_date.day;
                        entries[counter].entry_month=
                                head_entry->entry_date.month;
                        entries[counter].entry_year=
                                head_entry->entry_date.year;
                        entries[counter].entry_hour=
                                head_entry->entry_time.hour;
                        entries[counter].entry_minute=
                                head_entry->entry_time.minute;
                        entries[counter].exit_day = head_entry->exit_date.day;
                        entries[counter].exit_month=
                                head_entry->exit_date.month;
                        entries[counter].exit_year =
                                head_entry->exit_date.year;
                        entries[counter].exit_hour =
                                head_entry->exit_time.hour;
                        entries[counter].exit_minute =
                                head_entry->exit_time.minute;
                        entries[counter].headCar_in_park = 0;
                        entries[counter].entry_in_park =
                                head_entry->entry_in_park; // Check for each
                        // entry
                        entries[counter].compare_exit = 0;
                        counter++;
                        head_entry = head_entry->next_entry;
                    }
                }
                crtCar = crtCar->next_hashtable;
            }
        }
        crtPark = crtPark->next;

    }

    // Sort entries by park_name and date/time
    mergeSort(entries, 0, counter - 1);

    // entry_in_park tells us if that entry is in the park at the moment,
    // if it is we can't print its exit information
    for (int i = 0; i < counter; i++) {
        if (entries[i].entry_in_park == 0
            && entries[i].headCar_in_park == 0){
            printf("%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d\n",
                   entries[i].park_name,
                   entries[i].entry_day, entries[i].entry_month,
                   entries[i].entry_year, entries[i].entry_hour,
                   entries[i].entry_minute,
                   entries[i].exit_day, entries[i].exit_month,
                   entries[i].exit_year, entries[i].exit_hour,
                   entries[i].exit_minute);
        } else {
            printf("%s %02d-%02d-%04d %02d:%02d\n", entries[i].park_name,
                   entries[i].entry_day, entries[i].entry_month,
                   entries[i].entry_year, entries[i].entry_hour,
                   entries[i].entry_minute);
        }

    }
    for (int i = 0; i < counter; i++)
        free(entries[i].park_name);
    free(entries);
}

/**
 * @brief e function.
 *
 * @param park_list A pointer to the linked list
 * @param last_entry Date/time of last action
 *
 * @return New last_entry
 * @details Registers an entry of a car
 */
int e(List* park_list, int last_entry){
    char * name_aux;
    char license_plate[9];
    int day, month, year, minute, hours, error;
    getchar();
    read_park_name(&name_aux);
    scanf("%s %d-%d-%d %d:%d", license_plate, &day, &month,
          &year, &hours, &minute);

    error = CheckEntryErrors(park_list->head, name_aux,
                             license_plate, day, month, year,
                             hours, minute, last_entry);

    if (!error){
        register_car(park_list->head, name_aux, license_plate, day,
                     month, year, hours, minute);
        last_entry = conversor(day, month, year, hours, minute);
    }
    getchar();
    free(name_aux);
    return last_entry;
}

/**
 * @brief s function.
 *
 * @param park_list A pointer to the linked list
 * @param last_entry Date/time of last action
 *
 * @return New last_entry
 * @details Registers an exit of a car
 */
int s(List* park_list, int last_entry){
    char * name_aux;
    char license_plate[9];
    int day, month, year, minute, hours, error;
    getchar();
    read_park_name(&name_aux);
    scanf("%s %d-%d-%d %d:%d", license_plate, &day, &month, &year, &hours, &minute);

    error = CheckExitErrors(park_list->head, name_aux,
                            license_plate, day, month, year,
                            hours, minute, last_entry);
    if (!error){
        register_exit(park_list->head, name_aux, license_plate, day,
                      month, year, hours, minute);
        last_entry = conversor(day, month, year, hours, minute);
    }
    getchar();
    free(name_aux);
    return last_entry;
}


/**
 * @brief v function.
 *
 * @param park_list A pointer to the linked list
 *
 * @details Shows entries/exits of a car in all parks
 */
void v(List* park_list){
    char license_plate[9];
    int error, car_found = 0;
    getchar();
    scanf("%s", license_plate);
    error = CheckLicPlate(license_plate);
    if (error){
        int index = hash(license_plate);
        Park *CrntPark = park_list->head;
        while (CrntPark != NULL) {
            if (CrntPark->hashtable[index] != NULL) {
                Car *CrntCar = (Car *) CrntPark->hashtable[index];
                while (CrntCar != NULL) {
                    if (strcmp(CrntCar->licensePlate, license_plate) == 0)
                        car_found = 1;
                    CrntCar = (Car *) CrntCar->next_hashtable;
                }
            }
            CrntPark = CrntPark->next;
        }
        if (!car_found) {
            printf("%s: no entries found in any parking.\n", license_plate);
        } else
            list_entries(park_list->head, license_plate);
    }
    getchar();
}
