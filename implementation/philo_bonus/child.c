/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	single_child(t_philo *philo)
{
	sem_wait(philo->table->forks);
	print_state(philo, MSG_FORK);
	precise_sleep(philo->table->rules.time_die + 1);
	sem_post(philo->table->forks);
	exit(0);
}

static void	child_loop(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		take_forks(philo);
		philo_eat(philo);
		release_forks(philo);
		philo_sleep_think(philo);
	}
}

void	run_child(t_philo *philo)
{
	pthread_t	monitor;

	if (pthread_create(&monitor, 0, child_monitor, philo) != 0)
		exit(1);
	pthread_detach(monitor);
	if (philo->table->rules.count == 1)
		single_child(philo);
	child_loop(philo);
	exit(0);
}
