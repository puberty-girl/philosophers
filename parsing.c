/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-20 17:19:46 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-20 17:19:46 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isspace(char c)
{
	if (c == ' ')
		return (1);
	else if (c == '\t')
		return (1);
	else if (c == '\n')
		return (1);
	else if (c == '\v')
		return (1);
	else if (c == '\f')
		return (1);
	else if (c == '\r')
		return (1);
	else
		return (0);
}

long	ft_atol(const char *str)
{
	long	num;

	num = 0;
	while (ft_isspace(*str) || *str == '+')
		str++;
	if (*str == '-')
		{
			printf("numbers cant be negtive\n");
			return(-1);
		}
	while (*str >= '0' && *str <= '9' && *str)
	{
		num = num * 10 + (*str - '0');
		str++;
		if (num > INT_MAX)
		{
			printf("numbers cant be bigger than intmax\n");
			return(-1);
		}
	}
	if (!(*str >= '0' && *str <= '9') && *str != '\0')
	{
		printf("incorrect digits\n");
		return(-1);
	}
	return (num);
}

int	parse_init(t_table *table, char **argv)
{
	table->nbr_of_philos = ft_atol(argv[1]);
	if (table->nbr_of_philos <= 0)
		return (1);
	table->time_to_die = ft_atol(argv[2]) * 1000;
	if (table->time_to_die <= 0)
		return (1);
	table->time_to_eat = ft_atol(argv[3]) * 1000;
	if (table->time_to_eat <= 0)
		return (1);
	table->time_to_sleep = ft_atol(argv[4]) * 1000;
	if (table->time_to_sleep <= 0)
		return (1);
	if (argv[5])
	{
		table->must_eat = ft_atol(argv[5]);
		if (table->must_eat < 0)
			return (1);
	}
	else
		table->must_eat = -1;
	if (table->must_eat == 1)
		return (1);
	if (table->time_to_die < 60000 || table->time_to_eat < 60000
		|| table->time_to_sleep < 60000)
		return (error_print("time values should be longer than 60 ms"));
	return (0);
}
