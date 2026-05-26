/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	destroy_forks(t_table *table)
{
	while (table->forks_ready > 0)
	{
		table->forks_ready--;
		pthread_mutex_destroy(&table->forks[table->forks_ready]);
	}
}

static void	destroy_meals(t_table *table)
{
	while (table->meals_ready > 0)
	{
		table->meals_ready--;
		pthread_mutex_destroy(&table->philos[table->meals_ready].meal_lock);
	}
}

void	cleanup_table(t_table *table)
{
	destroy_meals(table);
	destroy_forks(table);
	if (table->state_ready)
		pthread_mutex_destroy(&table->state_lock);
	if (table->print_ready)
		pthread_mutex_destroy(&table->print_lock);
	free(table->fork_available);
	free(table->philos);
	free(table->forks);
}
