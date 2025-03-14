/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:54:18 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/15 19:15:30 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void *monitor(void *arg)
{
    t_data *program = (t_data *)arg;
    int	i;

    while (true)
    {
        // Lock the mutex to check death condition
        pthread_mutex_lock(&program->args.dead_lock);
        if (program->args.found_dead)
        {
            pthread_mutex_unlock(&program->args.dead_lock);
            break;
        }
        pthread_mutex_unlock(&program->args.dead_lock);
        // // check if philosopher dies in other isntances
		for (i = 0; i < program->args.philos_n; i++)
		{
			pthread_mutex_lock(&program->args.status_lock);
			if ((get_curr_ms(program->philo[i].args->unix_start) - program->philo[i].last_meal_time) >= program->philo[i].args->ttd && !program->philo[i].status.is_eating )
			{
				// printf(DEATH"%zu %d died AHHHH - timer: %zu\n"RESET, get_curr_ms(program->philo[i].args->unix_start), program->philo[i].id, (get_curr_ms(program->philo[i].args->unix_start) - program->philo[i].last_meal_time));
				printf(DEATH"%zu %d died AHHHH - timer: %zu"RESET, get_curr_ms(program->philo[i].args->unix_start), program->philo[i].id, (get_curr_ms(program->philo[i].args->unix_start) - program->philo[i].last_meal_time));
				if (program->philo[i].status.is_eating)
					printf(GREEN" <---- eating"RESET);
				else if (program->philo[i].status.is_sleeping)
					printf(SLEEP" <---- sleeping"RESET);
				else if (program->philo[i].status.is_thinking)
					printf(THINK" <---- thinking"RESET);
				else
					printf(FORK1" <---- no status is active"RESET);
			printf("\n");
				program->args.found_dead = true;
				program->philo[i].status.is_dead = true;

				pthread_mutex_unlock(&program->args.status_lock);
				return NULL; // Stop monitor thread immediately
			}
			pthread_mutex_unlock(&program->args.status_lock);
		}

		// printf("ciao");
        // Check if all philosophers have eaten enough times
        // if (program->args.meals_limit > 0 && !program->args.found_dead)
        // {
        //     int meals_met = 0;
        //     for (i = 0; i < program->args.philos_n; i++)
        //     {
        //         if (program->philo[i].meals_n >= program->args.meals_limit)
        //         {
        //             meals_met++;
        //             pthread_mutex_lock(&program->args.dead_lock);
        //             program->philo[i].status.elapsed_time = get_curr_ms(program->philo[i].args->unix_start) - program->philo[i].last_meal_time;
        //             pthread_mutex_unlock(&program->args.dead_lock);
        //         }
        //     }
        //     if (meals_met == program->args.philos_n)
        //     {
        //         pthread_mutex_lock(&program->args.dead_lock);
        //         program->args.found_dead = true;
        //         pthread_mutex_unlock(&program->args.dead_lock);
        //         break;
        //     }
        // }
        usleep(500);
    }
    return (NULL);
}


bool someone_died(t_philos *philo)
{

	// if (!philo || !philo->args) // NULL check
	// 	return true;
    pthread_mutex_lock(&philo->args->alert_lock);
    // size_t current_time = get_curr_ms(philo->args->unix_start);
    if (philo->args->found_dead || philo->status.is_dead)
    {
		printf("--- philo %d says someone is dead!!\n", philo->id);
        pthread_mutex_unlock(&philo->args->alert_lock);
        return true;
    }
    pthread_mutex_unlock(&philo->args->alert_lock);
    return false;
}



void *routine(void *arg)
{
	t_philos *philo = (t_philos *)arg;

	// while (!philo->status.is_dead || !philo->args->found_dead)
	while (!someone_died(philo))
	{
		// Check if the simulation should stop
		// pthread_mutex_lock(&philo->args->dead_lock);
		// if (philo->args->found_dead)
		// {
		// 	pthread_mutex_unlock(&philo->args->dead_lock);
		// 	break;
		// }
		// pthread_mutex_unlock(&philo->args->dead_lock);

		// Check if this philosopher should die
		// if (check_death(philo))
		// 	break;
		if(philo->id % 2 == 0)
		{
			// usleep(500);
			eats(philo, philo->left_fork, philo->right_fork);
		}
		else
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
		program->philo[i].left_fork = &program->forks[i];
		program->philo[i].right_fork = &program->forks[(i + 1) % program->args.philos_n];
		program->philo[i].status.elapsed_time = 0;
		program->philo[i].status.is_eating = false;
		program->philo[i].status.is_sleeping = false;
		program->philo[i].status.is_thinking = false;
		program->philo[i].status.is_dead = false;
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

