/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:54:18 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/17 15:55:50 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void *monitor(void *arg)
{
	t_data *program = (t_data *)arg;
	int	i;

	while (1) //if monitor_death || monitor_meals is true, return  null
	{
		// monitor_death
		for (i = 0; i < program->args.philos_n; i++)
		{
			if (starvation(&program->philo[i], NULL)) // or meals met (no actobity to be prented) -- check if i need to put (&& !philo->eating)
				return NULL; // Stop monitor thread immediately, return true if not false
		}

		// monitor_meals
		if (program->args.meals_limit > 0 && !program->args.found_dead)
		{
			int group_ate_enough = 0;
			for (i = 0; i < program->args.philos_n; i++)
			{
				if (program->philo[i].meals_n >= program->args.meals_limit) // checks every philo to see if meals are met
					group_ate_enough++;
			}
			if (group_ate_enough == program->args.philos_n)// after looping philo 1 by one we checked if all reached the eat_limit
			{
				program->args.found_dead = true;
				break;
			}
		}
		usleep(500); /// commened for now
	}
	return (NULL); // from if monitor functions
}


bool someone_died(t_philos *philo)
{
	if (philo->args->found_dead || philo->is_dead || philo->elapsed_time >= philo->args->ttd)
	{
		// pthread_mutex_lock(&philo->args->alert_lock);
		// printf("	-> %zu philo %d says someone is dead⁉️\n", get_curr_ms(philo->args->unix_start), philo->id);
		// pthread_mutex_unlock(&philo->args->alert_lock);
		return true;
	}
	return false;
}



void *routine(void *arg)
{
	t_philos *philo = (t_philos *)arg;

	if(philo->id % 2 == 0)
			ft_usleep(philo->args->tte / 2, philo); // delay to avoid even philos to be quicker than odd

	while (!someone_died(philo)) // why not using the starvation fucntion here instead?
	{
		if(philo->id % 2 != 0) // odd get first the left (theirs) and teh right (i - 1)
			eats(philo, philo->left_fork, philo->right_fork);
		else // even get first the right (i - 1) and then left (theirs)
			eats(philo, philo->right_fork, philo->left_fork); // even die
		sleeps(philo);
		thinks(philo);
	}
	return (NULL);
}


bool start_simulation(t_data *program)
{
	// pthread_t monitor_thread;
	int i;

	// Create philosopher threads
	for (i = 0; i < program->args.philos_n; i++)
	{
		if (pthread_create(&program->philo[i].lifespan, NULL, &routine, &program->philo[i]) != 0)
		{
			printf("Error: failed to create philosopher thread\n");
			return (false);
		}
	}

	// Create monitor thread
	// if (pthread_create(&monitor_thread, NULL, &monitor, program) != 0)
	// {
	// 	printf("Error: failed to create monitor thread\n");
	// 	return (false);
	// }

	// Wait for philosophers to finish
	for (i = 0; i < program->args.philos_n; i++)
		pthread_join(program->philo[i].lifespan, NULL);

	// Wait for monitor to finish
	// pthread_join(monitor_thread, NULL);
	print_status(program);
	print_mealcount(program); // debugger for meal count


	return (true);
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
		program->philo[i].left_fork = &program->forks[i]; // left fork equal to philo n
		if(i == 0) // philo[0] 1 should get fork[last_i] philos_n-1
			program->philo[i].right_fork = &program->forks[program->args.philos_n - 1];
		else
			program->philo[i].right_fork = &program->forks[i - 1];
		program->philo[i].elapsed_time = 0;
		program->philo[i].is_eating = false;
		program->philo[i].is_sleeping = false;
		program->philo[i].is_thinking = false;
		program->philo[i].is_dead = false;
		printf(SLEEP"	Philo %d: left_fork = fork[%ld], right_fork = fork[%ld]\n"RESET,
			program->philo[i].id,
			program->philo[i].left_fork - program->forks,
			program->philo[i].right_fork - program->forks);
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
	pthread_mutex_init(&program->args.status_lock, NULL);
	pthread_mutex_init(&program->args.alert_lock, NULL); // alert tahet tells other philos to die if someone is dead
	pthread_mutex_init(&program->args.output_lock, NULL);
	program->args.unix_start = get_unix_timestamp(); // ms since 1970 to start of program, does not need conversion (it's in milliseconds)
	if (argc == 6)
		program->args.meals_limit = ft_atoi(argv[5]);
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

