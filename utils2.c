/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-20 18:02:01 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-20 18:02:01 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	thrd(pthread_t *thread, void *(*start_routine)(void *), 
			void *arg, t_opcode opcode)
{
	int	status;
	pthread_attr_t attr;

	if (opcode == CREATE)
	{
		pthread_attr_init(&attr);
		pthread_attr_setstacksize(&attr, 64 * 1024); // 64KB stack to avoid memory exhaustion
		status = pthread_create(thread, &attr, start_routine, arg);
		pthread_attr_destroy(&attr);
	}
	else if (opcode == JOIN)
		status = pthread_join(*thread, NULL);
	else if (opcode == DETACH)
		status = pthread_detach(*thread);
	else
	{
		error_print("wrong opcode for thread");
		return (1);
	}
	thread_checker(status, opcode);
	return (status);
}

long	get_time(t_time_code time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error_print("gettimeofday failed");
	if (time_code == SECOND)
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (time_code == MILLISECOND)
		return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
	else if (time_code == MICROSECOND)
		return ((tv.tv_sec * 1e6) + tv.tv_usec);
	else
		error_print("wrong gettime input");
	return (4242);
}

void	increase_long(pthread_mutex_t *mutex, long *value)
{
	mtx(mutex, LOCK);
	(*value)++;
	mtx(mutex, UNLOCK);
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long	ret;

	mtx(mutex, LOCK);
	ret = *value;
	mtx(mutex, UNLOCK);
	return (ret);
}

void	clean(t_table *table)
{
	int i;

	if (!table)
		return;
	if (mtx(&table->table_mutex, LOCK) == 0)
	{
		table->stop = 1;
		mtx(&table->table_mutex, UNLOCK);
	}
	if (table->philosophers)
	{
		for (i = 0; i < table->nbr_of_philos; i++)
		{
			if (table->philosophers[i].therad_id && !table->philosophers[i].thread_joined)
			{
				thrd(&table->philosophers[i].therad_id, NULL, NULL, JOIN);
				table->philosophers[i].thread_joined = 1;
			}
		}
	}
	if (table->dead_checker && !table->death_checker_joined)
	{
		thrd(&table->dead_checker, NULL, NULL, JOIN);
		table->death_checker_joined = 1;
	}
	if (table->philosophers)
		free(table->philosophers);
	if (table->forks)
		free(table->forks);
}
