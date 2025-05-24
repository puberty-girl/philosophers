/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-20 17:19:11 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-20 17:19:11 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	all_threads_are_running(pthread_mutex_t *mutex,
	long *threads, long nbr_philos)
{
	int	res;

	res = 0;
	mtx(mutex, LOCK);
	if (*threads == nbr_philos)
		res = 1;
	mtx(mutex, UNLOCK);
	return (res);
}

int	get_last_meal_and_check_full(t_philosopher *philosopher, long *last_meal)
{
	int	full;

	mtx(&philosopher->philo_mutex, LOCK);
	full = philosopher->isfull;
	*last_meal = philosopher->last_meal_time;
	mtx(&philosopher->philo_mutex, UNLOCK);
	if (full)
		return (1);
	if (*last_meal == 0)
		*last_meal = philosopher->table->start_time;
	return (0);
}

int	declare_death(t_philosopher *philosopher)
{
	mtx(&philosopher->table->table_mutex, LOCK);
	if (philosopher->table->stop == 0)
	{
		philosopher->table->stop = 1;
		mtx(&philosopher->table->table_mutex, UNLOCK);
		print_status(DIES, philosopher);
		return (1);
	}
	mtx(&philosopher->table->table_mutex, UNLOCK);
	return (0);
}

int	isdead(t_philosopher *philosopher)
{
	long	elapsed;
	long	time_to_die;
	long	last_meal;
	long	current_time;

	if (get_last_meal_and_check_full(philosopher, &last_meal))
		return (0);
	current_time = get_time(MICROSECOND);
	elapsed = current_time - last_meal;
	time_to_die = philosopher->table->time_to_die;
	if (elapsed >= time_to_die)
		return (declare_death(philosopher));
	return (0);
}

void	*check_death(void *data)
{
	t_table	*table;
	int		i;

	table = (t_table *)data;
	while (!all_threads_are_running(&table->table_mutex, &table->nbr_of_threads,
			table->nbr_of_philos))
		ft_usleep(500, table);
	while (!ready_check(&table->table_mutex, table->stop))
	{
		i = 0;
		while (i < table->nbr_of_philos
			&& !ready_check(&table->table_mutex, table->stop))
		{
			if (isdead(&table->philosophers[i]))
				return (NULL);
			i++;
		}
		ft_usleep(500, table);
	}
	return (NULL);
}
