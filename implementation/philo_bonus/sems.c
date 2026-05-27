/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sems.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	unlink_semaphores(void)
{
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_MEALS);
	sem_unlink(SEM_DATA);
	sem_unlink(SEM_SEATS);
}

static sem_t	*open_named_sem(char *name, int value)
{
	sem_unlink(name);
	return (sem_open(name, O_CREAT, 0644, value));
}

static int	semaphores_ready(t_table *table)
{
	if (table->forks == SEM_FAILED || table->print == SEM_FAILED)
		return (0);
	if (table->meals_sem == SEM_FAILED || table->data_lock == SEM_FAILED)
		return (0);
	if (table->seats == SEM_FAILED)
		return (0);
	return (1);
}

static int	seat_count(t_table *table)
{
	int	seats;

	seats = table->rules.count / 2;
	if (seats < 1)
		seats = 1;
	return (seats);
}

int	open_semaphores(t_table *table)
{
	int	seats;

	seats = seat_count(table);
	unlink_semaphores();
	table->forks = open_named_sem(SEM_FORKS, table->rules.count);
	table->print = open_named_sem(SEM_PRINT, 1);
	table->meals_sem = open_named_sem(SEM_MEALS, 0);
	table->data_lock = open_named_sem(SEM_DATA, 1);
	table->seats = open_named_sem(SEM_SEATS, seats);
	if (!semaphores_ready(table))
		return (0);
	return (1);
}
