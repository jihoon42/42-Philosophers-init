/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schedule.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	read_meal_state(t_philo *philo, int *meals, long *last, int *full)
{
	pthread_mutex_lock(&philo->meal_lock);
	*meals = philo->meals;
	*last = philo->last_meal;
	*full = philo->full;
	pthread_mutex_unlock(&philo->meal_lock);
}

static int	tie_rank(t_philo *philo)
{
	if (philo->table->rules.count % 2 == 0)
	{
		if (philo->id % 2 == 1)
			return (0);
		return (1);
	}
	if (philo->id % 2 == 1 && philo->id != philo->table->rules.count)
		return (0);
	if (philo->id % 2 == 0)
		return (1);
	return (2);
}

static int	neighbor_has_priority(t_philo *philo, int index, int meals,
	long last)
{
	t_philo	*neighbor;
	int		neighbor_meals;
	int		neighbor_full;
	long	neighbor_last;

	neighbor = &philo->table->philos[index];
	read_meal_state(neighbor, &neighbor_meals, &neighbor_last, &neighbor_full);
	if (neighbor_full)
		return (0);
	if (neighbor_meals < meals)
		return (1);
	if (neighbor_meals > meals)
		return (0);
	if (neighbor_last < last)
		return (1);
	if (neighbor_last > last)
		return (0);
	return (tie_rank(neighbor) < tie_rank(philo));
}

int	can_try_eat(t_philo *philo)
{
	int		left;
	int		right;
	int		meals;
	int		full;
	long	last;

	read_meal_state(philo, &meals, &last, &full);
	if (full)
		return (0);
	left = philo->id - 2;
	if (left < 0)
		left = philo->table->rules.count - 1;
	right = philo->id % philo->table->rules.count;
	if (neighbor_has_priority(philo, left, meals, last))
		return (0);
	if (neighbor_has_priority(philo, right, meals, last))
		return (0);
	return (1);
}
