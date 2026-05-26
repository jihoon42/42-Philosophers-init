/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_state(t_philo *philo, char *message)
{
	int	allowed;

	pthread_mutex_lock(&philo->table->print_lock);
	allowed = !is_finished(philo->table);
	if (allowed)
		put_log(philo, message);
	pthread_mutex_unlock(&philo->table->print_lock);
	return (allowed);
}

void	print_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->print_lock);
	pthread_mutex_lock(&philo->table->state_lock);
	if (!philo->table->finished)
	{
		philo->table->finished = 1;
		put_log(philo, MSG_DIE);
	}
	pthread_mutex_unlock(&philo->table->state_lock);
	pthread_mutex_unlock(&philo->table->print_lock);
}
