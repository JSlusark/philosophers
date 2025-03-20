/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:54:18 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/20 20:13:37 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

bool	meal_check(t_philos *philo, int *group_ate_enough)
{
	pthread_mutex_lock(&philo->args->meal_lock);
	if (philo->args->meals_limit != -1)
	{
		if (philo->meals_n >= philo->args->meals_limit)
			(*group_ate_enough)++;
		pthread_mutex_unlock(&philo->args->meal_lock);
		if (*group_ate_enough == philo->args->philos_n)
		{
			pthread_mutex_lock(&philo->args->status_lock);
			philo->args->found_dead = true;
			pthread_mutex_unlock(&philo->args->status_lock);
			return (true);
		}
	}
	else
		pthread_mutex_unlock(&philo->args->meal_lock);
	return (false);
}

bool	found_death(t_philos *philo)
{
	pthread_mutex_lock(&philo->args->output_lock);
	pthread_mutex_lock(&philo->args->status_lock);
	philo->elapsed_time = get_curr_ms(philo->args->unix_start)
		- philo->last_meal_time;
	pthread_mutex_unlock(&philo->args->status_lock);
	if (philo->elapsed_time >= philo->args->ttd && !philo->is_eating)
	{
		printf(DEATH"%zu %d is dead"RESET, get_curr_ms(philo->args->unix_start), philo->id);
		routine_debugging(philo);
		pthread_mutex_lock(&philo->args->status_lock);
		philo->is_dead = true;
		philo->args->found_dead = true;
		pthread_mutex_unlock(&philo->args->status_lock);
		pthread_mutex_unlock(&philo->args->output_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->args->output_lock);
	return (false);
}

void	*monitor(void *arg)
{
	t_data	*program;
	int		group_ate_enough;
	int		i;

	program = (t_data *)arg;
	while (1)
	{
		i = 0;
		group_ate_enough = 0;
		while (i < program->args.philos_n)
		{
			if (meal_check(&program->philo[i], &group_ate_enough))
				return (NULL);
			if (found_death(&program->philo[i]))
				return (NULL);
			i++;
		}
		usleep(500);
	}
	return (NULL);
}

bool	death_alert(t_philos *philo)
{
	pthread_mutex_lock(&philo->args->status_lock);
	if (philo->args->found_dead || philo->is_dead)
	{
		pthread_mutex_unlock(&philo->args->status_lock);
		return (true);
	}
	pthread_mutex_unlock(&philo->args->status_lock);
	return (false);
}

void	*routine(void *arg)
{
	t_philos	*philo;

	philo = (t_philos *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(1, philo);
	while (!death_alert(philo))
	{
		if (philo->id % 2 != 0)
			eats(philo, philo->left_fork, philo->right_fork);
		else
			eats(philo, philo->right_fork, philo->left_fork);
		sleeps(philo);
		thinks(philo);
	}
	return (NULL);
}

bool	start_simulation(t_data *program)
{
	pthread_t	monitor_thread;
	int			i;

	i = 0;
	while (i < program->args.philos_n)
	{
		if (pthread_create(&program->philo[i].lifespan, NULL,
				&routine, &program->philo[i]) != 0)
		{
			printf("Error: failed to create philosopher thread\n");
			return (false);
		}
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, &monitor, program) != 0)
	{
		printf("Error: failed to create monitor thread\n");
		return (false);
	}
	while (i < program->args.philos_n)
	{
		pthread_join(program->philo[i].lifespan, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	print_status(program);
	print_mealcount(program);
	return (true);
}

bool	init_philos(t_data *program)
{
	int	i;

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
		if (i == 0)
			program->philo[i].right_fork = &program->forks
			[program->args.philos_n - 1];
		else
			program->philo[i].right_fork = &program->forks[i - 1];
		program->philo[i].elapsed_time = 0;
		program->philo[i].last_meal_time = 0;
		program->philo[i].start_activity = 0;
		program->philo[i].is_eating = false;
		program->philo[i].is_sleeping = false;
		program->philo[i].is_thinking = false;
		program->philo[i].is_dead = false;
		i++;
	}
	return (true);
}

bool	init_forks(t_data *program)
{
	int	i;

	program->forks = malloc(sizeof(pthread_mutex_t) * program->args.philos_n);
	if (!program->forks)
	{
		printf("Error: malloc of t_data program->forks failed\n");
		return (false);
	}
	i = 0;
	while (i < program->args.philos_n)
	{
		pthread_mutex_init(&program->forks[i], NULL);
		i++;
	}
	return (true);
}

bool	check_values(t_rules *args)
{
	if (args->philos_n < 1 || args->ttd < 1 || args->tte < 1 || args->tts < 1)
	{
		printf("Error: the first 4 values should be greater than 0\n");
		return (false);
	}
	if (args->philos_n > 200)
	{
		printf("Error: not allowed to test philos_n greater than 200\n");
		return (false);
	}
	// what about intmax limit for args???
	return (true);
}

bool	init_data(int argc, char **argv, t_data *program)
{
	program->args.philos_n = ft_atoi(argv[1]);
	program->args.ttd = ft_atoi(argv[2]);
	program->args.tte = ft_atoi(argv[3]);
	program->args.tts = ft_atoi(argv[4]);
	program->args.found_dead = false;
	pthread_mutex_init(&program->args.dead_lock, NULL);
	pthread_mutex_init(&program->args.status_lock, NULL);
	pthread_mutex_init(&program->args.meal_lock, NULL);
	pthread_mutex_init(&program->args.output_lock, NULL);
	program->args.unix_start = get_unix_timestamp();
	if (argc == 6)
		program->args.meals_limit = ft_atoi(argv[5]);
	else
		program->args.meals_limit = -1;
	if (!check_values(&program->args))
		return (false);
	if (!init_forks(program))
		return (false);
	if (!init_philos(program))
		return (false);
	if (program->args.meals_limit == 0)
		return (false);
	return (true);
}
