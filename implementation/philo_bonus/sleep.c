/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleep.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

void	relaxed_sleep(t_table *table, long duration)
{
	long	target;
	long	remaining;
	long	margin;

	if (table->rules.count <= 1
		|| table->rules.time_die <= minimum_cycle(table))
	{
		precise_sleep(duration);
		return ;
	}
	target = current_time_ms() + duration;
	margin = duration / 2;
	if (margin > 100)
		margin = 100;
	while (1)
	{
		remaining = target - current_time_ms();
		if (remaining <= margin)
			return ;
		usleep(100);
	}
}

int	philo_sleep_think(t_philo *philo)
{
	if (!print_state(philo, MSG_SLEEP))
		return (0);
	relaxed_sleep(philo->table, philo->table->rules.time_sleep);
	if (!print_state(philo, MSG_THINK))
		return (0);
	return (1);
}
