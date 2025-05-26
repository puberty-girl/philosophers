/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-20 17:26:54 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-20 17:26:54 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>
#include <limits.h>
#include <errno.h>

//////////////	enums		//////////////

typedef enum e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH
}	t_opcode;

typedef enum e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND
}	t_time_code;

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKING_FIRST_FORK,
	TAKING_SECOND_FORK,
	DIES
}	t_status;

//////////////	structs		//////////////

// typedef struct s_table	t_table;

typedef struct s_fork
{
	pthread_mutex_t	fork;
	int				fork_id;
}	t_fork;

typedef struct s_philosopher
{
	int				philo_id;
	int				isfull;
	long			meals_consumed;
	long			last_meal_time;
	t_fork			*first_fork;
	t_fork			*second_fork;
	pthread_t		therad_id;
	pthread_mutex_t	philo_mutex;
	struct s_table	*table;
	int				thread_joined;
}	t_philosopher;

typedef struct s_table
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
	int				death_checker_joined;
	t_fork			*forks;
	t_philosopher	*philosophers;
	pthread_t		dead_checker;
	pthread_mutex_t	table_mutex;
	pthread_mutex_t	output_mutex;
}	t_table;

////////////////	functions	//////

int					error_print(const char *error);
int					parse_init(t_table *table, char **argv);
int					data_init(t_table *table);
void				philo_init(t_table *table);

void				*mlc(int len);
int					mtx(pthread_mutex_t *mutex, t_opcode opcode);
void				mutex_checker(int status, t_opcode opcode);
int					thrd(pthread_t *thread,
						void *(*foo)(void *), void *data, t_opcode opcode);
void				sync_threads(t_table *table);
long				get_time(t_time_code time_code);
int					ready_check(pthread_mutex_t *mutex, int *value);
void				ft_usleep(long usec, t_table *table);
void				print_status(t_status status,
						t_philosopher *philosopher);
void				eat(t_philosopher *philosopher);
void				think(t_philosopher *philosopher, int flag);
void				dinner_start(t_table *table);
void				*check_death(void *data);
void				increase_long(pthread_mutex_t *mutex, long *value);
int					all_threads_are_running(pthread_mutex_t *mutex,
						long *threads, long nbr_philos);
void				*only_philo(void *arg);
void				clean(t_table *table);
void				thread_checker(int status, t_opcode opcode);
void				*dinner_simulation(void *data);
void				philosopher_loop(t_philosopher *philosopher);
void				prepare_philosopher(t_philosopher *philosopher);
long				get_long(pthread_mutex_t *mutex, long *value);
void				init_forks_destroy(t_table *t, int i);
