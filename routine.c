/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/03 17:26:51 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
/*
	order of philo actions:
	1. philo starts thinking
	2. philo starts eating for time_to_eat
		- philo picks up left fork
		- philo picks up right fork
		- time to eat has ended
		- philo puts down right fork
		- philo puts down left fork
	3. philo starts sleeping for time_to_sleep
	4. philo starts thinking for time_to_think
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

void	thinks()
{
	printf("timestamp_in_ms ID is thinking\n");
	//usleep(time_to_think * 1000) to simulate time duration
	// goes to eat
}
void	eats()
{
	//takes left fork - lock mutex
	//takes right fork - lock mutex
	printf("timestamp_in_ms ID is eating\n");
	//usleep(time_to_eat * 1000) to simulate time duration
	// has_eaten++
	//ended_eating = get_current_time();
	// goes to sleep
}
void	sleeps()
{
	//put down left fork - release mutex
	//put down right fork - release mutex
	printf("timestamp_in_ms ID is sleeping\n");
	//usleep(time_to_sleep * 1000) to simulate time duration
	// goes to think
}

void	dies() // pilospher should avoid dying
{
	// if(ended_eating - started_eating > time_to_die)
	printf("timestamp_in_ms ID is dead\n");
	//message should be displayed no more than 10 ms after the actual death of the philosopher.
}