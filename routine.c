/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/13 15:43:50 by jslusark         ###   ########.fr       */
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

	if ((get_curr_ms(philo->args->unix_start) - philo->last_meal_time) >= philo->args->ttd) // actually  last meal should be meal staered
	{
		pthread_mutex_lock(&philo->args->output_lock);
		philo->args->found_dead = true;
		printf(DEATH"%zu %d died IUUUUUUJHHH\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
		pthread_mutex_unlock(&philo->args->output_lock);
		// pthread_mutex_unlock(&philo->args->dead_lock);
		// return true;
	}
	pthread_mutex_lock(&philo->status_lock);
	philo->status.is_sleeping = false;
	philo->status.is_eating = false;
	philo->status.is_thinking = true;
	pthread_mutex_unlock(&philo->status_lock);
	printf(THINK"%zu %d is thinking - timer:%zu\n"RESET, get_curr_ms(philo->args->unix_start), philo->id, (get_curr_ms(philo->args->unix_start) - philo->last_meal_time));
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
	//  if (get_curr_ms(philo->args->unix_start) >= philo->args->ttd) // actually  last meal should be meal staered
	//  {
	// 	 printf("wiwiwiwi 3\n");
	// 	 philo->args->found_dead = true;
	// 	//  pthread_mutex_lock(&philo->args->output_lock);
	// 	 printf(DEATH"%zu %d died YUUUU\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
	// 	 philo->status.is_dead = true;
	// 	 philo->status.timer_stopped = get_curr_ms(philo->args->unix_start) - philo->last_meal_time;
	// 	 pthread_mutex_unlock(&philo->args->output_lock);
	// 	 pthread_mutex_unlock(&philo->args->dead_lock);
	// 	 return;
	//  }
	pthread_mutex_lock(&philo->status_lock);
	philo->status.is_eating = false;
	philo->status.is_thinking = false;
	philo->status.is_sleeping = true;
	pthread_mutex_unlock(&philo->status_lock);
	printf(SLEEP"%zu %d is sleeping - timer: %zu\n"RESET, get_curr_ms(philo->args->unix_start), philo->id, (get_curr_ms(philo->args->unix_start) - philo->last_meal_time));
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

	printf(FORK1"%zu %d has taken a fork - timer %zu\n"RESET, get_curr_ms(philo->args->unix_start), philo->id, (get_curr_ms(philo->args->unix_start) - philo->last_meal_time));
	printf(FORK2"%zu %d has taken a fork - timer %zu\n"RESET, get_curr_ms(philo->args->unix_start), philo->id, (get_curr_ms(philo->args->unix_start) - philo->last_meal_time));

	printf("%zu %d is eating - timer: %zu\n", get_curr_ms(philo->args->unix_start), philo->id, (get_curr_ms(philo->args->unix_start) - philo->last_meal_time));
	pthread_mutex_lock(&philo->status_lock);
	philo->status.is_sleeping = false;
	philo->status.is_thinking = false;
	philo->status.is_eating = true;
	pthread_mutex_unlock(&philo->status_lock);
	pthread_mutex_unlock(&philo->args->output_lock);

	philo->last_meal_time = get_curr_ms(philo->args->unix_start); // Update last meal time
	ft_usleep(philo->args->tte, philo);
	philo->meals_n++;

	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
 }

