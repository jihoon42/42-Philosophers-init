/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static void	put_text(char *text)
{
	write(1, text, ft_strlen(text));
}

static void	put_number(long number)
{
	char	buffer[20];
	int		i;

	i = 0;
	if (number == 0)
	{
		write(1, "0", 1);
		return ;
	}
	while (number > 0)
	{
		buffer[i] = (char)(number % 10 + '0');
		number = number / 10;
		i++;
	}
	while (i > 0)
		write(1, &buffer[--i], 1);
}

void	put_log(t_philo *philo, char *message)
{
	put_number(elapsed_ms(philo->table));
	write(1, " ", 1);
	put_number(philo->id);
	write(1, " ", 1);
	put_text(message);
	write(1, "\n", 1);
}

int	write_error(void)
{
	write(2, "Error\n", 6);
	return (1);
}
