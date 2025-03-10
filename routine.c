/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/10 17:17:01 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

 void thinks(t_philos *philo)
 {
	 pthread_mutex_lock(&philo->args->output_lock);
	 printf(THINK"%zu %d is thinking\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
	 pthread_mutex_unlock(&philo->args->output_lock);
 }

 void sleeps(t_philos *philo)
 {
	 pthread_mutex_lock(&philo->args->output_lock);
	 printf(SLEEP"%zu %d is sleeping\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
	 pthread_mutex_unlock(&philo->args->output_lock);
	 ft_usleep(philo->args->tts, philo);
 }

 void eats(t_philos *philo)
 {
	 pthread_mutex_lock(philo->left_fork);
	 pthread_mutex_lock(philo->right_fork);

	 pthread_mutex_lock(&philo->args->output_lock);
	 printf("%zu %d is eating\n", get_curr_ms(philo->args->unix_start), philo->id);
	 pthread_mutex_unlock(&philo->args->output_lock);

	 philo->lastmeal_time = get_curr_ms(philo->args->unix_start);
	 ft_usleep(philo->args->tte, philo);
	 philo->meals_n++;

	 pthread_mutex_unlock(philo->left_fork);
	 pthread_mutex_unlock(philo->right_fork);
 }

