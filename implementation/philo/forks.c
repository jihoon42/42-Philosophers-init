/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	lock_fork_pair(t_table *table, int left, int right)
{
	if (left < right)
	{
		pthread_mutex_lock(&table->forks[left]);
		pthread_mutex_lock(&table->forks[right]);
	}
	else
	{
		pthread_mutex_lock(&table->forks[right]);
		pthread_mutex_lock(&table->forks[left]);
	}
}

static void	unlock_fork_pair(t_table *table, int left, int right)
{
	pthread_mutex_unlock(&table->forks[left]);
	pthread_mutex_unlock(&table->forks[right]);
}

static int	reserve_forks(t_philo *philo, int left, int right)
{
	while (!is_finished(philo->table))
	{
		if (can_try_eat(philo))
		{
			lock_fork_pair(philo->table, left, right);
			if (philo->table->fork_available[left]
				&& philo->table->fork_available[right])
			{
				philo->table->fork_available[left] = 0;
				philo->table->fork_available[right] = 0;
				unlock_fork_pair(philo->table, left, right);
				return (1);
			}
			unlock_fork_pair(philo->table, left, right);
		}
		usleep(500);
	}
	return (0);
}

int	take_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id - 1;
	right = philo->id % philo->table->rules.count;
	if (!reserve_forks(philo, left, right))
		return (0);
	if (!print_state(philo, MSG_FORK))
	{
		release_forks(philo);
		return (0);
	}
	if (!print_state(philo, MSG_FORK))
	{
		release_forks(philo);
		return (0);
	}
	return (1);
}

void	release_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id - 1;
	right = philo->id % philo->table->rules.count;
	lock_fork_pair(philo->table, left, right);
	philo->table->fork_available[left] = 1;
	philo->table->fork_available[right] = 1;
	unlock_fork_pair(philo->table, left, right);
}
