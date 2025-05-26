/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-26 16:13:00 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-26 16:13:00 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	join_philos(t_table *table, int i)
{
	if (i >= table->nbr_of_philos)
		return ;
	if (table->philosophers[i].therad_id
		&& !table->philosophers[i].thread_joined)
	{
		thrd(&table->philosophers[i].therad_id, NULL, NULL, JOIN);
		table->philosophers[i].thread_joined = 1;
	}
	join_philos(table, i + 1);
}

void	clean(t_table *table)
{
	if (!table)
		return ;
	if (mtx(&table->table_mutex, LOCK) == 0)
	{
		table->stop = 1;
		mtx(&table->table_mutex, UNLOCK);
	}
	if (table->philosophers)
		join_philos(table, 0);
	if (table->dead_checker && !table->death_checker_joined)
	{
		thrd(&table->dead_checker, NULL, NULL, JOIN);
		table->death_checker_joined = 1;
	}
	free(table->philosophers);
	free(table->forks);
}
