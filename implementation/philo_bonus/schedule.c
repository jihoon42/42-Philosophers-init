/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schedule.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/27 00:00:00 by jkim2            ###   ########.fr       */
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

static int	even_phase(t_philo *philo, int phase)
{
	if (phase == 0 && philo->id % 2 == 1)
		return (1);
	if (phase == 1 && philo->id % 2 == 0)
		return (1);
	return (0);
}

static int	odd_phase(t_philo *philo, int phase)
{
	if (phase == 0 && philo->id % 2 == 1
		&& philo->id != philo->table->rules.count)
		return (1);
	if (phase == 1 && philo->id % 2 == 0)
		return (1);
	if (phase == 2 && philo->id == philo->table->rules.count)
		return (1);
	return (0);
}

int	can_try_eat(t_philo *philo)
{
	int	phase;

	if (philo->table->rules.time_die > minimum_cycle(philo->table))
		return (1);
	phase = (int)((elapsed_ms(philo->table) / philo->table->rules.time_eat)
			% 3);
	if (philo->table->rules.count % 2 == 0)
		return (even_phase(philo, phase % 2));
	return (odd_phase(philo, phase));
}
