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

void	thrd(pthread_t *thread, void *(*foo)(void *),
			void *data, t_opcode opcode)
{
	if (opcode == CREATE)
		thread_checker(pthread_create(thread, NULL, foo, data), opcode);
	else if (opcode == JOIN)
		thread_checker(pthread_join(*thread, NULL), opcode);
	else if (opcode == DETACH)
		thread_checker(pthread_detach(*thread), opcode);
	else
		error_print("erong opcode for thread");
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

void	clean(t_table *table)
{
	t_philosopher	*philo;
	int				i;

	i = 0;
	while (i < table->nbr_of_philos)
	{
		philo = &table->philosophers[i];
		mtx(&philo->philo_mutex, DESTROY);
		mtx(&table->forks[i].fork, DESTROY);
		i++;
	}
	mtx(&table->output_mutex, DESTROY);
	mtx(&table->table_mutex, DESTROY);
	free(table->forks);
	free(table->philosophers);
}
