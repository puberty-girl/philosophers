/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dvasilen <dvasilen@student.42.fr>          #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025-05-20 17:19:33 by dvasilen          #+#    #+#             */
/*   Updated: 2025-05-20 17:19:33 by dvasilen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc == 5 || argc == 6)
	{
		if (parse_init(&table, argv) != 0)
			return (0);
		if (data_init(&table) != 0);
		{
			clean(&table);
			return (1);
		}
		dinner_start(&table);
		clean(&table);
	}
	else
	{
		printf("valid input format is: number_of_philosophers"
			" time_to_die time_to_eat time_to_sleep"
			" [number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	return(0);
}
