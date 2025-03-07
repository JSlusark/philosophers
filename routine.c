/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/07 18:41:28 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

// void	ft_usleep(size_t milliseconds, t_philos *philo)
// {
// 	size_t	start_time;

// 	start_time = get_curr_ms(philo->args->unix_start);
// 	while ((get_curr_ms(philo->args->unix_start) - start_time) < milliseconds)
// 	{
// 		// Check for death AFTER sleep
// 		// if ((get_curr_ms(philo->args->unix_start) - philo->activity_start) >= philo->args->ttd)
// 		// {
// 		// 	printf(DEATH"%zu %d died AFTER SLEEP\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
// 		// 	philo->status.is_alive = false;
// 		// 	philo->args->found_dead = true;
// 		// 	return;
// 		// }
// 		usleep(500); // Small sleep to avoid CPU overuse
// 	}
// }

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
	// pthread_mutex_lock(&philo->args->think_lock);
	// if(philo->status.ate)
		// philo->activity_start = get_curr_ms(philo->args->unix_start);
	printf(THINK"%zu %d is thinking - START %zu\n" RESET, get_curr_ms(philo->args->unix_start), philo->id, philo->activity_start);
	philo->activity_end = get_curr_ms(philo->args->unix_start);
	if ((philo->activity_end - philo->activity_start) >= philo->args->ttd)
	{
		printf(DEATH"%zu %d died AFETR THINKS\n"RESET, philo->activity_end, philo->id);
		philo->status.is_alive = false;
		philo->args->found_dead = true;
		pthread_mutex_unlock(&philo->args->think_lock);
	}
	// pthread_mutex_unlock(&philo->args->think_lock);

}
void	eats(t_philos *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	if (!philo->status.is_eating && philo->args->found_dead == false)
	{
		pthread_mutex_lock(first_fork);
		philo->status.is_eating = true;
		printf(FORK1"%zu %d has taken a fork\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
		pthread_mutex_lock(second_fork); // left for even right for odd
		// if(!philo->status.is_eating && philo->args->found_dead == false)
		printf(FORK2"%zu %d has taken a fork\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);

		// THIS SHOULD BE FILLED IF PHILO WAS PREVIOUSLY FED??? <<<<<<<<<<---------------------------- IDK anymore :(((())))
		// if(!philo->status.ate)
		// 	philo->activity_start = get_curr_ms(philo->args->unix_start);
		if(philo->activity_start != get_curr_ms(philo->args->unix_start))
			philo->activity_start = get_curr_ms(philo->args->unix_start);
		printf("%zu %d is eating\n", philo->activity_start, philo->id);
		usleep(philo->args->tte * 1000);
		philo->activity_end = get_curr_ms(philo->args->unix_start);
		printf("%zu %d finished eating START: %zu\n", philo->activity_end, philo->id, philo->activity_start);
		philo->status.is_eating = false;
		// printf(GREEN"%d EAT DIFF:%zu \n"RESET, philo->id, philo->activity_end - philo->activity_start);
	}
	// pthread_mutex_lock(&philo->args->dead_lock);
	if ((philo->activity_end - philo->activity_start) >= philo->args->ttd)
	{
		printf(DEATH"%zu %d died AFTER EATING\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
		philo->status.is_alive = false;
		philo->args->found_dead = true;
		pthread_mutex_unlock(first_fork);
		pthread_mutex_unlock(second_fork);
	}
	else
	{
		printf(GREEN"%zu %d SURVIVED EAT DIFF:%zu\n"RESET, get_curr_ms(philo->args->unix_start), philo->id, philo->activity_end - philo->activity_start);
		philo->activity_start = get_curr_ms(philo->args->unix_start);
		philo->status.ate = true;
	}
	// pthread_mutex_unlock(&philo->args->dead_lock);
	// // Check if the philosopher dies after eating
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}

void	sleeps(t_philos *philo)
{
	pthread_mutex_lock(&philo->args->sleep_lock);
	printf(SLEEP"%zu %d is sleeping START %zu\n"RESET, get_curr_ms(philo->args->unix_start), philo->id, philo->activity_start);

	// Use the new `usleep()` to check for death
	usleep(philo->args->tts * 1000);
	philo->activity_end = get_curr_ms(philo->args->unix_start);
	if ((philo->activity_end - philo->activity_start) >= philo->args->ttd)
	{
		printf(DEATH"%zu %d died AFTER SLEEP\n"RESET, get_curr_ms(philo->args->unix_start), philo->id);
		philo->status.is_alive = false;
		philo->args->found_dead = true;
	}
	philo->status.ate = false;
	pthread_mutex_unlock(&philo->args->sleep_lock);
}
