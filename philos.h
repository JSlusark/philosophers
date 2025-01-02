/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:47:09 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/02 17:15:13 by jslusark         ###   ########.fr       */
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
	// data of philo - each philo should be a thread?
	int id; // index of philo?
	int eat_counter; // do we still need this when eat_req is not added as argv?
	// index of prev_fork (philo id - 1)
	// index of own_fork (philo id)
	// index of next_fork (philo id + 1)
	// is_eating flag?
	// is_dead flag?
	// is_sleeping flag?
} t_philo;

typedef	struct s_data
{
	// i suppose we will have to store the data of the program here for further use
	int philos_n;
	int forks_n;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int eat_req; // this is added optionally? what happens when not added?
}	t_data;


//utility functions
int	ft_atoi(const char *nptr);

#endif
