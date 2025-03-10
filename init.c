/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:54:18 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/10 12:44:31 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	dead_loop(t_philos *philo)
{
	pthread_mutex_lock(&philo->args->dead_lock);
	if ((philo->activity_end - philo->activity_start) >= philo->args->ttd)
	{
		printf(DEATH"PRINT DEATH!!!!!\n"RESET);
		return (pthread_mutex_unlock(&philo->args->dead_lock), 1);
	}
	pthread_mutex_unlock(&philo->args->dead_lock);
	return (0);
}
void *routine(void *arg)
{
	t_philos *philo = (t_philos *)arg;
	philo->tob = get_curr_ms(philo->args->unix_start);
	// philo->stop_timer = 0;
	philo->activity_start = get_curr_ms(philo->args->unix_start);
	// printf(GREEN"philo %d born at %zu\n"RESET, philo->id, philo->tob);
	// printf(YELLOW"philo %d current time %zu\n"RESET, philo->id, philo->stop_timer);

	// while (check_time(philo)) // Infinite loop until death
	// while (!dead_loop(philo)) // Infinite loop until death
	while (philo->args->found_dead == false) // Infinite loop until death
	{
		philo->activity_start = get_curr_ms(philo->args->unix_start); // philo sleeps pnly when full therefore we restart the cycle
		// printf("------------> 1. philo %d is dead? %i flag found dead %i\n",philo->id, philo->args->found_dead, philo->args->found_dead);
		if (philo->id % 2 == 0)
		{
			// usleep(500);
			eats(philo, philo->right_fork, philo->left_fork); // even takes right first and left second
		}
		else
			eats(philo, philo->left_fork, philo->right_fork); // odd takes left first and right second
		// printf("------------> 2. philo %d is dead? %i flag found dead %i\n",philo->id, philo->args->found_dead, philo->args->found_dead);
		if(philo->args->found_dead == false && !philo->status.is_eating)
			sleeps(philo);
		if(philo->args->found_dead == false && !philo->status.is_eating)
			thinks(philo);
		// printf("------------> 3. philo %d is dead? %i flag found dead %i\n",philo->id, philo->args->found_dead, philo->args->found_dead);
		if(philo->args->found_dead == true)
			break;
		// else
		// 	break;
	}
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	// printf("philo end\n");
	return NULL;
}

bool	start_simulation(t_data *program, t_philos *philo)
{
	// printf("\n - philos_n: %d\n - ttd: %d\n - tte: %d\n - tts: %d\n - meals_limit: %d\n\n", program->args.philos_n, program->args.ttd, program->args.tte, program->args.tts, program->args.meals_limit);
	int i;

	i = 0;
	(void)philo;
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
	i = 0; // as it's an array we loop from 0 to philos_n - 1 (we then print +1 to show the actual n)
	while(i < program->args.philos_n)
	{
		program->philo[i].id = i + 1;
		program->philo[i].args = &program->args;
		program->philo[i].meals_n = 0;
		program->philo[i].left_fork = &program->forks[i]; // left fork for philo_n 0 (1) it's fork 0 (1)
		if (i == 0)// if total philos if 50, the r_f of philo 1 (0 i) is the l_f of philo 50 (49 i so philo_n - 1)
				program->philo[i].right_fork = &program->forks[program->args.philos_n - 1 ]; // right fork is the fork of last philosopher
		else
				program->philo[i].right_fork = &program->forks[i - 1]; // right fork is the fork of previous philosopher
		program->philo[i].status.is_alive = true;
		program->philo[i].status.is_sleeping = false;
		program->philo[i].status.is_eating = false;
		program->philo[i].status.is_thinking = false;
		program->philo[i].status.ate = false;
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

bool	init_data(int argc, char **argv, t_data *program, t_rules *args)
{
	// program = malloc(sizeof(t_data));
	args->philos_n = ft_atoi(argv[1]); // i have to hanldle letters? should not give 0??
	args->ttd = ft_atoi(argv[2]); // check if more convenient to convert everything into microseconds and then use usleep with t/1000 to show milliseconds
	args->tte = ft_atoi(argv[3]);
	args->tts = ft_atoi(argv[4]);
	args->found_dead = false;
	pthread_mutex_init(&args->dead_lock, NULL); // <-- Initialize mutex
	pthread_mutex_init(&args->sleep_lock, NULL); // <-- Initialize mutex
	pthread_mutex_init(&args->think_lock, NULL); // <-- Initialize mutex
	args->unix_start = get_unix_timestamp(); // ms since 1970 to start of program, does not need conversion (it's in milliseconds)
	if (argc == 6)
	{
		args->meals_limit = ft_atoi(argv[5]);
		if(args->meals_limit < 0)
		{
			printf("Error: the 5th value(meals_limit) should not be negative\n");// check if condition needed for meals_limit
			return (false);
		}
	}
	else
		args->meals_limit = -1; // no limit should be given if not inputed from the user
	if(!check_values(args))
		return (false);
	if(!init_forks(program))
		return (false);
	if(!init_philos(program))
		return (false);
	return (true);
}
