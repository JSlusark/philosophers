/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:47:09 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/21 13:07:05 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOS_H
# define PHILOS_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>

// ANSI colors for output
# define RESET "\033[0m"
# define DEATH "\033[31m"
# define SLEEP "\033[34m"
# define THINK "\033[35m"
# define FORK1 "\033[33m"
# define FORK2 "\033[38;5;214m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
// Struct for simulation rules
typedef struct s_rules
{
	int				philos_n;
	int				meals_limit;
	bool			no_optional;
	size_t			ttd;
	size_t			tte;
	size_t			tts;
	bool			found_dead;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	output_lock;
	pthread_mutex_t	status_lock;
	long			unix_start;
}	t_rules;

// Philosopher
typedef struct s_philos
{
	int				id;
	pthread_t		lifespan;
	size_t			last_meal_time;
	int				meals_n;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*timer_lock;
	t_rules			*args;
	bool			is_eating;
	bool			is_sleeping;
	bool			is_thinking;
	bool			is_dead;
	size_t			elapsed_time;
	size_t			start_activity;
}	t_philos;

// Global Simulation Data
typedef struct s_data
{
	t_rules			args;
	t_philos		*philo;
	pthread_mutex_t	*forks;
}	t_data;

// main functions
bool	parse_args(int argc, char **argv);
bool	check_values(t_rules *args);
bool	init_data(int argc, char **argv, t_data *program);
void	cleanup(t_data *program);

// allocating functions
bool	init_forks(t_data *program);
bool	init_philos(t_data *program);
void	init_status(t_data *program, int *i);

// starting threads functions
bool	start_simulation(t_data *program);
bool	create_lifespan_threads(t_data *program);
void	*routine(void *arg);
bool	death_alert(t_philos *philo);
bool	lonely_philo(t_philos *philo, pthread_mutex_t *first_fork);

// routine  functions
void	eats(t_philos *philo, pthread_mutex_t *first_fork,
			pthread_mutex_t *second_fork);
void	sleeps(t_philos *philo);
void	thinks(t_philos *philo);
void	print_activity(t_philos *philo, size_t time,
			char *message, size_t delay);
bool	interrupt_activity(t_philos *philo);

// monitor functions
void	*monitor(void *arg);
bool	found_death(t_philos *philo);
bool	meal_check(t_philos *philo, int *group_ate_enough);

// utility functions
size_t	get_curr_ms(long start);
long	get_unix_timestamp(void);
int		ft_atoi(const char *nptr);
void	ft_usleep(size_t milliseconds, t_philos *philo);

//testing and debugging
void	routine_debugging(t_philos *philo);
void	print_status(t_data *program);
void	print_philo(t_philos *philo);
void	print_mealcount(t_data *program);

#endif
