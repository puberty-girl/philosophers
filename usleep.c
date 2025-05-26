/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usleep.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-20 17:27:11 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-20 17:27:11 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_usleep(long usec, t_table *table)
{
	long	start;

	start = get_time(MICROSECOND);
	while (!ready_check(&table->table_mutex, table->stop))
	{
		if (get_time(MICROSECOND) - start >= usec)
			break ;
		usleep(100);
	}
}
