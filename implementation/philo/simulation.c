/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	set_start_times(t_table *table)
{
	int	i;

	i = 0;
	table->start_time = current_time_ms();
	while (i < table->rules.count)
	{
		table->philos[i].last_meal = table->start_time;
		i++;
	}
}

static int	create_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->rules.count)
	{
		if (pthread_create(&table->philos[i].thread, 0, philo_routine,
				&table->philos[i]) != 0)
			return (i);
		i++;
	}
	return (i);
}

static void	join_threads(t_table *table, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(table->philos[i].thread, 0);
		i++;
	}
}

int	start_simulation(t_table *table)
{
	int	created;

	set_start_times(table);
	created = create_threads(table);
	if (created != table->rules.count)
	{
		set_finished(table);
		join_threads(table, created);
		return (0);
	}
	monitor_table(table);
	join_threads(table, created);
	return (1);
}
