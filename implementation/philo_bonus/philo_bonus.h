/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkim2 <jkim2@student.42seoul.kr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 00:00:00 by jkim2             #+#    #+#             */
/*   Updated: 2026/05/26 00:00:00 by jkim2            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <fcntl.h>
# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

# define MSG_FORK "has taken a fork"
# define MSG_EAT "is eating"
# define MSG_SLEEP "is sleeping"
# define MSG_THINK "is thinking"
# define MSG_DIE "died"
# define SEM_FORKS "/philo_jkim2_forks"
# define SEM_PRINT "/philo_jkim2_print"
# define SEM_MEALS "/philo_jkim2_meals"
# define SEM_DATA "/philo_jkim2_data"
# define SEM_CHILD_DATA "/philo_jkim2_d_"
# define SEM_SEATS "/philo_jkim2_seats"
# define SEM_PICK "/philo_jkim2_pick"
# define EXIT_FULL 0
# define EXIT_DEAD 1
# define EXIT_ERROR 2

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
	int		id;
	int		meals;
	int		meal_sent;
	long	last_meal;
	pid_t	pid;
	t_table	*table;
}	t_philo;

struct s_table
{
	t_rules	rules;
	t_philo	*philos;
	sem_t	*forks;
	sem_t	*print;
	sem_t	*meals_sem;
	sem_t	*data_lock;
	sem_t	*seats;
	sem_t	*pick;
	long	start_time;
};

int		parse_args(int argc, char **argv, t_rules *rules);
int		init_table(t_table *table, t_rules *rules);
int		open_semaphores(t_table *table);
int		start_processes(t_table *table);
int		run_parent(t_table *table);
int		open_child_data_lock(t_philo *philo);
int		write_error(void);
long	current_time_ms(void);
long	elapsed_ms(t_table *table);
void	start_meal_clock(t_philo *philo);
void	cleanup_table(t_table *table);
void	close_semaphores(t_table *table);
void	unlink_semaphores(void);
void	precise_sleep(long duration);
void	run_child(t_philo *philo);
void	*child_monitor(void *arg);
void	put_log(t_philo *philo, char *message);
int		print_state(t_philo *philo, char *message);
int		philo_alive(t_philo *philo);
void	kill_children(t_table *table);
void	wait_all_children(void);
int		take_forks(t_philo *philo);
void	release_forks(t_philo *philo);
int		philo_eat(t_philo *philo);
int		philo_sleep_think(t_philo *philo);
void	wait_for_death(void);

#endif
