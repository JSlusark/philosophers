/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/12 17:30:28 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

 void thinks(t_philos *philo)
 {
	 pthread_mutex_lock(&philo->args->output_lock);
	if(philo->args->found_dead)
	{
		pthread_mutex_unlock(&philo->args->output_lock);
		return ;
	}
	//
	// if ((get_curr_ms(philo->args->unix_start) - philo->last_meal_time) >= philo->args->ttd) // actually  last meal should be meal staered
	// {
	// 	pthread_mutex_lock(&philo->args->output_lock);
	// 	philo->args->found_dead = true;
	// 	printf(DEATH"%zu %d died AHH\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
	// 	pthread_mutex_unlock(&philo->args->output_lock);
	// 	// pthread_mutex_unlock(&philo->args->dead_lock);
	// 	// return true;
	// }
	philo->status.is_sleeping = false;
	philo->status.is_eating = false;
	philo->status.is_thinking = true;
	printf(THINK"%zu %d is thinking\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
	pthread_mutex_unlock(&philo->args->output_lock);
 }

 void sleeps(t_philos *philo)
 {
	 pthread_mutex_lock(&philo->args->output_lock);
	 if(philo->args->found_dead)
	 {
		 pthread_mutex_unlock(&philo->args->output_lock);
		 return ;
	 }
	philo->status.is_eating = false;
	philo->status.is_thinking = false;
	philo->status.is_sleeping = true;
	printf(SLEEP"%zu %d is sleeping\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
	pthread_mutex_unlock(&philo->args->output_lock);
	ft_usleep(philo->args->tts, philo);
 }

 void eats(t_philos *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
 {
	 pthread_mutex_lock(first_fork);
	 pthread_mutex_lock(second_fork);
	 pthread_mutex_lock(&philo->args->output_lock);
	 if (philo->args->found_dead)
	 {
		 pthread_mutex_unlock(first_fork);
		 pthread_mutex_unlock(second_fork);
		 pthread_mutex_unlock(&philo->args->output_lock);
		 return;
	 }
	//  pthread_mutex_lock(&philo->args->dead_lock);
	//  pthread_mutex_unlock(&philo->args->dead_lock);

	 printf(FORK1"%zu %d has taken a fork\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
	 printf(FORK2"%zu %d has taken a fork\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);

	 printf("%zu %d is eating\n", get_curr_ms(philo->args->unix_start), philo->id);
	 philo->status.is_sleeping = false;
	 philo->status.is_thinking = false;
	 philo->status.is_eating = true;
	 pthread_mutex_unlock(&philo->args->output_lock);

	 philo->last_meal_time = get_curr_ms(philo->args->unix_start); // Update last meal time
	 ft_usleep(philo->args->tte, philo);
	 philo->meals_n++;

	 pthread_mutex_unlock(first_fork);
	 pthread_mutex_unlock(second_fork);
 }

