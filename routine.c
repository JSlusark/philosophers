/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/06 12:55:15 by jslusark         ###   ########.fr       */
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
	printf("%zu %d is thinking\n", get_curr_ms(philo->args.unix_start), philo->id);
	usleep(philo->args.ttt * 1000); // sleeps for time to think len
	// goes to eat
}
void	eats(t_philos *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	if(!philo->status.is_eating)
	{
		pthread_mutex_lock(first_fork); // right for even left for odd
		printf("%zu %d has taken a fork\n", get_curr_ms(philo->args.unix_start), philo->id);
		pthread_mutex_lock(second_fork); // left for even right for odd
		printf("%zu %d has taken a fork\n", get_curr_ms(philo->args.unix_start), philo->id);
		philo->status.is_eating = true;
	}
	if(philo->status.is_eating)
	{
		philo->meal_start = get_curr_ms(philo->args.unix_start);
		printf("%zu %d is eating\n", philo->meal_start, philo->id);
		usleep(philo->args.tte * 1000);
		philo->meal_end = get_curr_ms(philo->args.unix_start);
		// Release forks
		// printf("%zu %d put down a fork\n", get_curr_ms(philo->args.unix_start), philo->id);
		pthread_mutex_unlock(philo->right_fork);
		// printf("%zu %d has put down RIGHT fork i:%d\n", get_curr_ms(philo->args.unix_start), philo->id, philo->rf_id);
		// printf("%zu %d put down a fork\n", get_curr_ms(philo->args.unix_start), philo->id);
		pthread_mutex_unlock(philo->left_fork);
		philo->meals_n++; // increase the meal in case optional requirement given
		philo->status.is_eating = false;
	}
	if (philo->meal_end - philo->meal_start >= philo->args.ttd) // this has no sense
	{
		printf("%zu %d is dead 💀\n", philo->meal_end, philo->id);
		philo->status.is_alive = false;
		// should already break
	}
}
void	sleeps(t_philos *philo)
{
	printf("%zu %d is sleeping\n", get_curr_ms(philo->args.unix_start), philo->id);
	philo->status.is_sleeping = true;
	usleep(philo->args.tts * 1000); //to simulate time duration
	philo->status.is_sleeping = false;
	// goes to think
}

void	dies(t_philos *philo) // pilospher should avoid dying
{
	// if(meal_end - meal_start > ttd)
	printf("%zu %d is dead 💀\n", get_unix_timestamp() - philo->tob, philo->id);
	//message should be displayed no more than 10 ms after the actual death of the philosopher.
}

