/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_fork_locks(t_table *table)
{
	while (table->forks_ready < table->rules.count)
	{
		if (pthread_mutex_init(&table->forks[table->forks_ready], 0) != 0)
			return (0);
		table->forks_ready++;
	}
	return (1);
}

static int	init_meal_locks(t_table *table)
{
	while (table->meals_ready < table->rules.count)
	{
		if (pthread_mutex_init(&table->philos[table->meals_ready].meal_lock,
				0) != 0)
			return (0);
		table->meals_ready++;
	}
	return (1);
}

static int	init_locks(t_table *table)
{
	if (pthread_mutex_init(&table->print_lock, 0) != 0)
		return (0);
	table->print_ready = 1;
	if (pthread_mutex_init(&table->state_lock, 0) != 0)
		return (0);
	table->state_ready = 1;
	if (!init_fork_locks(table))
		return (0);
	if (!init_meal_locks(table))
		return (0);
	return (1);
}

static void	init_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->rules.count)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals = 0;
		table->philos[i].full = 0;
		table->philos[i].last_meal = 0;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1)
			% table->rules.count];
		table->philos[i].table = table;
		table->fork_available[i] = 1;
		i++;
	}
}

int	init_table(t_table *table, t_rules *rules)
{
	table->rules = *rules;
	table->finished = 0;
	table->full_count = 0;
	table->forks_ready = 0;
	table->meals_ready = 0;
	table->print_ready = 0;
	table->state_ready = 0;
	table->ready_count = 0;
	table->start_time = 0;
	table->fork_available = 0;
	table->forks = malloc(sizeof(pthread_mutex_t) * rules->count);
	table->philos = malloc(sizeof(t_philo) * rules->count);
	table->fork_available = malloc(sizeof(int) * rules->count);
	if (!table->forks || !table->philos || !table->fork_available)
	{
		cleanup_table(table);
		return (0);
	}
	if (!init_locks(table))
	{
		cleanup_table(table);
		return (0);
	}
	init_philos(table);
	return (1);
}
