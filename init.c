/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:54:18 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/20 17:27:38 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void *monitor(void *arg)
{
	t_data *program = (t_data *)arg;
	int	i;

	while (1) //if monitor_death || monitor_meals is true, return  null
	{
		int group_ate_enough = 0;
		for (i = 0; i < program->args.philos_n; i++)
		{
			// count meals
			pthread_mutex_lock(&program->philo[i].args->meal_lock);// i would call it activity lock
			if(program->philo[i].args->meals_limit != -1) // checks every philo to see if meals are met
			{

				if (program->philo[i].meals_n >= program->args.meals_limit) // checks every philo to see if meals are met
				group_ate_enough++;
				pthread_mutex_unlock(&program->philo[i].args->meal_lock);// i would call it activity lock
				if (group_ate_enough == program->args.philos_n)// after looping philo 1 by one we checked if all reached the eat_limit
				{
					pthread_mutex_lock(&program->philo[i].args->status_lock);// i would call it activity lock
					program->args.found_dead = true;
					pthread_mutex_unlock(&program->philo[i].args->status_lock);// i would call it activity lock
					return(NULL);
				}
			}
			else
				pthread_mutex_unlock(&program->philo[i].args->meal_lock);// i would call it activity lock


			// monitor death
			pthread_mutex_lock(&program->philo[i].args->output_lock);// i would call it activity lock
			// printf("---[MONITOR]--- philo %d\n", i);
				pthread_mutex_lock(&program->philo[i].args->status_lock);// i would call it activity lock
				// program->philo[i].elapsed_time = get_curr_ms(program->philo[i].args->unix_start) - program->philo[i].ref_time;
				program->philo[i].elapsed_time = get_curr_ms(program->philo[i].args->unix_start) - program->philo[i].last_meal_time;
				pthread_mutex_unlock(&program->philo[i].args->status_lock);// i would call it activity lock
				// if(program->philo->is_dead)
			// printf(DEATH"%d ELAPSED %zu\n"RESET, program->philo[i].id, program->philo[i].elapsed_time);// i would call it activity lock

			if(program->philo[i].elapsed_time >= program->args.ttd && !program->philo[i].is_eating)
			{
				// printf(DEATH"---[MONITOR]--- philo %d\nisdead\n"RESET, i+1);
				printf(DEATH"%zu %d is dead"RESET, get_curr_ms(program->args.unix_start), program->philo[i].id);// i would call it activity lock
				if (program->philo[i].is_eating)
				printf(GREEN" 🍝: %d <---- eating"RESET, program->philo[i].meals_n);
				else if (program->philo[i].is_sleeping)
				printf(SLEEP" <---- sleeping"RESET);
				else if (program->philo[i].is_thinking)
				printf(THINK" <---- thinking"RESET);
				else
				printf(FORK1" <---- no status is active"RESET);
				// elapsed time to check death
				printf(" - timer %s%zums%s",
					program->philo[i].elapsed_time == 0 ? GREEN :
					(program->philo[i].elapsed_time >= program->philo[i].args->ttd ? DEATH"   ☠️ ☠️ ☠️   " : RESET), // Set color
					program->philo[i].elapsed_time, // Value
					RESET); // Reset color
					printf("\n");
				pthread_mutex_lock(&program->philo[i].args->status_lock);// i would call it activity lock
				program->philo[i].is_dead = true;
				program->args.found_dead = true;
				pthread_mutex_unlock(&program->philo[i].args->status_lock);// i would call it activity lock
				pthread_mutex_unlock(&program->philo[i].args->output_lock);// i would call it activity lock
				return(NULL);
			}
			pthread_mutex_unlock(&program->philo[i].args->output_lock);// i would call it activity lock
		}
		usleep(500);
	}
	return (NULL); // from if monitor functions
}


bool someone_died(t_philos *philo)
{
	pthread_mutex_lock(&philo->args->status_lock);// i would call it activity lock
	if (philo->args->found_dead || philo->is_dead)
	{
		pthread_mutex_unlock(&philo->args->status_lock);// i would call it activity lock
		return true;
	}
	pthread_mutex_unlock(&philo->args->status_lock);// i would call it activity lock
	return false;
}



void *routine(void *arg)
{
	t_philos *philo = (t_philos *)arg;

	if(philo->id % 2 == 0)
		ft_usleep(1, philo); // delay to avoid even philos to be quicker than odd
			// usleep((philo->args->tte * 100) / philo->args->philos_n); // delay to avoid even philos to be quicker than odd

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
	pthread_t monitor_thread;
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
	if (pthread_create(&monitor_thread, NULL, &monitor, program) != 0)
	{
		printf("Error: failed to create monitor thread\n");
		return (false);
	}

	// Wait for philosophers to finish
	for (i = 0; i < program->args.philos_n; i++)
		pthread_join(program->philo[i].lifespan, NULL);

	// Wait for monitor to finish
	pthread_join(monitor_thread, NULL);
	// print_status(program);
	// print_mealcount(program); // debugger for meal count


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
		program->philo[i].last_meal_time = 0;
		program->philo[i].start_activity = 0;
		program->philo[i].is_eating = false;
		program->philo[i].is_sleeping = false;
		program->philo[i].is_thinking = false;
		program->philo[i].is_dead = false;
		// printf(SLEEP"	Philo %d: left_fork = fork[%ld], right_fork = fork[%ld]\n"RESET,
		// program->philo[i].id,
		// program->philo[i].left_fork - program->forks,
		// program->philo[i].right_fork - program->forks);
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
	pthread_mutex_init(&program->args.meal_lock, NULL); // alert tahet tells other philos to die if someone is dead
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
	if(program->args.meals_limit == 0)// should 0 mean no meals or no stop
		return (false);
	return (true);
}
