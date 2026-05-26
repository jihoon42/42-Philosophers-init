/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_finished(t_table *table)
{
	int	value;

	pthread_mutex_lock(&table->state_lock);
	value = table->finished;
	pthread_mutex_unlock(&table->state_lock);
	return (value);
}

void	set_finished(t_table *table)
{
	pthread_mutex_lock(&table->state_lock);
	table->finished = 1;
	pthread_mutex_unlock(&table->state_lock);
}
