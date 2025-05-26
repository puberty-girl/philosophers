/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-20 18:20:10 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-20 18:20:10 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*dinner_simulation(void *data)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)data;
	prepare_philosopher(philosopher);
	philosopher_loop(philosopher);
	return (NULL);
}

void	start_threads(t_table *table)
{
	int	i;

	if (table->must_eat == 0)
		return ;
	else if (table->nbr_of_philos == 1)
	{
		thrd(&table->philosophers[0].therad_id,
			only_philo, &table->philosophers[0], CREATE);
	}
	else
	{
		i = 0;
		while (i < table->nbr_of_philos)
		{
			thrd(&table->philosophers[i].therad_id,
				dinner_simulation, &table->philosophers[i], CREATE);
			i++;
		}
	}
}

void	join_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->nbr_of_philos)
	{
		thrd(&table->philosophers[i].therad_id, NULL, NULL, JOIN);
		table->philosophers[i].thread_joined = 1;
		i++;
	}
}

void	dinner_start(t_table *table)
{
	table->start_time = get_time(MICROSECOND);
	mtx(&table->table_mutex, LOCK);
	table->ready = 1;
	mtx(&table->table_mutex, UNLOCK);
	start_threads(table);
	thrd(&table->dead_checker, check_death, table, CREATE);
	join_threads(table);
	mtx(&table->table_mutex, LOCK);
	table->stop = 1;
	mtx(&table->table_mutex, UNLOCK);
	thrd(&table->dead_checker, NULL, NULL, JOIN);
	table->death_checker_joined = 1;
}
