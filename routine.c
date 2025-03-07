/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/07 15:56:51 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
/*
	order of philo actions:
	1. philo starts thinking
	2. philo starts eating for tte
		- philo picks up left fork
		- philo picks up right fork
		- time to eat has ended
		- philo puts down right fork
		- philo puts down left fork
	3. philo starts sleeping for tts
	4. philo starts thinking for ttt
	9. repeat from 2

	Different appraoches if philo_n is even or odd:

		- If philo_n is even:
			~ Each philo will pick up the left fork first and then the right fork.
			~ The last philo will pick up the right fork first and then the left fork.
			~ This will prevent the deadlock condition.
		- If philo_n is odd:
			~ Each philo will pick up the right fork first and then the left fork.
			~ The last philo will pick up the left fork first and then the right fork.
			~ This will prevent the deadlock condition.
 */

void	thinks(t_philos *philo)
{
	pthread_mutex_lock(&philo->args->think_lock);
	printf(THINK"%zu %d is thinking\n" RESET, get_curr_ms(philo->args->unix_start), philo->id);
	// usleep(philo->args->ttt * 1000); // sleeps for time to think len
	philo->stop_timer = get_curr_ms(philo->args->unix_start);
	if (philo->stop_timer >= philo->args->ttd)
	{
		printf(DEATH"%zu %d died IN THINKS\n"RESET, philo->stop_timer, philo->id);
		philo->status.is_alive = false;
		philo->args->found_dead = true;
		pthread_mutex_unlock(&philo->args->think_lock);
	}
	pthread_mutex_unlock(&philo->args->think_lock);
	// goes to eat
}
void	eats(t_philos *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	if(!philo->status.is_eating && philo->args->found_dead == false)
	{
		// printf("EATS found dead %d\n", philo->args->found_dead);
		pthread_mutex_lock(first_fork); // right for even left for odd
		if(!philo->status.is_eating && philo->args->found_dead == false)
			printf(FORK1"%zu %d has taken a fork\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
		pthread_mutex_lock(second_fork); // left for even right for odd
		if(!philo->status.is_eating && philo->args->found_dead == false)
			printf(FORK2"%zu %d has taken a fork\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
		philo->meal_start = get_curr_ms(philo->args->unix_start);
		printf("%zu %d is eating\n", get_curr_ms(philo->args->unix_start), philo->id);
		usleep(philo->args->tte * 1000);
		printf("%zu %d finished eating\n", get_curr_ms(philo->args->unix_start), philo->id);
		// philo->meal_end = get_curr_ms(philo->args->unix_start);
		philo->stop_timer = get_curr_ms(philo->args->unix_start);
		philo->meals_n++; // increase the meal in case optional requirement given
		philo->status.is_eating = false;
		printf(FORK1"%zu %d put down 1st a fork\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
		pthread_mutex_unlock(first_fork);
		printf(FORK2"%zu %d put down 2nd a fork\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
		pthread_mutex_unlock(second_fork);
		if ((philo->stop_timer - philo->meal_start )>= philo->args->ttd)
		{
			printf(DEATH"%zu %d died IN EATS\n"RESET, philo->stop_timer, philo->id);
			philo->status.is_alive = false;
			philo->args->found_dead = true;
		}
		else
			philo->stop_timer = 0; // resta
	}
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}
void	sleeps(t_philos *philo)
{
	pthread_mutex_lock(&philo->args->sleep_lock);
	printf(SLEEP"%zu %d is sleeping\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
	philo->status.is_sleeping = true;
	usleep(philo->args->tts * 1000); //to simulate time duration	philo->stop_timer = get_curr_ms(philo->args->unix_start);
	philo->stop_timer = get_curr_ms(philo->args->unix_start);
	philo->status.is_sleeping = false;
	if (philo->stop_timer >= philo->args->ttd)
	{
		printf(DEATH"%zu %d died IN SLEEPS\n"RESET, philo->stop_timer, philo->id);
		philo->status.is_alive = false;
		philo->args->found_dead = true;
		pthread_mutex_unlock(&philo->args->sleep_lock);
	}
	pthread_mutex_unlock(&philo->args->sleep_lock);

	// goes to think
}