/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philo_dead(t_philo *philo)
{
	long	last_meal;
	int		dead;

	pthread_mutex_lock(&philo->meal_lock);
	last_meal = philo->last_meal;
	pthread_mutex_unlock(&philo->meal_lock);
	dead = 0;
	if (current_time_ms() - last_meal >= philo->table->rules.time_die)
		dead = 1;
	return (dead);
}

void	monitor_table(t_table *table)
{
	int	i;

	while (!is_finished(table))
	{
		i = 0;
		while (i < table->rules.count && !is_finished(table))
		{
			if (philo_dead(&table->philos[i]))
			{
				print_death(&table->philos[i]);
				return ;
			}
			i++;
		}
		usleep(1000);
	}
}
