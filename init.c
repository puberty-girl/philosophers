#include "philosophers.h"

void assign_forks(t_philosopher *philosopher, t_fork *forks, int ph_position)
{
	int philo_nbr;

	philo_nbr = philosopher->table->nbr_of_philos;
	philosopher->first_fork = &forks[(ph_position + 1) % philo_nbr];
	philosopher->second_fork = &forks[ph_position];
	if (philosopher->philo_id % 2 == 0)
	{
		philosopher->first_fork = &forks[ph_position];
		philosopher->second_fork = &forks[(ph_position + 1) % philo_nbr];
	}
}					

void	philo_init(t_table *table)
{
	int i;

	i = -1;
	while (i++ < table->nbr_of_philos)
	{
		table->philosophers[i].philo_id = i;
		table->philosophers[i].isfull = 0;
		table->philosophers[i].meals_consumed = 0;
		table->philosophers[i].last_meal_time = 0;
		table->philosophers[i].table = table;
		mtx(&table->philosophers[i].philo_mutex, INIT);
		assign_forks(&table->philosophers[i], table->forks, i);
	}
	
}

void    data_init(t_table *table)
{
	int	i;

	i = -1;
	table->stop = 0;
	table->ready = 0;
	table->nbr_of_threads = 0;
    table->philosophers = mlc(sizeof(t_philosopher) * table->nbr_of_philos);
	table->forks = mlc(sizeof(t_fork) * table->nbr_of_philos);
	mtx(&table->table_mutex, INIT);
	mtx(&table->output_mutex, INIT);
	while (i++ < table->nbr_of_philos)
	{
		mtx(&table->forks[i].fork, INIT);
		table->forks[i].fork_id = i;
	}
	philo_init(table);
}