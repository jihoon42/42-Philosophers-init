/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   meals.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_alive(t_philo *philo)
{
	long	last_meal;
	int		alive;

	pthread_mutex_lock(&philo->meal_lock);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->meal_lock);
	alive = 1;
	if (current_time_ms() - last_meal >= philo->table->rules.time_die)
		alive = 0;
	return (alive);
}

int	start_meal_clock(t_philo *philo)
{
	long	last_meal;
	long	now;

	pthread_mutex_lock(&philo->meal_lock);
	last_meal = philo->last_meal;
	now = current_time_ms();
	if (now - last_meal >= philo->table->rules.time_die)
	{
		pthread_mutex_unlock(&philo->meal_lock);
		return (0);
	}
	philo->last_meal = now;
	pthread_mutex_unlock(&philo->meal_lock);
	return (1);
}

long	meal_start_time(t_philo *philo)
{
	long	start;

	pthread_mutex_lock(&philo->meal_lock);
	start = philo->last_meal;
	pthread_mutex_unlock(&philo->meal_lock);
	return (start);
}

static void	mark_full(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->state_lock);
	philo->table->full_count++;
	if (philo->table->full_count >= philo->table->rules.count)
		philo->table->finished = 1;
	pthread_mutex_unlock(&philo->table->state_lock);
}

int	register_meal(t_philo *philo)
{
	int	keep_running;

	pthread_mutex_lock(&philo->meal_lock);
	philo->meals++;
	if (philo->table->rules.has_limit
		&& philo->meals >= philo->table->rules.must_eat
		&& !philo->full)
	{
		philo->full = 1;
		mark_full(philo);
	}
	keep_running = !philo->full;
	pthread_mutex_unlock(&philo->meal_lock);
	if (!philo->table->rules.has_limit)
		return (1);
	return (keep_running);
}
