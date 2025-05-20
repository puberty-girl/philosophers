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

void	desync(t_philosopher *philosopher)
{
	if (philosopher->table->nbr_of_philos % 2 == 0)
	{
		if (philosopher->philo_id % 2 == 0)
			ft_usleep(1000, philosopher->table);
	}
	else
	{
		if (philosopher->philo_id % 2 != 0)
			think(philosopher, 1);
	}
}

void	*dinner_simulation(void *data)
{
	t_philosopher *philosopher;

	philosopher = (t_philosopher *)data;
	sync_threads(philosopher->table);
	increase_long(&philosopher->table->table_mutex, &philosopher->table->nbr_of_threads);
	mtx(&philosopher->philo_mutex, LOCK);
	philosopher->last_meal_time = philosopher->table->start_time;
	mtx(&philosopher->philo_mutex, UNLOCK);
	desync(philosopher);
	while(!ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
	{
		if (philosopher->isfull)
			break;
		eat(philosopher);
		if (ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
			break;
		print_status(SLEEPING, philosopher, 0);
		ft_usleep(philosopher->table->time_to_sleep, philosopher->table);
		if (ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
			break;
		think(philosopher, 0);
	}
	return NULL;
}

void	dinner_start(t_table *table)
{
	int	i;

	i = 0;

	table->start_time = get_time(MICROSECOND);

	if (table->must_eat == 0)
		return;
	else if (table->nbr_of_philos == 1)
		thrd(&table->philosophers[0].therad_id, only_philo, &table->philosophers[0], CREATE);
	else
	{
		while(i < table->nbr_of_philos)
		{
			thrd(&table->philosophers[i].therad_id, dinner_simulation, 
				&table->philosophers[i], CREATE);
			i++;
		}
	}

	thrd(&table->dead_checker, check_death, table, CREATE);

	//start of simulation
	mtx(&table->table_mutex, LOCK);
	table->ready = 1;
	mtx(&table->table_mutex, UNLOCK);
	i = 0;
	while (i < table->nbr_of_philos)
	{
		thrd(&table->philosophers[i].therad_id, NULL, NULL, JOIN);
		i++;
	}
	mtx(&table->table_mutex, LOCK);
	table->stop = 1;
	mtx(&table->table_mutex, UNLOCK);
	thrd(&table->dead_checker, NULL, NULL, JOIN);
}