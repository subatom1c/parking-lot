/**
 * @file hashtable.c
 * @brief Function about hashtable of cars
 * @author ist109872
 */

// Includes
#include <stdio.h>
#include "parque.h"
#include "carros.h"
#include "datas.h"
#include <string.h>

// Defines
/**
 * HASHSIZE: Hashtable size
 * PRIME: Prime number
*/
#define HASHSIZE 53
#define PRIME 31


/**
 * @brief hash(1 argument) Calculates hashtable key
 *
 * @param licensePlate licensePlate of a car
 *
 * @return unsigned int representing a key
 */
unsigned int hash(const char* licensePlate){
    unsigned int key = 0;
    for (int i = 0; licensePlate[i] != '\0'; i++)
        if (licensePlate[i] != '-'){
            key += licensePlate[i];
            key *= licensePlate[i];
            key *= PRIME;
        }
    return key % HASHSIZE;
}


/**
 * @brief init_hashtable(1 argument) Initializes hashtable
 *
 * @param park Park that hashtable is going to be initialized
 */
void init_hashtable(Park * park){
    for (int i = 0; i < HASHSIZE; i++){
        park->hashtable[i] = NULL;
    }
}


/**
 * @brief insert(2 arguments) Tries to insert a car into a hashtable
 *
 * @param park Park that hashtable is going to be initialized
 * @param carAdd Car to be added
 *
 * @return Car was inserted
 */
int insert(Park *park, Car *carAdd) {
    unsigned int index = hash(carAdd->licensePlate);

    // If this index is available we insert the car and initialize variables
    if (park->hashtable[index] == NULL) {
        park->hashtable[index] = carAdd;
        park->hashtable[index]->head_entries = NULL;
        park->hashtable[index]->next_hashtable = NULL;
        return 1;
    }
    return 0;
}

/**
 * @brief car_inside_park(2 arguments) Checks if a car is inside a park
 *
 * @param parkVerify Park to be verified
 * @param licensePlate Car's license plate
 *
 * @return Car is inside the park
 */
int car_inside_park(Park *parkVerify, const char * licensePlate){
    int index = hash(licensePlate);
    Park * crtPark = parkVerify;

    while (crtPark != NULL){
        if (crtPark->hashtable[index] != NULL){
            Car * crtCar = crtPark->hashtable[index];
            while (crtCar != NULL){
                if (strcmp(crtCar->licensePlate, licensePlate) == 0){
                    if (crtCar->head_entries == NULL)
                        return crtCar->mainCar_in_park;
                    return crtCar->car_in_park;
                }

                crtCar = crtCar->next_hashtable;
            }
        }
        crtPark = crtPark->next;
    }
    return 0;
}