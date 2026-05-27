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

#include "philo_bonus.h"

static int	wait_turn(t_philo *philo)
{
	while (!can_try_eat(philo))
	{
		if (!philo_alive(philo))
			return (0);
		usleep(100);
	}
	return (1);
}

int	take_forks(t_philo *philo)
{
	if (!wait_turn(philo))
		return (0);
	sem_wait(philo->table->seats);
	sem_wait(philo->table->forks);
	sem_wait(philo->table->forks);
	if (!start_meal_clock(philo))
	{
		release_forks(philo);
		return (0);
	}
	if (!print_state(philo, MSG_FORK) || !print_state(philo, MSG_FORK))
	{
		release_forks(philo);
		return (0);
	}
	return (1);
}

void	release_forks(t_philo *philo)
{
	sem_post(philo->table->forks);
	sem_post(philo->table->forks);
	sem_post(philo->table->seats);
}

static int	register_meal(t_philo *philo)
{
	philo->meals++;
	if (philo->table->rules.has_limit
		&& philo->meals >= philo->table->rules.must_eat
		&& !philo->meal_sent)
	{
		philo->meal_sent = 1;
		return (0);
	}
	return (1);
}

int	philo_eat(t_philo *philo)
{
	int		keep_running;
	long	meal_start;

	meal_start = meal_start_time(philo);
	if (!print_state(philo, MSG_EAT))
		return (0);
	relaxed_sleep(philo->table, meal_start + philo->table->rules.time_eat
		- current_time_ms());
	if (!philo_alive(philo))
		return (0);
	sem_wait(philo->table->data_lock);
	keep_running = register_meal(philo);
	sem_post(philo->table->data_lock);
	return (keep_running);
}
