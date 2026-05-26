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

static int	all_eaten(t_table *table)
{
	int	i;
	int	done;

	i = 0;
	done = 1;
	while (i < table->rules.count)
	{
		pthread_mutex_lock(&table->philos[i].meal_lock);
		if (table->philos[i].meals < table->rules.must_eat)
			done = 0;
		pthread_mutex_unlock(&table->philos[i].meal_lock);
		if (!done)
			return (0);
		i++;
	}
	return (1);
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
		if (table->rules.has_limit && all_eaten(table))
		{
			set_finished(table);
			return ;
		}
		usleep(1000);
	}
}
