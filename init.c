/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:54:18 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/08 15:56:40 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	*routine(void *arg)
{
	// thread function, what it will do, what happens in the philo lifespan
	t_philos *philo = (t_philos *)arg; // casting back to t_philos so we can use data of the philosopher struct
	// while (1) // while process is running
	// {
		// philo->meal_wait = 1000;
		philo->tob = get_curr_ms(philo->args.unix_start);
		if(philo->status.is_born == 0)
		{
			printf("👶 philo[%d] is born at %zu\n", philo->id, philo->tob);
			philo->status.is_born = 1;
		}
		else
		{
			if(philo->meals_n < philo->args.meals_limit && philo->status.is_dead == 0)
			{
				// printf("philo[%d] is alive\n", philo->id);
				if(philo->status.is_thinking == 0)
				{
					thinks(philo);
					philo->status.is_thinking = 1;
				}
				else if(philo->status.is_eating == 0)
				{
					eats(philo);
					philo->meals_n++;
					philo->status.is_eating = 1;
				}
				else if(philo->status.is_sleeping == 0)
				{
					sleeps(philo);
					philo->status.is_sleeping = 1;
				}
				if(philo->meal_start >= (size_t)philo->args.ttd)
				{
					printf("💀 %zu philo[%d] died\n",  get_unix_timestamp() - philo->tob, philo->id);
					philo->status.is_dead = 1;
				}
			}
		}
	// }
	usleep(philo->args.ttd * 1000); //milliseconds tp microseconds
	return (philo); // exit status will point to the memory of this arg, can be used with pthreadjoin to avoid threads running indefinitely
}

int start_simulation(t_data *program, t_philos *philo)
{
	// create lifespan thread for each philosopher
	printf("\n - philos_n: %d\n - ttd: %d\n - tte: %d\n - tts: %d\n - meals_limit: %d\n\n", program->args.philos_n, program->args.ttd, program->args.tte, program->args.tts, program->args.meals_limit);
	int i;
	i = 0;
	(void)philo;
	while (i < program->args.philos_n)
	{
		if(pthread_create(&program->philo[i].lifespan, NULL, routine, &program->philo[i]) != 0)
			{
				printf("Error: failed to create thread\n"); // should I also destroy all threads and mutexes here?
				return(0);
			}
		if(philo[i].meals_n == program->args.meals_limit || philo[i].status.is_dead == 1)
		{
			// dies(&program->philo[i]); // was just testing this out no nee to be here
			printf("👶 philo[%d] MEAL TIMES %d MEAL LIMIT %d\n", philo->id, philo[i].meals_n, program->args.meals_limit );
			break;
		}
		pthread_join(program->philo[i].lifespan, NULL);
		i++;
	}
	return(1);
}

int init_philos(t_data *program)
{
	int i;

	i = 0;
	program->philo = malloc(sizeof(t_philos) * program->args.philos_n);
	if (!program->philo)
	{
		printf("Error: malloc of t_data program->philo failed\n");
		return(0);
	}
	while(i < program->args.philos_n)
	{
		program->philo[i].id = i + 1;
		// program->philo[i].tob = get_unix_timestamp();
		program->philo[i].meals_n = 0;
		program->philo[i].curr_fork = &program->forks[i]; // left fork
		program->philo[i].args = program->args;
		if (i == 1)
				program->philo[i].prev_fork = &program->forks[program->args.philos_n]; // right fork of 1st is the fork of last
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
	program->forks = malloc(sizeof(pthread_mutex_t) * program->args.philos_n);
	if (!program->forks)
	{
		printf("Error: malloc of t_data program->forks failed\n");
		return(0);
	}
	while(i <= program->args.philos_n)
	{
		pthread_mutex_init(&program->forks[i], NULL);
		i++;
	}
	return(1);
}

int check_values(t_rules *args)
{
	// printf("INIT.C LINE 17---> philos_n: %d, ttd: %d, tte: %d, tts: %d meals_limit: %d\n", args->philos_n, args->ttd, args->tte, args->tts, args->meals_limit);
	if(args->philos_n < 1 || args->ttd < 1 || args->tte < 1 || args->tts < 1)
	{
		printf("Error: the first 4 values should be greater than 0\n");
		return(0);
	}
	if(args->meals_limit < 0)
	{
		printf("Error: the 5th value(meals_limit) should not be negative\n");// check if condition needed for meals_limit
		return(0);
	}
	if(args->philos_n > 200)
	{
		printf("Error: not allowed to test philos_n greater than 200\n"); // check if needed
		return(0);
	}
	// what about intmax limit?
	return(1);
}

int	init_data(int argc, char **argv, t_data *program, t_rules *args)
{
	args->philos_n = ft_atoi(argv[1]); // i have to hanldle letters? should not give 0??
	args->ttd = ft_atoi(argv[2]) * 1000;
	args->tte = ft_atoi(argv[3]) * 1000;
	args->tts = ft_atoi(argv[4]) * 1000;
	args->unix_start = get_unix_timestamp(); // it's already in ms
	if (argc == 6)
		args->meals_limit = ft_atoi(argv[5]);
	else
		args->meals_limit = 0; // what happens when this is not added?
	if(!check_values(args))
		return(0);
	if(!init_forks(program))
		return(0);
	if(!init_philos(program))
		return(0);
	return (1);
}