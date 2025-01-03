/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:47:09 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/03 16:57:29 by jslusark         ###   ########.fr       */
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

typedef struct s_philo
{
	pthread_t thread; // thread for each philo
	int id; // index of philo?
	int has_eaten; // number of times philo has eaten
	int is_eating; // curr and prev fork are locked
	int is_sleeping;
	int is_thinking;
	int is_dead;
	size_t			started_eating; // timestamp of when philo starts eating
	size_t			ended_eating; // timestamp to check last meal and if exceeding ttd philo has to die
	pthread_mutex_t *curr_fork; // left fork
	pthread_mutex_t *prev_fork; // right fork
	// pthread_mutex_t *next_fork; // left of left fork -- curious if this can help more later
} t_philo;

typedef	struct s_data
{
	// i suppose we will have to store the data of the program here for further use
	int philos_n; // number of philosophers and forks
	int time_to_die; // time limit a philo can stay without eating, if surpassed philo dies and simulation stops
	int time_to_eat; // time it takes for each philo to eat
	int time_to_sleep; // time it takes for each philo to sleep
	int eat_count; // number of times each philo must eat, if reached simulation stops
	t_philo	*philo;
	pthread_mutex_t *forks;
	int	is_dead; // flag to stop simulation
	int *forks_taken;
}	t_data;

//utility functions
int	ft_atoi(const char *nptr);
int	init_data(int argc, char **argv, t_data *program);
long get_current_ms(void);

#endif
