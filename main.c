/* iaed24 - ist1109872 - project */
/**
 * @file project.c
 * @brief Includes the main function
 * @author ist109872
 */

// Includes
#include <stdio.h>
#include "parque.h"
#include "carros.h"
#include "hashtable.h"
#include "datas.h"
#include "sorting.h"
#include <stdlib.h>

// Defines
/**
 * MAXPARQUES: Max number of parks
*/
#define MAXPARQUES 20

/**
* Reads characters from stdin and runs conrresponding functions
   @param running flag that runs the program
   @param parks_number number of parks in a system
   @param park_list List with head and tail of a parks
   @param last_entry Date/time of last action
   @return Always returns 0
*/
int main(){
    int character, running = 1, parks_number = 0, last_entry = 0;
    List * park_list = (List*) malloc(sizeof (List));
    park_list->head = NULL;
    park_list->tail = NULL;
    while(running) {
        character= getchar();
        switch (character) {
            case 'q': q(park_list);
                running = 0;
                break;
            case 'p': parks_number = p(park_list, parks_number);
                break;
            case 'e': last_entry = e(park_list, last_entry);
                break;
            case 's': last_entry = s(park_list, last_entry);
                break;
            case 'v': v(park_list);
                break;
            case 'f': f(park_list, last_entry);
                break;
            case 'r': parks_number = r(park_list, parks_number);
                break;
        }
    }
    return 0;
}
