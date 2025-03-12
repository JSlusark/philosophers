/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:47:09 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/12 17:15:50 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOS_H
# define PHILOS_H

#include <string.h>     // memset
#include <stdio.h>      // printf
#include <stdlib.h>     // malloc, free
#include <unistd.h>     // write, usleep
#include <sys/time.h>   // gettimeofday
#include <pthread.h>    // pthread_create, pthread_detach , pthread_join, pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock
#include <stdbool.h> // added to use bool type instead thna 0/1

// ANSI colors for output
#define RESET "\033[0m"
#define DEATH "\033[31m"   // Red for death
#define SLEEP "\033[34m"   // Blue for sleep
#define THINK "\033[35m"   // Purple for thinking
#define FORK1 "\033[33m"   // Yellow for fork 1
#define FORK2 "\033[38;5;214m" // Orange for fork 2 (uses extended ANSI)
#define GREEN "\033[32m"    // Green for highlight 1
#define YELLOW "\033[33m"    // Yellow for highlight 2
// Struct for simulation rules
typedef struct s_rules
{
	int				philos_n; // number of philosophers and forks
	int				meals_limit; // number of times philo has eaten - null by default
	size_t				ttt; // could use this to manage the philosopher routine?
	size_t				ttd; // time limit a philo can stay without eating, if surpassed philo dies and simulation stops
	size_t				tte; // time it takes for each philo to eat
	size_t				tts; // time it takes for each philo to sleep
	bool			found_dead; // flag for checking if a philo has died and stops the simulation
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	output_lock;
	long			unix_start;
}	t_rules;

typedef struct s_props // not assigning as null by default
{
	// bool is_born;
	// bool ate;
	bool is_eating;
	bool is_sleeping;
	bool is_thinking;
	bool is_dead;
	size_t timer_stopped; // time since last meal time until stop of simulation
}	t_props;

typedef struct s_philos // struct for each philosopher
{
	int				id;
	pthread_t		lifespan;
	size_t			last_meal_time;
	int				meals_n;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_rules			*args; // pointer to args struct (which modifies the data args of the program)
	t_props			status; // status of the philosopher
	// pthread_mutex_t *next_fork; // left of left fork -- curious if this can help more later
} t_philos;

// Global simulation struct
typedef struct s_data
{
	t_rules			args;
	t_philos		*philo;
	pthread_mutex_t	*forks;
}	t_data;

// Utility functions
long	get_unix_timestamp(void);
size_t	get_curr_ms(long start);
int		ft_atoi(const char *nptr);
void	ft_usleep(size_t milliseconds, t_philos *philo);
// Initialization functions
bool	init_data(int argc, char **argv, t_data *program);
bool	init_philos(t_data *program);
bool	init_forks(t_data *program);

// Simulation functions
bool	start_simulation(t_data *program);
void	*routine(void *arg); // starts the routine of each philosopher
void	thinks(t_philos *philo);
void	eats(t_philos *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork);
void	sleeps(t_philos *philo);
bool	check_death(t_philos *philo);
void	cleanup(t_data *program);
void *monitor(void *arg);

//testing and debugging
void print_mealcount(t_data *program);
void print_status(t_data *program);



#endif
