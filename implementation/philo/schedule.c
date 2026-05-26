/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schedule.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	long	step;
	int		phase;

	step = elapsed_ms(philo->table) / philo->table->rules.time_eat;
	if (philo->table->rules.count % 2 == 0)
	{
		phase = (int)(step % 2);
		return (even_phase(philo, phase));
	}
	phase = (int)(step % 3);
	return (odd_phase(philo, phase));
}
