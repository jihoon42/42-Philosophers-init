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

#include "philo_bonus.h"

static void	close_one_sem(sem_t *sem)
{
	if (sem && sem != SEM_FAILED)
		sem_close(sem);
}

void	close_semaphores(t_table *table)
{
	close_one_sem(table->forks);
	close_one_sem(table->print);
	close_one_sem(table->meals_sem);
	close_one_sem(table->data_lock);
	close_one_sem(table->seats);
}

void	cleanup_table(t_table *table)
{
	close_semaphores(table);
	unlink_semaphores();
	free(table->philos);
}
