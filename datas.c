/**
 * @file datas.c
 * @brief Includes functions about dates
 * @author ist109872
*/

// Includes
#include "parque.h"

// Defines:
/**
 * NUMBER_15MIN_DAILY: Number of 15mins in a day
*/
#define NUMBER_15MIN_DAILY 96


/**
 * @brief numberOfDays(1 argument) Calculates number of days based on month
 *
 * @param month Month
 *
 * @return Number of days that month has, -1 if invalid
 */
int numberOfDays(int month) {
    switch(month) {
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        case 2:
            return 28;
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        default:
            return -1;
    }
}

/**
 * @brief conversor(5 arguments) Coverts time into minutes from reference point
 *
 * @param day day
 * @param month Month
 * @param year year
 * @param hour hour
 * @param minute minute
 *
 * @return Number of minutes from start point (1/1/2024)
 */
int conversor(int day, int month, int year, int hour, int minute) {
    int total;
    total = (year - 2024) * 365 * 24 * 60;

    for (int i = 1; i < month; i++) {
        total += numberOfDays(i) * 24 * 60;
    }

    total += (day - 1) * 24 * 60;
    total += hour * 60 + minute;

    return total;
}


/**
 * @brief verifyDate(5 arguments) Verifies if date is valid
 *
 * @param day day
 * @param month Month
 * @param year year
 * @param hour hour
 * @param minute minute
 *
 * @return Validation
 */
int verifyDate(int day, int month, int year, int hour, int minute){
    if (day > numberOfDays(month) || day <= 0)
        return 0;
    if (month <= 0 || month > 12)
        return 0;
    if (year < 0)
        return 0;
    if (hour < 0 || hour >= 24)
        return 0;
    if (minute < 0 || minute >= 60)
        return 0;

    return 1;
}


/**
 * @brief minuteDiff(10 arguments) Calculates minute difference between 2 dates
 *
 * @param entry_day entry_day
 * @param entry_month entry_month
 * @param entry_year entry_year
 * @param entry_hour entry_hour
 * @param entry_minute entry_minute
 * @param exit_day exit_day
 * @param exit_month exit_month
 * @param exit_year exit_year
 * @param exit_hour exit_hour
 * @param exit_minute exit_minute
 *
 * @return Minute difference between dates/times
 */
int minuteDiff(int entry_day, int entry_month, int entry_year, int entry_hour,
               int entry_minute,int exit_day, int exit_month, int exit_year,
               int exit_hour, int exit_minute) {

    int count_entry = (entry_day) * 24 * 60;
    int count_exit = (exit_day) * 24 * 60;
    // Counts number of days through months
    for (int i = 1; i < entry_month; i++){
        count_entry += numberOfDays(i) * 24 * 60;
    }

    for (int i = 1; i < exit_month; i++){
        count_exit += numberOfDays(i) * 24 * 60;
    }
    // Calculates difference
    count_exit += (exit_year - entry_year) * 365 * 24 * 60 +
                  exit_hour * 60 + exit_minute;
    count_entry += entry_hour * 60 + entry_minute;
    return count_exit - count_entry;
}

/**
 * @brief minuteDiff(11 arguments) Calculates revenue of a car stay
 *
 * @param park Park that car is in
 * @param entry_day entry_day
 * @param entry_month entry_month
 * @param entry_year entry_year
 * @param entry_hour entry_hour
 * @param entry_minute entry_minute
 * @param exit_day exit_day
 * @param exit_month exit_month
 * @param exit_year exit_year
 * @param exit_hour exit_hour
 * @param exit_minute exit_minute
 *
 * @return Revenue
 */
float calculate_revenue(Park *park, int entry_day, int entry_month,
                        int entry_year, int entry_hour, int entry_minute,
                        int exit_day, int exit_month, int exit_year,
                        int exit_hour, int exit_minute){

    int minutes = minuteDiff(entry_day, entry_month, entry_year, entry_hour,
                             entry_minute,
                             exit_day, exit_month, exit_year, exit_hour,
                             exit_minute);


    float revenue = 0, parcial_revenue, minutes_aux;
    int number15min = minutes / 15;
    minutes_aux = minutes % 15; // Rest of minutes

    // Car stayed more that a day
    if (number15min >= NUMBER_15MIN_DAILY){
        revenue = (number15min / NUMBER_15MIN_DAILY) * park->max_daily_val;
        number15min = number15min % NUMBER_15MIN_DAILY;
    }

    // If Car stayed more than one hour
    if (number15min >= 4) {
        if (minutes_aux == 0)
            parcial_revenue = (number15min - 4) * park->val_15_after_hour +
                              4 * park->val_first_15;
        else
            parcial_revenue = (1 + number15min - 4) * park->val_15_after_hour +
                              4 * park->val_first_15;

    }else {
        if (minutes_aux == 0)
            parcial_revenue = number15min * park->val_first_15;
        else
            parcial_revenue = (1 + number15min) * park->val_first_15;
    }
    if (parcial_revenue > park->max_daily_val)
        parcial_revenue = park->max_daily_val;

    return revenue + parcial_revenue;
}