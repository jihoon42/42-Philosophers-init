/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	kill_children(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->rules.count)
	{
		if (table->philos[i].pid > 0)
			kill(table->philos[i].pid, SIGTERM);
		i++;
	}
}

void	wait_all_children(void)
{
	int	status;

	while (waitpid(-1, &status, 0) > 0)
	{
	}
}

static int	handle_child_status(t_table *table, int status, int *full)
{
	if (!WIFEXITED(status))
	{
		kill_children(table);
		wait_all_children();
		return (0);
	}
	if (WEXITSTATUS(status) == EXIT_FULL)
		(*full)++;
	else if (WEXITSTATUS(status) == EXIT_DEAD)
	{
		kill_children(table);
		wait_all_children();
		return (2);
	}
	else
	{
		kill_children(table);
		wait_all_children();
		return (0);
	}
	return (1);
}

int	run_parent(t_table *table)
{
	int	status;
	int	done;
	int	full;
	int	result;

	done = 0;
	full = 0;
	while (done < table->rules.count)
	{
		if (waitpid(-1, &status, 0) <= 0)
			return (0);
		result = handle_child_status(table, status, &full);
		if (result == 0)
			return (0);
		if (result == 2)
			return (1);
		done++;
	}
	if (table->rules.has_limit && full == table->rules.count)
		return (1);
	return (0);
}
