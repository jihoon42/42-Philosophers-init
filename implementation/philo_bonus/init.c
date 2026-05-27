/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	set_empty_table(t_table *table)
{
	table->philos = 0;
	table->forks = 0;
	table->print = 0;
	table->meals_sem = 0;
	table->data_lock = 0;
	table->seats = 0;
	table->start_time = 0;
}

static void	init_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->rules.count)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals = 0;
		table->philos[i].meal_sent = 0;
		table->philos[i].last_meal = 0;
		table->philos[i].pid = 0;
		table->philos[i].table = table;
		i++;
	}
}

int	init_table(t_table *table, t_rules *rules)
{
	table->rules = *rules;
	set_empty_table(table);
	table->philos = malloc(sizeof(t_philo) * rules->count);
	if (!table->philos)
		return (0);
	init_philos(table);
	if (!open_semaphores(table))
	{
		cleanup_table(table);
		return (0);
	}
	return (1);
}
