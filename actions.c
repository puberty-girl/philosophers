/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-20 17:18:53 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-20 17:18:53 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	eat(t_philosopher *philosopher)
{
	mtx(&philosopher->first_fork->fork, LOCK);
	print_status(TAKING_FIRST_FORK, philosopher);
	mtx(&philosopher->second_fork->fork, LOCK);
	print_status(TAKING_SECOND_FORK, philosopher);
	mtx(&philosopher->philo_mutex, LOCK);
	philosopher->last_meal_time = get_time(MICROSECOND);
	mtx(&philosopher->philo_mutex, UNLOCK);
	print_status(EATING, philosopher);
	ft_usleep(philosopher->table->time_to_eat, philosopher->table);
	mtx(&philosopher->philo_mutex, LOCK);
	philosopher->meals_consumed++;
	if (philosopher->table->must_eat > 0 &&
		philosopher->meals_consumed == philosopher->table->must_eat)
		philosopher->isfull = 1;
	mtx(&philosopher->philo_mutex, UNLOCK);
	mtx(&philosopher->first_fork->fork, UNLOCK);
	mtx(&philosopher->second_fork->fork, UNLOCK);
}


void	think(t_philosopher *philosopher, int flag)
{
	long	time_sleep;
	long	time_eat;
	long	time_think;

	if (!flag)
		print_status(THINKING, philosopher);
	if (philosopher->table->nbr_of_philos % 2 == 0)
		return ;
	time_eat = philosopher->table->time_to_eat;
	time_sleep = philosopher->table->time_to_sleep;
	time_think = time_eat * 2 - time_sleep;
	if (time_think < 0)
		time_think = 0;
	ft_usleep(time_think * 0.5, philosopher->table);
}

void	*only_philo(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	sync_threads(philo->table);
	increase_long(&philo->table->table_mutex, &philo->table->nbr_of_threads);
	mtx(&philo->first_fork->fork, LOCK);
	print_status(TAKING_FIRST_FORK, philo);
	mtx(&philo->philo_mutex, LOCK);
	philo->last_meal_time = get_time(MICROSECOND);
	mtx(&philo->philo_mutex, UNLOCK);
	ft_usleep(philo->table->time_to_die + 1, philo->table);
	print_status(DIES, philo);
	mtx(&philo->first_fork->fork, UNLOCK);
	return (NULL);
}
