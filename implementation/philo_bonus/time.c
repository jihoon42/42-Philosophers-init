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

#include "philo_bonus.h"

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

void	precise_sleep(long duration)
{
	long	target;
	long	remaining;

	target = current_time_ms() + duration;
	while (1)
	{
		remaining = target - current_time_ms();
		if (remaining <= 0)
			return ;
		usleep(100);
	}
}

int	start_meal_clock(t_philo *philo)
{
	long	last_meal;
	long	now;

	sem_wait(philo->table->data_lock);
	last_meal = philo->last_meal;
	now = current_time_ms();
	if (now - last_meal >= philo->table->rules.time_die)
	{
		sem_post(philo->table->data_lock);
		return (0);
	}
	philo->last_meal = now;
	sem_post(philo->table->data_lock);
	return (1);
}

long	meal_start_time(t_philo *philo)
{
	long	start;

	sem_wait(philo->table->data_lock);
	start = philo->last_meal;
	sem_post(philo->table->data_lock);
	return (start);
}
