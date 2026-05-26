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

#include "philo_bonus.h"

static int	philo_dead(t_philo *philo)
{
	long	last_meal;
	int		dead;

	sem_wait(philo->table->data_lock);
	last_meal = philo->last_meal;
	sem_post(philo->table->data_lock);
	dead = 0;
	if (current_time_ms() - last_meal >= philo->table->rules.time_die)
		dead = 1;
	return (dead);
}

void	*child_monitor(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (philo_dead(philo))
		{
			sem_wait(philo->table->print);
			put_log(philo, MSG_DIE);
			exit(1);
		}
		usleep(1000);
	}
	return (0);
}
