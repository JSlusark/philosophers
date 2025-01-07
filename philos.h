/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:47:09 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/07 16:00:56 by jslusark         ###   ########.fr       */
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

typedef struct s_rules
{
	int				philos_n; // number of philosophers and forks
	int				meals_limit; // number of times philo has eaten - null by default
	int				ttt; // could use this to manage the philosopher routine?
	int				ttd; // time limit a philo can stay without eating, if surpassed philo dies and simulation stops
	int				tte; // time it takes for each philo to eat
	int				tts; // time it takes for each philo to sleep
}	t_rules;

typedef struct s_props // not assigning as null by default
{
	int is_born;
	int is_thinking;
	int is_eating;
	int is_sleeping;
	int is_dead;
}	t_props;

typedef struct s_philos // struct for each philosopher
{
	int				id; // index of philosopher in struct array, can be useful with phtread_join/pthread_detach
	pthread_t		lifespan; // thread of each philosopher's lifespan
	size_t			tob; // time of birth timestamp
	size_t			curr_t; // time of birth timestamp
	t_props			status; // struct to manage the status of each philosopher
	int				meals_n; // number of times philo has eaten - null by default
	size_t			meal_wait; // counter to see if philo has eaten before ttd
	size_t			meal_start; // timestamp of when philo starts eating
	size_t			meal_end; // timestamp to check last meal and if exceeding ttd philo has to die
	pthread_mutex_t	*curr_fork; // left fork
	pthread_mutex_t	*prev_fork; // right fork
	t_rules args; // struct to store the arguments
	// pthread_mutex_t *next_fork; // left of left fork -- curious if this can help more later
} t_philos;

typedef	struct s_data
{
	// i suppose we will have to store the data of the program here for further use
	t_rules args; // struct to store the arguments
	t_philos	*philo; // array of structs (each struct is a philosopher)
	pthread_mutex_t *forks; // array of mutexs (each mutex is a fork)
	int	is_dead; // flag to stop simulation
	int *forks_taken;
}	t_data;

//utility functions
int	ft_atoi(const char *nptr);
long get_current_ms(void); // to be subctracted from tob to get passing time in ms

//parsing checks
int	parse_args(int argc);
int check_values(t_rules *args); // called in init_data

// init functions
int	init_data(int argc, char **argv, t_data *program, t_rules *args);
int init_forks(t_data *program);
int init_philos(t_data *program);

// simulation functions
int	start_simulation(t_data *program, t_philos *philo); // starts threads
void	*routine(void *arg); // starts the routine of each philosopher
void	thinks(t_philos *philo); // action
void	eats(t_philos *philo); // action
void	sleeps(t_philos *philo); // action
void	dies(t_philos *philo); // action



#endif
