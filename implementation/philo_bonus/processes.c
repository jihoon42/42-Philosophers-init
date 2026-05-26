/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

static int	create_processes(t_table *table)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < table->rules.count)
	{
		pid = fork();
		if (pid < 0)
			return (i);
		if (pid == 0)
			run_child(&table->philos[i]);
		table->philos[i].pid = pid;
		i++;
	}
	return (i);
}

int	start_processes(t_table *table)
{
	int	created;

	set_start_times(table);
	created = create_processes(table);
	if (created != table->rules.count)
	{
		kill_children(table);
		wait_all_children();
		return (0);
	}
	return (run_parent(table));
}
