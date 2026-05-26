/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	parse_number(char *str, long *number)
{
	int		i;
	long	value;

	i = 0;
	value = 0;
	if (!str[0])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		if (value > (2147483647 - (str[i] - '0')) / 10)
			return (0);
		value = value * 10 + (str[i] - '0');
		i++;
	}
	*number = value;
	return (1);
}

static int	fill_required(char **argv, t_rules *rules)
{
	long	value;

	if (!parse_number(argv[1], &value) || value == 0)
		return (0);
	rules->count = (int)value;
	if (!parse_number(argv[2], &value) || value == 0)
		return (0);
	rules->time_die = value;
	if (!parse_number(argv[3], &value) || value == 0)
		return (0);
	rules->time_eat = value;
	if (!parse_number(argv[4], &value) || value == 0)
		return (0);
	rules->time_sleep = value;
	return (1);
}

int	parse_args(int argc, char **argv, t_rules *rules)
{
	long	value;

	if (argc != 5 && argc != 6)
		return (0);
	if (!fill_required(argv, rules))
		return (0);
	rules->has_limit = 0;
	rules->must_eat = 0;
	if (argc == 6)
	{
		if (!parse_number(argv[5], &value))
			return (0);
		rules->has_limit = 1;
		rules->must_eat = (int)value;
	}
	return (1);
}
