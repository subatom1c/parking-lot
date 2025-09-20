/**
 * @file hashtable.h
 * @brief Function prototypes about hashtable of cars
 */

// Includes
#include <stdio.h>

// Function prototypes
unsigned int hash(const char* licensePlate);
void init_hashtable(Park * park);
int insert(Park *park, Car *carAdd);
int car_inside_park(Park *parkVerify, const char * licensePlate);