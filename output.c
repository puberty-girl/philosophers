/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-20 17:19:40 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-20 17:19:40 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	print_status(t_status status, t_philosopher *philosopher)
{
	long	elapsed;

	if (status != DIES && (philosopher->isfull
			|| ready_check(&philosopher->table->table_mutex,
				philosopher->table->stop)))
		return ;
	elapsed = get_time(MICROSECOND) - philosopher->table->start_time;
	elapsed /= 1000;
	mtx(&philosopher->table->output_mutex, LOCK);
	if (status == TAKING_FIRST_FORK || status == TAKING_SECOND_FORK)
		printf("%-6ld %d has taken a fork\n", elapsed, philosopher->philo_id);
	else if (status == EATING)
		printf("%-6ld %d is eating\n", elapsed, philosopher->philo_id);
	else if (status == SLEEPING)
		printf("%-6ld %d is sleeping\n", elapsed, philosopher->philo_id);
	else if (status == THINKING)
		printf("%-6ld %d is thinking\n", elapsed, philosopher->philo_id);
	else if (status == DIES)
	{
		printf("%-6ld %d died\n", elapsed, philosopher->philo_id);
		fflush(stdout);
	}
	mtx(&philosopher->table->output_mutex, UNLOCK);
}
