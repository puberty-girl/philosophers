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

	if (ready_check(&philosopher->philo_mutex, philosopher->isfull))
		return 0;
	mtx(&philosopher->philo_mutex, LOCK);
	last_meal = philosopher->last_meal_time;
	mtx(&philosopher->philo_mutex, UNLOCK);
	elapsed = get_time(MILLISECOND) - last_meal;
	time_to_die = philosopher->table->time_to_die;
	if (elapsed > time_to_die)
		return 1;
	return 0;
}

void    *check_death(void *data)
{
	t_table *table;
	int		i; 

	table = (t_table *)data;
	while(!all_threads_are_running(&table->table_mutex, &table->nbr_of_threads, 
		table->nbr_of_philos))
		;
	while(!ready_check(&table->table_mutex, table->stop))
	{
		i = 0;
		while (i++ < table->nbr_of_philos && ready_check(&table->table_mutex, table->stop) == 0)
		{
			if (isdead(table->philosophers + i))
			{
				mtx(&table->table_mutex, LOCK);
				table->stop = 1;
				mtx(&table->table_mutex, UNLOCK);
				print_status(DIES, table->philosophers + i, 0);
			}
		}
	}
	return NULL;
}
