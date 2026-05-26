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

static void	post_meal_tokens(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->rules.count)
	{
		sem_post(table->meals_sem);
		i++;
	}
}

static void	*death_watcher(void *arg)
{
	t_table	*table;
	int		status;

	table = (t_table *)arg;
	if (waitpid(-1, &status, 0) > 0)
	{
		kill_children(table);
		post_meal_tokens(table);
	}
	return (0);
}

int	run_parent(t_table *table)
{
	pthread_t	watcher;
	int			i;

	if (pthread_create(&watcher, 0, death_watcher, table) != 0)
		return (0);
	if (!table->rules.has_limit)
		pthread_join(watcher, 0);
	i = 0;
	while (table->rules.has_limit && i < table->rules.count)
	{
		sem_wait(table->meals_sem);
		i++;
	}
	if (table->rules.has_limit)
	{
		kill_children(table);
		pthread_join(watcher, 0);
	}
	wait_all_children();
	return (1);
}
