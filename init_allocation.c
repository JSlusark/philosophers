/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_allocation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 11:54:18 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/21 13:02:04 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	cleanup(t_data *program)
{
	int	i;

	i = 0;
	while (i < program->args.philos_n)
	{
		pthread_mutex_destroy(&program->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&program->args.dead_lock);
	pthread_mutex_destroy(&program->args.status_lock);
	pthread_mutex_destroy(&program->args.meal_lock);
	pthread_mutex_destroy(&program->args.output_lock);
	if (program->forks)
		free(program->forks);
	if (program->philo)
		free(program->philo);
	free(program);
}

void	init_status(t_data *program, int *i)
{
	program->philo[*i].id = (*i) + 1;
	program->philo[*i].args = &program->args;
	program->philo[*i].meals_n = 0;
	program->philo[*i].left_fork = &program->forks[*i];
	if (*i == 0)
		program->philo[*i].right_fork = &program->forks
		[program->args.philos_n - 1];
	else
		program->philo[*i].right_fork = &program->forks[(*i) - 1];
	program->philo[*i].elapsed_time = 0;
	program->philo[*i].last_meal_time = 0;
	program->philo[*i].start_activity = 0;
	program->philo[*i].is_eating = false;
	program->philo[*i].is_sleeping = false;
	program->philo[*i].is_thinking = false;
	program->philo[*i].is_dead = false;
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
	while (i < program->args.philos_n)
	{
		init_status(program, &i);
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
