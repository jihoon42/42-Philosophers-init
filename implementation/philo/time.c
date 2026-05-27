/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	current_time_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, 0) != 0)
		return (0);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

long	elapsed_ms(t_table *table)
{
	return (current_time_ms() - table->start_time);
}

static long	minimum_cycle(t_table *table)
{
	long	cycle;
	long	rest;

	if (table->rules.count % 2 == 0)
		cycle = table->rules.time_eat * 2;
	else
		cycle = table->rules.time_eat * 3;
	rest = table->rules.time_eat + table->rules.time_sleep;
	if (rest > cycle)
		return (rest);
	return (cycle);
}

void	precise_sleep(t_table *table, long duration)
{
	long	target;
	long	remaining;

	target = current_time_ms() + duration;
	while (!is_finished(table))
	{
		remaining = target - current_time_ms();
		if (remaining <= 0)
			return ;
		if (remaining > 1)
			usleep(100);
	}
}

void	relaxed_sleep(t_table *table, long duration)
{
	long	target;
	long	remaining;
	long	margin;

	if (table->rules.count <= 1
		|| table->rules.time_die <= minimum_cycle(table))
	{
		precise_sleep(table, duration);
		return ;
	}
	target = current_time_ms() + duration;
	margin = duration / 2;
	if (margin > 100)
		margin = 100;
	while (!is_finished(table))
	{
		remaining = target - current_time_ms();
		if (remaining <= margin)
			return ;
		usleep(100);
	}
}
