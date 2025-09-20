/**
 * @file carros.h
 * @brief Definitions and function prototypes about cars
 * @author ist109872
 */

// Includes
#include <stdio.h>

/**
 * @struct Date
 * @brief Represents a Date
 */
typedef struct Date {
    int day, month, year;
} Date;

/**
 * @struct Time
 * @brief Represents a time
 */
typedef struct Time{
    int hour, minute;
} Time;

/**
 * @struct CarEntry
 * @brief Represents an entry of a car
 *
 * This structure represents an entry of a car after the first one,
 * used in the hashtable in case of many entries of the same car in the
 * same park
 */
typedef struct CarEntry {
    int entry_in_park; // Flag if certain entry is still inside a park
    struct Date entry_date, exit_date;
    struct Time entry_time, exit_time;
    struct CarEntry * next_entry; // Pointer to next entry
} CarEntry;

/**
 * @struct Car
 * @brief Represents a Car
 *
 * This structure has all the necessary information about a Car,
 * including its licensePlate, flags, linked lists and dates.
 */
typedef struct Car{
    char licensePlate[9];
    int car_in_park; // Flag if any entry is still inside the park
    int mainCar_in_park; // Flag if mainCar is in park
    struct Car * next_hashtable; // Pointer to next car in hashtable (Colision)
    Date entry_date, exit_date;
    Time entry_time, exit_time;
    CarEntry * head_entries; // Pointer for an entry of this same car
} Car;

// Function prototypes
void register_car(Park * head, const char * park_name,const char* licensePlate,
                  int day, int month, int year, int hour, int minute);
int CheckEntryErrors(Park * head, const char * park_name,
                     const char*licensePlate, int day, int month, int year,
                     int hour, int minute, int lastEntry);
void register_exit(Park * head, const char * park_name,
                   const char * licensePlate, int day, int month, int year,
                   int hour, int minute);
int CheckLicPlate(const char* licensePlate);
int CheckExitErrors(Park * head, const char * park_name,
                    const char* licensePlate, int day, int month, int year,
                    int hour, int minute, int last_entry);
void list_entries(Park * head , const char * matricula);
int e(List* park_list, int last_entry);
int s(List* park_list, int last_entry);
void v(List* park_list);