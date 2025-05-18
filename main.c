#include "philosophers.h"

int main(int argc, char **argv)
{   
    t_table table;
    if (argc == 5 || argc == 6)
    {
        parse_init(&table, argv);
        data_init(&table);
        dinner_start(&table);
        clean(&table); //if stop == 1, otherwise its an endless loop
    }
    else
        error_print("valid input format is: number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n");
    
}