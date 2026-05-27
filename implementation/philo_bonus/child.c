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
	precise_sleep(philo->table->rules.time_die);
	sem_wait(philo->table->print);
	put_log(philo, MSG_DIE);
	exit(EXIT_DEAD);
}

static void	child_loop(t_philo *philo)
{
	if (philo->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (!take_forks(philo))
			wait_for_death();
		if (!philo_eat(philo))
		{
			release_forks(philo);
			if (philo->meal_sent)
				exit(EXIT_FULL);
			wait_for_death();
		}
		release_forks(philo);
		if (!philo_sleep_think(philo))
			wait_for_death();
	}
}

void	run_child(t_philo *philo)
{
	pthread_t	monitor;

	if (!open_child_data_lock(philo))
		exit(EXIT_ERROR);
	if (philo->table->rules.count == 1)
		single_child(philo);
	if (pthread_create(&monitor, 0, child_monitor, philo) != 0)
		exit(EXIT_ERROR);
	pthread_detach(monitor);
	child_loop(philo);
	exit(EXIT_ERROR);
}
