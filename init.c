/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:54:18 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/06 17:53:56 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	*routine(void *arg)
{
	// thread function, what it will do, what happens in the philo lifespan
	t_philos *philo = (t_philos *)arg; // casting back to t_philos so we can use data of the philosopher struct
	while (1) // while process is running
	{
		printf("👶 philo[%d] is born at %zu\n", philo->id, get_current_ms() - philo->tob);
		usleep(500000);
		thinks(philo);
		eats(philo);
		sleeps(philo);
	}
	return (arg); // exit status will point to the memory of this arg, can be used with pthreadjoin to avoid threads running indefinitely
}

void start_simulation(t_data *program, t_philos *philo)
{
	// create lifespan thread for each philosopher
	printf("\n - philos_n: %d\n - time_to_die: %d\n - time_to_eat: %d\n - time_to_sleep: %d\n -eat_count: %d\n\n", program->philos_n, program->time_to_die, program->time_to_eat, program->time_to_sleep, program->eat_count);
	int i;
	i = 0;
	(void)philo;
	while (i < program->philos_n)
	{
		// if(program->eat_count >= philo[i].has_eaten) // needs to go in routine as just the respective philo needs to end eating
		// {
		// 	// dies(&program->philo[i]); // was just testing this out no nee to be here
		// 	break;
		// }
		pthread_create(&program->philo[i].lifespan, NULL, routine, &program->philo[i]);
		usleep(500000);
		i++;
	}
}

int init_philos(t_data *program)
{
	int i;

	i = 0;
	program->philo = malloc(sizeof(t_philos) * program->philos_n);
	if (!program->philo)
	{
		printf("Error: malloc of t_data program->philo failed\n");
		return(0);
	}
	while(i < program->philos_n)
	{
		program->philo[i].id = i + 1;
		program->philo[i].has_eaten = 0;
		program->philo[i].is_eating = 0;
		program->philo[i].is_sleeping = 0;
		program->philo[i].is_thinking = 0;
		program->philo[i].is_dead = 0;
		program->philo[i].tob = get_current_ms();
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

	i = 0;
	//forks[program.philos_n]; ?
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
	// printf("INIT.C LINE 17---> philos_n: %d, time_to_die: %d, time_to_eat: %d, time_to_sleep: %d eat_count: %d\n", program->philos_n, program->time_to_die, program->time_to_eat, program->time_to_sleep, program->eat_count);
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