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
	long	start;

	start = current_time_ms();
	while (current_time_ms() - start < duration)
		usleep(500);
}

void	start_meal_clock(t_philo *philo)
{
	sem_wait(philo->table->data_lock);
	philo->last_meal = current_time_ms();
	sem_post(philo->table->data_lock);
}
