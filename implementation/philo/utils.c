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

#include "philo.h"

static void	append_text(char *buffer, int *index, char *text)
{
	int	i;

	i = 0;
	while (text[i])
	{
		buffer[*index] = text[i];
		(*index)++;
		i++;
	}
}

static void	append_number(char *buffer, int *index, long number)
{
	char	digits[20];
	int		i;

	i = 0;
	if (number == 0)
		digits[i++] = '0';
	while (number > 0)
	{
		digits[i] = (char)(number % 10 + '0');
		number = number / 10;
		i++;
	}
	while (i > 0)
		buffer[(*index)++] = digits[--i];
}

void	put_log(t_philo *philo, char *message)
{
	char	buffer[64];
	int		index;

	index = 0;
	append_number(buffer, &index, elapsed_ms(philo->table));
	buffer[index++] = ' ';
	append_number(buffer, &index, philo->id);
	buffer[index++] = ' ';
	append_text(buffer, &index, message);
	buffer[index++] = '\n';
	write(1, buffer, index);
}

void	put_meal_log(t_philo *philo)
{
	char	buffer[192];
	char	*messages[3];
	int		index;
	int		i;
	long	time;

	index = 0;
	i = 0;
	time = elapsed_ms(philo->table);
	messages[0] = MSG_FORK;
	messages[1] = MSG_FORK;
	messages[2] = MSG_EAT;
	while (i < 3)
	{
		append_number(buffer, &index, time);
		buffer[index++] = ' ';
		append_number(buffer, &index, philo->id);
		buffer[index++] = ' ';
		append_text(buffer, &index, messages[i]);
		buffer[index++] = '\n';
		i++;
	}
	write(1, buffer, index);
}

int	write_error(void)
{
	write(2, "Error\n", 6);
	return (1);
}
