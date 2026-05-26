/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	current_time_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, 0) != 0)
		return (0);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

long	elapsed_ms(t_table *table)
{
	return (current_time_ms() - table->start_time);
}

void	precise_sleep(t_table *table, long duration)
{
	long	start;

	start = current_time_ms();
	while (!is_finished(table) && current_time_ms() - start < duration)
		usleep(500);
}
