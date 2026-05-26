/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->meal_lock);
	philo->last_meal = current_time_ms();
	pthread_mutex_unlock(&philo->meal_lock);
	if (!print_state(philo, MSG_EAT))
		return (0);
	precise_sleep(philo->table, philo->table->rules.time_eat);
	if (is_finished(philo->table))
		return (0);
	pthread_mutex_lock(&philo->meal_lock);
	philo->meals++;
	pthread_mutex_unlock(&philo->meal_lock);
	return (1);
}

int	philo_sleep_think(t_philo *philo)
{
	if (!print_state(philo, MSG_SLEEP))
		return (0);
	precise_sleep(philo->table, philo->table->rules.time_sleep);
	if (!print_state(philo, MSG_THINK))
		return (0);
	return (1);
}
