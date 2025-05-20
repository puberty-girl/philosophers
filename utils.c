#include "philosophers.h"

void error_print(const char *error)
{
    printf("%s\n", error);
    exit(EXIT_FAILURE);
}

void    *mlc(int len)
{
    void    *res;

    res = malloc(len);
    if (res == NULL)
        error_print("malloc error");
    return (res);
}

void	mutex_checker(int status, t_opcode opcode)
{
	if (status == 0)
		return;
	if (status == EINVAL && (opcode == LOCK || opcode == UNLOCK))
		error_print("invalid mutex value");
	else if (status == EINVAL && opcode == INIT)
		error_print("attr value is invalid");
	else if (status == EDEADLK)
		error_print("A deadlock would occur if the thread blocked waiting for mutex.");
	else if (status == EPERM)
		error_print("the current thread does not hold a lock on mutex");
	else if (status == ENOMEM)
		error_print("the procces cant allocate enough memory to create another mutex");
	else if (status == EBUSY)
		error_print("locked mutex");
}

void	mtx(pthread_mutex_t *mutex, t_opcode opcode)
{
	if (opcode == LOCK)
		mutex_checker(pthread_mutex_lock(mutex), opcode);
	else if (opcode == UNLOCK)
		mutex_checker(pthread_mutex_unlock(mutex), opcode);
	else if (opcode == INIT)
		mutex_checker(pthread_mutex_init(mutex, NULL), opcode);
	else if (opcode == DESTROY)
		mutex_checker(pthread_mutex_destroy(mutex), opcode);
	else
		error_print("wrong upcode");
}

void thread_checker(int status, t_opcode opcode)
{
	if (status == 0)
		return;
	if (status == EAGAIN)
		error_print("No resources to create another thread");
	else if (status == EPERM) 
		error_print( "The caller does not have appropriate permission\n");
	else if (status == EINVAL && opcode == CREATE) 
		error_print("The value specified by attr is invalid.");
	else if (status == EINVAL && (opcode == JOIN || opcode == DETACH))
		error_print("The value specified by thread is not joinable\n");
	else if (status == ESRCH)
		error_print("No thread could be found corresponding to that specified by the given thread ID, thread.");
	else if (status == EDEADLK)
		error_print("A deadlock was detected or the value of thread specifies the calling thread.");
}

void	thrd(pthread_t *thread, void *(*foo)(void *), void *data, t_opcode opcode)
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