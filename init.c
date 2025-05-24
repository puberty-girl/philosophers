/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-20 17:19:22 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-20 17:19:22 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	assign_forks(t_philosopher *philosopher, t_fork *forks, int ph_position)
{
	int	philo_nbr;

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
	int	i;

	i = 0;
	while (i < table->nbr_of_philos)
	{
		table->philosophers[i].philo_id = i + 1;
		table->philosophers[i].isfull = 0;
		table->philosophers[i].meals_consumed = 0;
		table->philosophers[i].last_meal_time = 0;
		table->philosophers[i].table = table;
		table->philosophers[i].thread_joined = 0;
		mtx(&table->philosophers[i].philo_mutex, INIT);
		assign_forks(&table->philosophers[i], table->forks, i);
		i++;
	}
	table->death_checker_joined = 0;
}

int	data_init(t_table *table)
{
	int	i;

	i = 0;
	table->stop = 0;
	table->ready = 0;
	table->nbr_of_threads = 0;
	table->philosophers = mlc(sizeof(t_philosopher) * table->nbr_of_philos);
	if (!table->philosophers)
		return (error_print("malloc error"));
	table->forks = mlc(sizeof(t_fork) * table->nbr_of_philos);
	if (!table->forks)
	{
		free(table->philosophers);
		return (error_print("malloc error"));
	}
	if (mtx(&table->table_mutex, INIT) != 0)
	{
		free(table->philosophers);
		free(table->forks);
		return (error_print("mutex initialization error"));
	}
	if (mtx(&table->output_mutex, INIT) != 0)
	{
		mtx(&table->table_mutex, DESTROY);
		free(table->philosophers);
		free(table->forks);
		return (error_print("mutex initialization error"));
	}
	while (i < table->nbr_of_philos)
	{
		if (mtx(&table->forks[i].fork, INIT) != 0)
		{
			while (--i >= 0)
				mtx(&table->forks[i].fork, DESTROY);
			mtx(&table->output_mutex, DESTROY);
			mtx(&table->table_mutex, DESTROY);
			free(table->philosophers);
			free(table->forks);
			return (error_print("mutex initialization error"));
		}
		table->forks[i].fork_id = i;
		i++;
	}
	philo_init(table);
	return (0);
}
