/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:54:18 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/10 17:20:52 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

#include "philos.h"

bool check_death(t_philos *philo)
{
	size_t current_time = get_curr_ms(philo->args->unix_start);

	pthread_mutex_lock(&philo->args->dead_lock);
	if (philo->args->found_dead)
	{
		pthread_mutex_unlock(&philo->args->dead_lock);
		return true;
	}

	if ((current_time - philo->lastmeal_time) >= philo->args->ttd)
	{
		philo->args->found_dead = true;
		pthread_mutex_lock(&philo->args->output_lock);
		printf(DEATH"%zu %d died\n"RESET, current_time, philo->id);
		pthread_mutex_unlock(&philo->args->output_lock);
		pthread_mutex_unlock(&philo->args->dead_lock);
		return true;
	}

	pthread_mutex_unlock(&philo->args->dead_lock);
	return false;
}


void *routine(void *arg)
{
	t_philos *philo = (t_philos *)arg;

	while (!check_death(philo))
	{
		eats(philo);
		sleeps(philo);
		thinks(philo);
	}
	return NULL;
}

bool	start_simulation(t_data *program)
{
	// printf("\n - philos_n: %d\n - ttd: %d\n - tte: %d\n - tts: %d\n - meals_limit: %d\n\n", program->args.philos_n, program->args.ttd, program->args.tte, program->args.tts, program->args.meals_limit);
	int i;

	i = 0;
	while (i < program->args.philos_n && program->args.found_dead == false)
	{
		if(pthread_create(&program->philo[i].lifespan, NULL, &routine, &program->philo[i]) != 0)
		{
			printf("Error: failed to create thread\n"); // should I also destroy all threads and mutexes here?
			return (false);
		}
		i++;
	}
	i = 0;
	while (i < program->args.philos_n)
	{
		pthread_join(program->philo[i].lifespan, NULL);// <- stops threads from running -.-''
		i++;
	}
	return(true);
}

bool	init_philos(t_data *program)
{
	int i;

	program->philo = malloc(sizeof(t_philos) * program->args.philos_n);
	if (!program->philo)
	{
		printf("Error: malloc of t_data program->philo failed\n");
		return (false);
	}
	i = 0;
	while(i < program->args.philos_n)
	{
		program->philo[i].id = i + 1;
		program->philo[i].args = &program->args;
		program->philo[i].meals_n = 0;
		program->philo[i].lastmeal_time = get_curr_ms(program->args.unix_start);
		program->philo[i].left_fork = &program->forks[i];
		program->philo[i].right_fork = &program->forks[(i + 1) % program->args.philos_n];
		i++;
	}
	return (true);
}

bool	init_forks(t_data *program)
{
	int i;

	program->forks = malloc(sizeof(pthread_mutex_t) * program->args.philos_n);
	if (!program->forks)
	{
		printf("Error: malloc of t_data program->forks failed\n");
		return (false);
	}
	i = 0; // as it's an array we loop from 0 to philos_n - 1
	while(i < program->args.philos_n) // if philos_n is 50, we have 50 forks indexed 0 to 49
	{
	pthread_mutex_init(&program->forks[i], NULL);
		i++;
	}
	return(true);
}

bool	check_values(t_rules *args)
{
	// printf("INIT.C LINE 17---> philos_n: %d, ttd: %d, tte: %d, tts: %d meals_limit: %d\n", args->philos_n, args->ttd, args->tte, args->tts, args->meals_limit);
	if(args->philos_n < 1 || args->ttd < 1 || args->tte < 1 || args->tts < 1)
	{
		printf("Error: the first 4 values should be greater than 0\n"); // check if greater than 0 or -1
		return (false);
	}
	if(args->philos_n > 200)
	{
		printf("Error: not allowed to test philos_n greater than 200\n"); // check if needed
		return (false);
	}
	// what about intmax limit?
	return (true);
}

bool	init_data(int argc, char **argv, t_data *program)
{
	// program = malloc(sizeof(t_data));
	program->args.philos_n = ft_atoi(argv[1]);
	program->args.ttd = ft_atoi(argv[2]);
	program->args.tte = ft_atoi(argv[3]);
	program->args.tts = ft_atoi(argv[4]);
	program->args.found_dead = false;
	pthread_mutex_init(&program->args.dead_lock, NULL);
	pthread_mutex_init(&program->args.output_lock, NULL);
	program->args.unix_start = get_unix_timestamp(); // ms since 1970 to start of program, does not need conversion (it's in milliseconds)
	if (argc == 6)
	{
		program->args.meals_limit = ft_atoi(argv[5]);
		if(program->args.meals_limit < 0)
		{
			printf("Error: the 5th value(meals_limit) should not be negative\n");// check if condition needed for meals_limit
			return (false);
		}
	}
	else
		program->args.meals_limit = -1; // no limit should be given if not inputed from the user
	if(!check_values(&program->args))
		return (false);
	if(!init_forks(program))
		return (false);
	if(!init_philos(program))
		return (false);
	return (true);
}

// void cleanup(t_data *program)
// {
// 	for (int i = 0; i < program->args.philos_n; i++)
// 		pthread_mutex_destroy(&program->forks[i]);
// 	free(program->forks);
// 	free(program->philo);
// }

