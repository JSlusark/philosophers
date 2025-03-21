/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:11:49 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/21 11:50:48 by jslusark         ###   ########.fr       */
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
		printf(DEATH"%zu %d died"RESET,
			get_curr_ms(philo->args->unix_start), philo->id);
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
