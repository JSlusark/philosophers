/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/08 15:41:02 by jslusark         ###   ########.fr       */
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
	philo->curr_t = get_unix_timestamp() - philo->tob;
	philo->status.is_eating = 0;
	// philo->status.is_sleeping = 0;
	printf("💭 %zu philo[%d] is thinking\n", philo->curr_t, philo->id);
	// usleep(500000); //to simulate time duration
	// goes to eat
}
void	eats(t_philos *philo)
{
	//takes left fork - lock mutex
	//takes right fork - lock mutex
	philo->meal_start = get_unix_timestamp() - philo->tob;
	philo->status.is_eating = 1;
	philo->status.is_sleeping = 0;
	printf("🍔 %zu philo[%d] started eating\n", philo->meal_start, philo->id);
	// philo->meals_n++;
	// usleep(philo->args.tte * 1000); //to simulate time duration
	// philo->meal_end = get_unix_timestamp() - philo->tob;
	// printf("🍽️ %zu philo[%d] ended eating\n", philo->meal_end, philo->id);
	// goes to sleep
}
void	sleeps(t_philos *philo)
{
	//put down left fork - release mutex
	//put down right fork - release mutex
	philo->curr_t = get_unix_timestamp() - philo->tob;
	philo->status.is_thinking = 0;
	// philo->status.is_eating = 0;
	printf("🌙 %zu philo[%d] is sleeping\n", philo->curr_t, philo->id);
	// usleep(philo->args.tts); //to simulate time duration
	// goes to think
}

void	dies(t_philos *philo) // pilospher should avoid dying
{
	// if(meal_end - meal_start > ttd)
	printf("%zu philo[%d] is dead 💀\n", get_unix_timestamp() - philo->tob, philo->id);
	//message should be displayed no more than 10 ms after the actual death of the philosopher.
}

