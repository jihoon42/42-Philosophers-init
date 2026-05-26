/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int argc, char **argv)
{
	t_rules	rules;
	t_table	table;
	int		status;

	if (!parse_args(argc, argv, &rules))
		return (write_error());
	if (rules.has_limit && rules.must_eat == 0)
		return (0);
	if (!init_table(&table, &rules))
		return (write_error());
	status = start_processes(&table);
	cleanup_table(&table);
	if (!status)
		return (write_error());
	return (0);
}
