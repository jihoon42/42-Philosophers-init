/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_sems.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	append_text(char *name, int *index, char *text)
{
	int	i;

	i = 0;
	while (text[i])
		name[(*index)++] = text[i++];
}

static void	append_id(char *name, int *index, int id)
{
	char	digits[10];
	int		i;

	i = 0;
	if (id == 0)
		digits[i++] = '0';
	while (id > 0)
	{
		digits[i] = (char)(id % 10 + '0');
		id = id / 10;
		i++;
	}
	while (i > 0)
		name[(*index)++] = digits[--i];
	name[*index] = '\0';
}

static void	build_child_name(t_philo *philo, char *name)
{
	int	index;

	index = 0;
	append_text(name, &index, SEM_CHILD_DATA);
	append_id(name, &index, philo->id);
}

int	open_child_data_lock(t_philo *philo)
{
	char	name[32];

	build_child_name(philo, name);
	sem_close(philo->table->data_lock);
	sem_unlink(name);
	philo->table->data_lock = sem_open(name, O_CREAT, 0644, 1);
	sem_unlink(name);
	if (philo->table->data_lock == SEM_FAILED)
		return (0);
	return (1);
}
