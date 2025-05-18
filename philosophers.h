#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include <limits.h>
#include <errno.h>

//////////////	enums		//////////////

typedef enum	e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH
}	t_opcode;

typedef enum	e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND
}	t_time_code;

typedef enum	e_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKING_FIRST_FORK,
	TAKING_SECOND_FORK,
	DIES
}	t_status;

//////////////	structs		//////////////

typedef struct s_table t_table;

typedef struct	s_fork
{
	pthread_mutex_t	fork;
	int				fork_id;
}	t_fork;

typedef struct	s_philosopher
{
	int				philo_id;
	int				isfull;
	long			meals_consumed;
	long			last_meal_time;
	t_fork			*first_fork;
	t_fork			*second_fork;
	pthread_t		therad_id;
	pthread_mutex_t	philo_mutex;
	t_table			*table;
}	t_philosopher;

typedef struct	s_table
{
	long			nbr_of_philos;
	long			time_to_die;
	long			time_to_sleep;
	long			time_to_eat;
	long			must_eat;
	long			start_time;
	long			nbr_of_threads;
	int				stop; // 1 or 0
	int				ready; // 1 or 0
	t_fork			*forks;
	t_philosopher	*philosophers;
	pthread_t		dead_checker;
	pthread_mutex_t	table_mutex;
	pthread_mutex_t	output_mutex;
}	t_table;

////////////////	functions	//////

void	error_print(const char *error);
void	parse_init(t_table *table, char **argv);
void    data_init(t_table *table);
void	philo_init(t_table *table);

void	*mlc(int len);
void	mtx(pthread_mutex_t *mutex, t_opcode opcode);
void	mutex_checker(int status, t_opcode opcode);
void	thrd(pthread_t *thread, void *(*foo)(void *), void *data, t_opcode opcode);
void	sync_threads(t_table *table);
long	get_time(t_time_code time_code);
int		ready_check(pthread_mutex_t *mutex, int value);
void   	ft_usleep(long usec, t_table *table);
void    print_status(t_status status, t_philosopher *philosopher, int debug);
void    eat(t_philosopher *philosopher);
void    think(t_philosopher *philosopher);
void	dinner_start(t_table *table);
void    *check_death(void *data);
void	increase_long(pthread_mutex_t *mutex, long *value);
int		all_threads_are_running(pthread_mutex_t *mutex, long *threads, long nbr_philos);
void	*only_philo(void *arg);
void	clean(t_table *table);