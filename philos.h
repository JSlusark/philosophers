/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:47:09 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/07 14:56:48 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOS_H
# define PHILOS_H


#define RESET "\033[0m"
#define DEATH "\033[31m"   // Red for death
#define SLEEP "\033[34m"   // Blue for sleep
#define THINK "\033[35m"   // Purple for thinking
#define FORK1 "\033[33m"   // Yellow for fork 1
#define FORK2 "\033[38;5;214m" // Orange for fork 2 (uses extended ANSI)
#define GREEN "\033[32m"    // Green for highlight 1
#define YELLOW "\033[33m"    // Yellow for highlight 2

#include <string.h>     // memset
#include <stdio.h>      // printf
#include <stdlib.h>     // malloc, free
#include <unistd.h>     // write, usleep
#include <sys/time.h>   // gettimeofday
#include <pthread.h>    // pthread_create, pthread_detach , pthread_join, pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock
#include <stdbool.h> // added to use bool type instead thna 0/1

typedef struct s_rules
{
	int				philos_n; // number of philosophers and forks
	int				meals_limit; // number of times philo has eaten - null by default
	int				ttt; // could use this to manage the philosopher routine?
	size_t				ttd; // time limit a philo can stay without eating, if surpassed philo dies and simulation stops
	int				tte; // time it takes for each philo to eat
	int				tts; // time it takes for each philo to sleep
	bool			found_dead; // flag for checking if a philo has died and stops the simulation
	pthread_mutex_t	dead_lock; // mutex teo lock the found_dead flag
	long			unix_start;
}	t_rules;

typedef struct s_props // not assigning as null by default
{
	bool is_born;
	bool is_alive;
	bool is_thinking;
	bool is_eating;
	bool is_sleeping;
	bool is_dead;
	bool is_full;
}	t_props;

typedef struct s_philos // struct for each philosopher
{
	int				id; // index of philosopher in struct array, can be useful with phtread_join/pthread_detach
	pthread_t		lifespan; // thread of each philosopher's lifespan
	size_t			tob; // time of birth timestamp
	size_t			stop_timer; // start of action timestamp
	t_props			status; // struct to manage the status of each philosopher
	int				meals_n; // number of times philo has eaten - null by default
	size_t			meal_wait; // counter to see if philo has eaten before ttd
	size_t			meal_start; // timestamp of when philo starts eating
	size_t			meal_end; // timestamp to check last meal and if exceeding ttd philo has to die
	pthread_mutex_t	*left_fork; // left fork
	pthread_mutex_t	*right_fork; // right fork
	t_rules				*args; // pointer to args struct (which modifies the data args of the program)
	// pthread_mutex_t *next_fork; // left of left fork -- curious if this can help more later
} t_philos;

typedef	struct s_data
{
	// i suppose we will have to store the data of the program here for further use
	t_rules args; // struct to store the arguments
	t_philos	*philo; // array of structs (each struct is a philosopher)
	pthread_mutex_t *forks; // array of mutexs (each mutex is a fork)
	// bool	is_dead; // flag to stop simulation ---- works better in args i think
	int *forks_taken;
}	t_data;

//utility functions
int	ft_atoi(const char *nptr);
long get_unix_timestamp(void); // to be subctracted from tob to get passing time in ms
size_t get_curr_ms(long start);

//parsing checks
bool	parse_args(int argc);
bool check_values(t_rules *args); // called in init_data

// init functions
bool	init_data(int argc, char **argv, t_data *program, t_rules *args);
bool init_forks(t_data *program);
bool init_philos(t_data *program);

// simulation functions
bool	start_simulation(t_data *program, t_philos *philo); // starts threads
void	*routine(void *arg); // starts the routine of each philosopher
void	thinks(t_philos *philo); // action
void	eats(t_philos *philo, pthread_mutex_t *first_fork, pthread_mutex_t *previous_fork);
void	sleeps(t_philos *philo); // action
void	dies(t_philos *philo); // action



#endif
