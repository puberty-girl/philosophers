#include "philosophers.h"

void    eat(t_philosopher *philosopher)
{
    mtx(&philosopher->first_fork->fork, LOCK);
    print_status(TAKING_FIRST_FORK, philosopher, 0);
    mtx(&philosopher->second_fork->fork, LOCK);
    print_status(TAKING_SECOND_FORK, philosopher, 0);

	mtx(&philosopher->philo_mutex, LOCK);
    philosopher->last_meal_time = get_time(MILLISECOND);
    mtx(&philosopher->philo_mutex, UNLOCK);
    philosopher->meals_consumed++;
    print_status(EATING, philosopher, 0);
    ft_usleep(philosopher->table->time_to_eat, philosopher->table);
    if (philosopher->table->must_eat > 0
        && philosopher->meals_consumed == philosopher->table->must_eat)
        philosopher->isfull = 1;
    mtx(&philosopher->first_fork->fork, UNLOCK);
    mtx(&philosopher->second_fork->fork, UNLOCK);
}

void    think(t_philosopher *philosopher)
{
    print_status(THINKING, philosopher, 0);
}

void	*only_philo(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	sync_threads(philo->table);
	mtx(&philo->philo_mutex, LOCK);
	philo->last_meal_time = get_time(MILLISECOND);
	mtx(&philo->philo_mutex, UNLOCK);
	increase_long(&philo->table->table_mutex, &philo->table->nbr_of_threads);
	print_status(TAKING_FIRST_FORK, philo, 0);
	while(!ready_check(&philo->philo_mutex, philo->table->stop)) //maybe whitch to philo->table->mutex
		usleep(200);
	return (NULL);

}