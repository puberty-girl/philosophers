/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-20 17:19:16 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-20 17:19:16 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ready_check(pthread_mutex_t *mutex, int *value)
{
	int	ret;

	mtx(mutex, LOCK);
	ret = *value;
	mtx(mutex, UNLOCK);
	return (ret);
}

void	sync_threads(t_table *table)
{
	while (!ready_check(&table->table_mutex, &table->ready))
		usleep(10);
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

void	prepare_philosopher(t_philosopher *philosopher)
{
	sync_threads(philosopher->table);
	increase_long(&philosopher->table->table_mutex,
		&philosopher->table->nbr_of_threads);
	mtx(&philosopher->philo_mutex, LOCK);
	mtx(&philosopher->table->table_mutex, LOCK);
	philosopher->last_meal_time = philosopher->table->start_time;
	mtx(&philosopher->table->table_mutex, UNLOCK);
	mtx(&philosopher->philo_mutex, UNLOCK);
	desync(philosopher);
}

void	philosopher_loop(t_philosopher *philosopher)
{
	while (!ready_check(&philosopher->table->table_mutex,
			&philosopher->table->stop))
	{
		if (philosopher->isfull)
			break ;
		eat(philosopher);
		if (ready_check(&philosopher->table->table_mutex,
				&philosopher->table->stop))
			break ;
		print_status(SLEEPING, philosopher);
		ft_usleep(philosopher->table->time_to_sleep, philosopher->table);
		if (ready_check(&philosopher->table->table_mutex,
				&philosopher->table->stop))
			break ;
		think(philosopher, 0);
		if (philosopher->table->must_eat > 0
			&& philosopher->meals_consumed == philosopher->table->must_eat)
		{
			mtx(&philosopher->philo_mutex, LOCK);
			philosopher->isfull = 1;
			mtx(&philosopher->philo_mutex, UNLOCK);
		}
	}
}
