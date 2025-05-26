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

static int	init_forks(t_table *t, int i)
{
	if (i >= t->nbr_of_philos)
		return (0);
	if (mtx(&t->forks[i].fork, INIT) != 0)
	{
		if (i > 0)
			init_forks_destroy(t, i - 1);
		return (1);
	}
	t->forks[i].fork_id = i;
	return (init_forks(t, i + 1));
}

void	init_forks_destroy(t_table *t, int i)
{
	if (i < 0)
		return ;
	mtx(&t->forks[i].fork, DESTROY);
	init_forks_destroy(t, i - 1);
}

int	data_init(t_table *t)
{
	t->stop = 0;
	t->ready = 0;
	t->nbr_of_threads = 0;
	t->philosophers = mlc(sizeof(t_philosopher) * t->nbr_of_philos);
	if (!t->philosophers)
		return (error_print("malloc error"));
	t->forks = mlc(sizeof(t_fork) * t->nbr_of_philos);
	if (!t->forks)
		return (free(t->philosophers), error_print("malloc error"));
	if (mtx(&t->table_mutex, INIT) != 0
		|| mtx(&t->output_mutex, INIT) != 0
		|| init_forks(t, 0))
	{
		mtx(&t->output_mutex, DESTROY);
		mtx(&t->table_mutex, DESTROY);
		free(t->philosophers);
		free(t->forks);
		return (error_print("mutex initialization error"));
	}
	philo_init(t);
	return (0);
}
