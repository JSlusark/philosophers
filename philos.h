/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:47:09 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/06 17:38:33 by jslusark         ###   ########.fr       */
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

typedef struct s_philos // struct for each philosopher
{
	pthread_t lifespan; // thread of each philosopher's lifespan
	int id; // index of philosopher in struct array, can be useful with phtread_join/pthread_detach
	int has_eaten; // number of times philo has eaten
	int is_eating; // curr and prev fork are locked
	int is_sleeping;
	int is_thinking;
	int is_dead;
	int time_to_think; // could use this to manage the philosopher routine?
	int time_to_die; // time limit a philo can stay without eating, if surpassed philo dies and simulation stops
	int time_to_eat; // time it takes for each philo to eat
	int time_to_sleep; // time it takes for each philo to sleep
	size_t			tob; // time of birth timestamp
	size_t			started_eating; // timestamp of when philo starts eating
	size_t			ended_eating; // timestamp to check last meal and if exceeding ttd philo has to die
	pthread_mutex_t *curr_fork; // left fork
	pthread_mutex_t *prev_fork; // right fork
	// pthread_mutex_t *next_fork; // left of left fork -- curious if this can help more later
} t_philos;

typedef	struct s_data
{
	// i suppose we will have to store the data of the program here for further use
	int philos_n; // number of philosophers and forks
	int time_to_die; // time limit a philo can stay without eating, if surpassed philo dies and simulation stops
	int time_to_eat; // time it takes for each philo to eat
	int time_to_sleep; // time it takes for each philo to sleep
	int time_to_think; // could use this to manage the philosopher routine?
	int eat_count; // number of times each philo must eat, if reached simulation stops
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
int check_values(t_data *program); // called in init_data

// init functions
int	init_data(int argc, char **argv, t_data *program);
int init_forks(t_data *program);
int init_philos(t_data *program);

// simulation functions
void	start_simulation(t_data *program, t_philos *philo); // starts threads
void	*routine(void *arg); // starts the routine of each philosopher
void	thinks(t_philos *philo); // action
void	eats(t_philos *philo); // action
void	sleeps(t_philos *philo); // action
void	dies(t_philos *philo); // action



#endif
