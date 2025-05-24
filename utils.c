/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-20 17:27:16 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-20 17:27:16 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	error_print(const char *error)
{
	printf("%s\n", error);
	return (1);
}

void	*mlc(int len)
{
	void	*res;

	res = malloc(len);
	if (res == NULL)
	{
		printf("malloc error");
		return (NULL);
	}
	return (res);
}

void	mutex_checker(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	if (status == EINVAL && (opcode == LOCK || opcode == UNLOCK))
		return ;
	else if (status == EINVAL && opcode == INIT)
		error_print("attr value is invalid");
	else if (status == EDEADLK)
		error_print("A deadlock would occur if the thread"
			" blocked waiting for mutex.");
	else if (status == EPERM)
		error_print("the current thread does not hold a lock on mutex");
	else if (status == ENOMEM)
		error_print("the procces cant allocate enough memory"
			"to create another mutex");
	else if (status == EBUSY)
		error_print("locked mutex");
}

int	mtx(pthread_mutex_t *mutex, t_opcode opcode)
{
	int	status;

	if (opcode == LOCK)
		status = pthread_mutex_lock(mutex);
	else if (opcode == UNLOCK)
		status = pthread_mutex_unlock(mutex);
	else if (opcode == INIT)
		status = pthread_mutex_init(mutex, NULL);
	else if (opcode == DESTROY)
		status = pthread_mutex_destroy(mutex);
	else
	{
		error_print("wrong upcode");
		return (1);
	}
	mutex_checker(status, opcode);
	return (status);
}

void	thread_checker(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	if (status == EAGAIN)
		error_print("No resources to create another thread");
	else if (status == EPERM)
		error_print("The caller does not have appropriate permission\n");
	else if (status == EINVAL && opcode == CREATE)
		error_print("The value specified by attr is invalid.");
	else if (status == EINVAL && (opcode == JOIN || opcode == DETACH))
		error_print("The value specified by thread is not joinable\n");
	else if (status == ESRCH)
		error_print("No thread could be found corresponding"
			"to that specified by the given thread ID, thread.");
	else if (status == EDEADLK)
		error_print("A deadlock was detected or the value of"
			"thread specifies the calling thread.");
}
