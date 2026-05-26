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

void	take_forks(t_philo *philo)
{
	sem_wait(philo->table->seats);
	sem_wait(philo->table->forks);
	print_state(philo, MSG_FORK);
	sem_wait(philo->table->forks);
	print_state(philo, MSG_FORK);
}

void	release_forks(t_philo *philo)
{
	sem_post(philo->table->forks);
	sem_post(philo->table->forks);
	sem_post(philo->table->seats);
}

static void	register_meal(t_philo *philo)
{
	philo->meals++;
	if (philo->table->rules.has_limit
		&& philo->meals >= philo->table->rules.must_eat
		&& !philo->meal_sent)
	{
		philo->meal_sent = 1;
		sem_post(philo->table->meals_sem);
	}
}

void	philo_eat(t_philo *philo)
{
	sem_wait(philo->table->data_lock);
	philo->last_meal = current_time_ms();
	sem_post(philo->table->data_lock);
	print_state(philo, MSG_EAT);
	precise_sleep(philo->table->rules.time_eat);
	sem_wait(philo->table->data_lock);
	register_meal(philo);
	sem_post(philo->table->data_lock);
}

void	philo_sleep_think(t_philo *philo)
{
	print_state(philo, MSG_SLEEP);
	precise_sleep(philo->table->rules.time_sleep);
	print_state(philo, MSG_THINK);
}
