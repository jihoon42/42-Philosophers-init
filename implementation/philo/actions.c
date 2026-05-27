/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_eat(t_philo *philo)
{
	long	meal_start;

	meal_start = meal_start_time(philo);
	relaxed_sleep(philo->table, meal_start + philo->table->rules.time_eat
		- current_time_ms());
	if (is_finished(philo->table))
		return (0);
	return (register_meal(philo));
}

int	philo_sleep_think(t_philo *philo)
{
	long	meal_start;
	long	time_left;

	meal_start = meal_start_time(philo);
	if (!print_state(philo, MSG_SLEEP))
		return (0);
	relaxed_sleep(philo->table, meal_start + philo->table->rules.time_eat
		+ philo->table->rules.time_sleep - current_time_ms());
	time_left = meal_start + philo->table->rules.time_die - current_time_ms();
	if (time_left > philo->table->rules.time_eat
		+ philo->table->rules.time_sleep + 10
		&& !print_state(philo, MSG_THINK))
		return (0);
	return (1);
}
