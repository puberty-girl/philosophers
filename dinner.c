#include "philosophers.h"

int	ready_check(pthread_mutex_t *mutex, int value)
{
	int	ret;

	mtx(mutex, LOCK);
	ret = value;
	mtx(mutex, UNLOCK);
	return(ret);
}


void sync_threads(t_table *table)
{
	while(!ready_check(&table->table_mutex, table->ready))
		;
}

void	*dinner_simulation(void *data)
{
	t_philosopher *philosopher;

	philosopher = (t_philosopher *)data;
	sync_threads(philosopher->table); //wait_all_threads
	mtx(&philosopher->philo_mutex, LOCK);
	philosopher->last_meal_time = get_time(MILLISECOND);
	mtx(&philosopher->philo_mutex, UNLOCK);
	increase_long(&philosopher->table->table_mutex, &philosopher->table->nbr_of_threads);
	while(!ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
	{
		if (philosopher->isfull)
			break;
		eat(philosopher);
		print_status(SLEEPING, philosopher, 0); ///here 0 is for debug
		ft_usleep(philosopher->table->time_to_sleep, philosopher->table);
	}
	return NULL;
}

void	dinner_start(t_table *table)
{
	int	i;

	i = 0;
	if (table->must_eat == 0)
		return;
	else if (table->nbr_of_philos == 1)
		thrd(&table->philosophers[0].therad_id, only_philo, &table->philosophers[0], CREATE);
	else
	{
		while(i++ < table->nbr_of_philos)
			thrd(&table->philosophers[i].therad_id, dinner_simulation, 
				&table->philosophers[i], CREATE);
	}

	thrd(&table->dead_checker, check_death, table, CREATE);

	//start of simulation
	table->start_time = get_time(MILLISECOND);
	mtx(&table->table_mutex, LOCK);
	table->ready = 1;
	mtx(&table->table_mutex, UNLOCK);
	i = 0;
	while (i++ < table->nbr_of_philos)
		thrd(&table->philosophers[i].therad_id, NULL, NULL, JOIN);
	mtx(&table->table_mutex, LOCK);
	table->stop = 1;
	mtx(&table->table_mutex, UNLOCK);
	thrd(&table->dead_checker, NULL, NULL, JOIN);
}