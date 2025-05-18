#include "philosophers.h"


void	print_status_debug(t_status status, t_philosopher *philosopher, long elapsed)
{
	if (status == TAKING_FIRST_FORK && !ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
		printf("%-6ld %d has taken the 1 fork\n", elapsed, philosopher->philo_id);
	else if (status == TAKING_SECOND_FORK && !ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
		printf("%-6ld %d has taken the 2 fork\n", elapsed, philosopher->philo_id);
	else if (status == EATING && !ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
		printf("%-6ld %d is eating\n", elapsed, philosopher->philo_id);
	else if (status == SLEEPING && !ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
		printf("%-6ld %d is sleeping\n", elapsed, philosopher->philo_id);
	else if (status == THINKING && !ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
		printf("%-6ld %d is thinking\n", elapsed, philosopher->philo_id);	
}

void    print_status(t_status status, t_philosopher *philosopher, int debug)
{
	long	elapsed;

	elapsed = get_time(MILLISECOND) - philosopher->table->start_time;
	if (philosopher->isfull)
		return;
	mtx(&philosopher->table->output_mutex, LOCK);

	if (debug)
		print_status_debug(status, philosopher, elapsed);
	else
	{
		if ((status == TAKING_FIRST_FORK || status == TAKING_SECOND_FORK) 
			&& !ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
			printf("%-6ld %d has taken a fork\n", elapsed, philosopher->philo_id);
		else if (status == EATING && !ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
			printf("%-6ld %d is eating\n", elapsed, philosopher->philo_id);
		else if (status == SLEEPING && !ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
			printf("%-6ld %d is sleeping\n", elapsed, philosopher->philo_id);
		else if (status == THINKING && !ready_check(&philosopher->table->table_mutex, philosopher->table->stop))
			printf("%-6ld %d is thinking\n", elapsed, philosopher->philo_id);
		else if (status == DIES)
			printf("%-6ld %d died\n", elapsed, philosopher->philo_id);
	}
	mtx(&philosopher->table->output_mutex, UNLOCK);
}
