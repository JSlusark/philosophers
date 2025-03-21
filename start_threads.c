/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:54:46 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/21 14:40:20 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

bool	lonely_philo(t_philos *philo, pthread_mutex_t *first_fork)
{
	if (philo->args->philos_n == 1)
	{
		usleep(philo->args->ttd * 1000);
		pthread_mutex_unlock(first_fork);
		return (true);
	}
	return (false);
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
		usleep(philo->args->tte * 100 / philo->args->philos_n);
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

bool	create_lifespan_threads(t_data *program)
{
	int			i;

	i = 0;
	while (i < program->args.philos_n)
	{
		if (pthread_create(&program->philo[i].lifespan, NULL,
				&routine, &program->philo[i]) != 0)
		{
			printf("Error: failed to create philosopher thread\n");
			while (--i >= 0)
				pthread_join(program->philo[i].lifespan, NULL);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	start_simulation(t_data *program)
{
	pthread_t	monitor_thread;
	int			i;

	i = 0;
	if (!create_lifespan_threads(program))
		return (false);
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
	return (true);
}
