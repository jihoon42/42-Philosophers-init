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

static void	release_start(t_table *table)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&table->state_lock);
	table->start_time = current_time_ms();
	while (i < table->rules.count)
	{
		pthread_mutex_lock(&table->philos[i].meal_lock);
		table->philos[i].last_meal = table->start_time;
		pthread_mutex_unlock(&table->philos[i].meal_lock);
		i++;
	}
	pthread_mutex_unlock(&table->state_lock);
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

static void	wait_workers_ready(t_table *table)
{
	int	ready;

	ready = 0;
	while (!ready)
	{
		pthread_mutex_lock(&table->state_lock);
		ready = (table->ready_count >= table->rules.count);
		pthread_mutex_unlock(&table->state_lock);
		if (!ready)
			usleep(100);
	}
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

	created = create_threads(table);
	if (created != table->rules.count)
	{
		set_finished(table);
		join_threads(table, created);
		return (0);
	}
	wait_workers_ready(table);
	release_start(table);
	monitor_table(table);
	join_threads(table, created);
	return (1);
}
