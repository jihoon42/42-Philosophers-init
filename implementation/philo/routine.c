/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*single_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_state(philo, MSG_FORK);
	precise_sleep(philo->table, philo->table->rules.time_die + 1);
	pthread_mutex_unlock(philo->left_fork);
	return (0);
}

static void	philo_loop(t_philo *philo)
{
	while (!is_finished(philo->table))
	{
		if (!take_forks(philo))
			return ;
		if (!philo_eat(philo))
		{
			release_forks(philo);
			return ;
		}
		release_forks(philo);
		if (!philo_sleep_think(philo))
			return ;
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->rules.count == 1)
		return (single_routine(philo));
	if (philo->id % 2 == 0)
		usleep(1000);
	philo_loop(philo);
	return (0);
}
