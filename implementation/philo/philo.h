/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_DIE "died"

typedef struct s_rules
{
	int		count;
	long	time_die;
	long	time_eat;
	long	time_sleep;
	int		must_eat;
	int		has_limit;
}	t_rules;

typedef struct s_table	t_table;

typedef struct s_philo
{
	int				id;
	int				meals;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_table			*table;
}	t_philo;

struct s_table
{
	t_rules			rules;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	int				*fork_available;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	state_lock;
	int				finished;
	int				forks_ready;
	int				meals_ready;
	int				print_ready;
	int				state_ready;
	long			start_time;
};

int		parse_args(int argc, char **argv, t_rules *rules);
int		init_table(t_table *table, t_rules *rules);
int		start_simulation(t_table *table);
int		is_finished(t_table *table);
int		write_error(void);
long	current_time_ms(void);
long	elapsed_ms(t_table *table);
void	cleanup_table(t_table *table);
void	set_finished(t_table *table);
void	precise_sleep(t_table *table, long duration);
void	*philo_routine(void *arg);
void	monitor_table(t_table *table);
void	put_log(t_philo *philo, char *message);
void	print_death(t_philo *philo);
int		print_state(t_philo *philo, char *message);
int		take_forks(t_philo *philo);
int		can_try_eat(t_philo *philo);
void	release_forks(t_philo *philo);
int		philo_eat(t_philo *philo);
int		philo_sleep_think(t_philo *philo);

#endif
