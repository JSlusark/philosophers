/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:54:18 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/03 17:24:28 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int init_philos(t_data *program)
{
	int i;

	i = 1; // doing i 1 and loop until <= as not starting from 0
	program->philo = malloc(sizeof(t_philo) * program->philos_n);
	if (!program->philo)
	{
		printf("Error: malloc of t_data program->philo failed\n");
		return(0);
	}
	while(i <= program->philos_n)
	{
		program->philo[i].id = i;
		program->philo[i].has_eaten = 0;
		program->philo[i].is_eating = 0;
		program->philo[i].is_sleeping = 0;
		program->philo[i].is_thinking = 0;
		program->philo[i].is_dead = 0;
		program->philo[i].started_eating = get_current_ms(); // no need to initialize here?
		// program->philo[i].ended_eating = 0; // no need to initialize here?
		program->philo[i].curr_fork = &program->forks[i]; // left fork
		if (i == 1)
				program->philo[i].prev_fork = &program->forks[program->philos_n]; // right fork of 1st is the fork of last
		else
				program->philo[i].prev_fork = &program->forks[i - 1]; // right fork is the fork of previous philosopher
		i++;
	}
	return(1);
}

int init_forks(t_data *program)
{
	int i;

	i = 1; // doing i 1 and loop until <= as not starting from 0
	// i know the number of forks needed is philos_n so do not need to malloc?
	program->forks = malloc(sizeof(pthread_mutex_t) * program->philos_n);
	if (!program->forks)
	{
		printf("Error: malloc of t_data program->forks failed\n");
		return(0);
	}
	while(i <= program->philos_n)
	{
		pthread_mutex_init(&program->forks[i], NULL);
		i++;
	}
	return(1);
}

int check_values(t_data *program)
{
	printf("INIT.C LINE 17---> philos_n: %d, time_to_die: %d, time_to_eat: %d, time_to_sleep: %d eat_count: %d\n", program->philos_n, program->time_to_die, program->time_to_eat, program->time_to_sleep, program->eat_count);
	if(program->philos_n < 1 || program->time_to_die < 1 || program->time_to_eat < 1 || program->time_to_sleep < 1)
	{
		printf("Error: the first 4 values should be greater than 0\n");
		return(0);
	}
	if(program->eat_count < 0)
	{
		printf("Error: the 5th value(eat_count) should not be negative\n");// check if condition needed for eat_count
		return(0);
	}
	if(program->philos_n > 200)
	{
		printf("Error: not allowed to test philos_n greater than 200\n"); // check if needed
		return(0);
	}
	// what about intmax limit?
	return(1);
}

int	init_data(int argc, char **argv, t_data *program)
{
	program->philos_n = ft_atoi(argv[1]); // i have to hanldle letters? should not give 0??
	program->time_to_die = ft_atoi(argv[2]);
	program->time_to_eat = ft_atoi(argv[3]);
	program->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		program->eat_count = ft_atoi(argv[5]);
	else
		program->eat_count = 0; // what happens when this is not added?
	if(!check_values(program))
		return(0);
	if(!init_forks(program))
		return(0);
	if(!init_philos(program))
		return(0);
	return (1);
}