#include "philosophers.h"

int	all_threads_are_running(pthread_mutex_t *mutex, long *threads, long nbr_philos)
{
	int	res;

	res = 0;
	mtx(mutex, LOCK);
	if (*threads == nbr_philos)
		res = 1;
	mtx(mutex, UNLOCK);
	return (res);
}

int	isdead(t_philosopher *philosopher)
{
	long	elapsed;
	long	time_to_die;
	long	last_meal;
	long	current_time;
	int		full;

	mtx(&philosopher->philo_mutex, LOCK);
	full = philosopher->isfull;
	mtx(&philosopher->philo_mutex, UNLOCK);

	if (full)
		return 0;
		
	mtx(&philosopher->philo_mutex, LOCK);
	last_meal = philosopher->last_meal_time;
	mtx(&philosopher->philo_mutex, UNLOCK);
	
	current_time = get_time(MICROSECOND);
	if (last_meal == 0)  // Never eaten
		last_meal = philosopher->table->start_time;
	
	elapsed = current_time - last_meal;
	time_to_die = philosopher->table->time_to_die;
	
	// If time_to_eat is greater than time_to_die, they will die during their meal
	if (philosopher->table->time_to_eat > time_to_die && elapsed >= time_to_die)
	{
		mtx(&philosopher->table->table_mutex, LOCK);
		philosopher->table->stop = 1;
		mtx(&philosopher->table->table_mutex, UNLOCK);
		print_status(DIES, philosopher, 0);  // This prints proper elapsed time
		return 1;
	}

	
	// Normal death check
	if (elapsed >= time_to_die)
	{
    mtx(&philosopher->table->table_mutex, LOCK);
    philosopher->table->stop = 1;
    mtx(&philosopher->table->table_mutex, UNLOCK);
    print_status(DIES, philosopher, 0);  // <- this prints correct time
    return 1;
	}
	return 0;
}

void    *check_death(void *data)
{
	t_table *table;
	int		i; 

	table = (t_table *)data;
	while(!all_threads_are_running(&table->table_mutex, &table->nbr_of_threads, 
		table->nbr_of_philos))
		ft_usleep(50, table);  // Check more frequently
	
	while(!ready_check(&table->table_mutex, table->stop))
	{
		i = 0;
		while (i < table->nbr_of_philos && !ready_check(&table->table_mutex, table->stop))
		{
			if (isdead(&table->philosophers[i]))
				return NULL;
			i++;
		}
		ft_usleep(50, table);  // Check more frequently
	}
	return NULL;
}
