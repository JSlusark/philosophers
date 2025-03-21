/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/21 10:55:40 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

bool	interrupt_activity(t_philos *philo)
{
	if (philo->args->found_dead)
	{
		pthread_mutex_unlock(&philo->args->status_lock);
		pthread_mutex_unlock(&philo->args->output_lock);
		return (true);
	}
	if (philo->elapsed_time >= philo->args->ttd && !philo->is_eating)
	{
		philo->is_dead = true;
		philo->is_dead = true;
		philo->args->found_dead = true;
		pthread_mutex_unlock(&philo->args->status_lock);
		pthread_mutex_unlock(&philo->args->output_lock);
		return (true);
	}
	return (false);
}

void	print_activity(t_philos *philo, size_t time,
		char *message, size_t delay)
{
	pthread_mutex_lock(&philo->args->output_lock);
	philo->elapsed_time = get_curr_ms(philo->args->unix_start)
		- philo->last_meal_time;
	pthread_mutex_lock(&philo->args->status_lock);
	if (interrupt_activity(philo))
		return ;
	pthread_mutex_unlock(&philo->args->status_lock);
	printf("%zu %d %s", time, philo->id, message);
	routine_debugging(philo);
	pthread_mutex_unlock(&philo->args->output_lock);
	if (delay >= 1)
		usleep(delay * 1000);
	else
		usleep(500);
}

void	thinks(t_philos *philo)
{
	pthread_mutex_lock(&philo->args->status_lock);
	philo->is_eating = false;
	philo->is_thinking = true;
	philo->is_sleeping = false;
	pthread_mutex_unlock(&philo->args->status_lock);
	print_activity(philo, get_curr_ms(philo->args->unix_start),
		THINK"is thinking     "RESET, 0);
}

void	sleeps(t_philos *philo)
{
	pthread_mutex_lock(&philo->args->status_lock);
	philo->is_eating = false;
	philo->is_thinking = false;
	philo->is_sleeping = true;
	pthread_mutex_unlock(&philo->args->status_lock);
	print_activity(philo, get_curr_ms(philo->args->unix_start),
		SLEEP"is sleeping"RESET, philo->args->tts);
}

void	eats(t_philos *philo,
	pthread_mutex_t *first_fork,
	pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(first_fork);
	print_activity(philo, get_curr_ms(philo->args->unix_start),
		FORK2"has taken a fork"RESET, 0);
	if (lonely_philo(philo, first_fork))
		return ;
	pthread_mutex_lock(second_fork);
	print_activity(philo, get_curr_ms(philo->args->unix_start),
		FORK2"has taken a fork"RESET, 0);
	pthread_mutex_lock(&philo->args->status_lock);
	philo->last_meal_time = get_curr_ms(philo->args->unix_start);
	philo->is_eating = true;
	philo->is_thinking = false;
	philo->is_sleeping = false;
	pthread_mutex_lock(&philo->args->meal_lock);
	philo->meals_n++;
	pthread_mutex_unlock(&philo->args->meal_lock);
	pthread_mutex_unlock(&philo->args->status_lock);
	print_activity(philo, philo->last_meal_time, "is eating  ",
		philo->args->tte);
	usleep(500);
	pthread_mutex_unlock(first_fork);
	usleep(500);
	pthread_mutex_unlock(second_fork);
}
