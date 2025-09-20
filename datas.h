/**
 * @file datas.h
 * @brief Includes prototypes functions about dates
 * @author ist109872
*/

// Includes
#include <stdio.h>

// Function Prototypes
int numberOfDays(int month);
int conversor(int day, int month, int year, int hour, int minute);
int verifyDate(int day, int month, int year, int hour, int minute);
float calculate_revenue(Park *park, int entry_day, int entry_month,
                        int entry_year, int entry_hour, int entry_minute,
                        int exit_day, int exit_month, int exit_year,
                        int exit_hour, int exit_minute);
int minuteDiff(int entry_day, int entry_month, int entry_year,
               int entry_hour, int entry_minute,
               int exit_day, int exit_month, int exit_year,
               int exit_hour, int exit_minute);

