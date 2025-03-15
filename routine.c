/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/15 19:17:05 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
// philosophers race only for shared resources/data:
// forks
// prints
// sleeps

// they do not compete for statuses as each ha their own data which cannot be acceessed from them


void	print_activity(t_philos *philo, size_t time, char *message)
{
	pthread_mutex_lock(&philo->args->output_lock);// i would call it activity lock
	philo->status.elapsed_time = get_curr_ms(philo->args->unix_start) - philo->last_meal_time;
	printf("%zu %d %s", time, philo->id, message);
	if (philo->status.is_eating)
    	printf(GREEN" <---- eating"RESET);
	else if (philo->status.is_sleeping)
		printf(SLEEP" <---- sleeping"RESET);
	else if (philo->status.is_thinking)
		printf(THINK" <---- thinking"RESET);
	else
		printf(FORK1" <---- no status is active"RESET);
	// elapsed time to check death
	printf(" - timer %s%zums%s",
		philo->status.elapsed_time == 0 ? GREEN :
		(philo->status.elapsed_time >= philo->args->ttd ? DEATH : RESET), // Set color
		philo->status.elapsed_time, // Value
		RESET); // Reset color
	printf("\n");
	pthread_mutex_unlock(&philo->args->output_lock);
}


bool starvation(t_philos * philo, pthread_mutex_t *active_mutex)
{
	philo->status.elapsed_time = get_curr_ms(philo->args->unix_start) - philo->last_meal_time;
	if( philo->status.elapsed_time >= philo->args->ttd)
	{
		philo->status.is_dead = true;
		pthread_mutex_lock(&philo->args->output_lock);
		printf(DEATH"%zu %d IS DEAD!!!!"RESET, get_curr_ms(philo->args->unix_start), philo->id); // QUICKER MESSAGE
		if (philo->status.is_eating)
			printf(GREEN" <---- eating"RESET);
		else if (philo->status.is_sleeping)
			printf(SLEEP" <---- sleeping"RESET);
		else if (philo->status.is_thinking)
			printf(THINK" <---- thinking"RESET);
		else
			printf(FORK1" <---- no status is active"RESET);
		printf(" - timer %s%zums%s",
			philo->status.elapsed_time == 0 ? GREEN :
			(philo->status.elapsed_time >= philo->args->ttd ? DEATH : RESET), // Set color
			philo->status.elapsed_time, // Value
			RESET); // Reset color
		printf("\n");
		pthread_mutex_unlock(&philo->args->output_lock);
		pthread_mutex_unlock(active_mutex); // fork mutex and others
		return true;
	}
	return false;
}


 void thinks(t_philos *philo) // TO DO: need to understand why starvation freezes in thinks and sleeps --- SEEMS ONLY IN SLEEP??
 {
	pthread_mutex_lock(&philo->args->status_lock);
	philo->status.is_sleeping = false; // i removed this status from the mutex as philo should not race
	philo->status.is_eating = false;
	philo->status.is_thinking = true;
	// STARVATION NEEDS TO START WITH A MUTEX_LOCK AS IT WILL CLOSE IT INSIDE!!
	if(starvation(philo, &philo->args->status_lock))// checks if philo starved before printing (unlocks fork)
		return;
	print_activity(philo, get_curr_ms(philo->args->unix_start), "is thinking"); // message lok/unlock used for printing
	// unsure if thise needs to go back inside the lock
	ft_usleep(1, philo); // the sleep function needs to be put afterthe lock to avoid problems with bigger philo n
	// usleep(500); // As i use a lag of 500ms in ft_usleep, i need to add this to avoid problems
 }

 void sleeps(t_philos *philo)
 {
	// pthread_mutex_lock(&philo->args->status_lock);
	philo->status.is_eating = false; // same here
	philo->status.is_thinking = false;
	philo->status.is_sleeping = true;
	// STARVATION NEEDS TO START WITH A MUTEX_LOCK AS IT WILL CLOSE IT INSIDE!!
	// if(starvation(philo, &philo->args->status_lock)) // checks if philo starved before printing (unlocks fork)
	// 	return;
	print_activity(philo, get_curr_ms(philo->args->unix_start), "is sleeping"); // message lok/unlock used for printing
	ft_usleep(philo->args->tts, philo);
 }

 void eats(t_philos *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
 {

	//1. fork grabbing
	pthread_mutex_lock(first_fork);
	if(starvation(philo, first_fork))
		return;
	// checks if philo starved before printing (unlocks fork)
	// printf(FORK1"%zu %d has taken a fork - timer %zu\n"RESET, get_curr_ms(philo->args->unix_start), philo->id, (get_curr_ms(philo->args->unix_start) - philo->last_meal_time));
	// printf("%zu %d has taken a fork\n", get_curr_ms(philo->args->unix_start), philo->id);
	printf("%zu %d has taken a fork", get_curr_ms(philo->args->unix_start), philo->id);
	printf(" - timer %s%zums%s",
		philo->status.elapsed_time == 0 ? GREEN :
		(philo->status.elapsed_time >= philo->args->ttd ? DEATH : RESET), // Set color
		philo->status.elapsed_time, // Value
		RESET); // Reset color
	printf("\n");
	pthread_mutex_lock(second_fork);
	// printf(FORK2"%zu %d has taken a fork - timer %zu\n"RESET, get_curr_ms(philo->args->unix_start), philo->id, (get_curr_ms(philo->args->unix_start) - philo->last_meal_time));
	printf("%zu %d has taken a fork", get_curr_ms(philo->args->unix_start), philo->id);
	printf(" - timer %s%zums%s",
		philo->status.elapsed_time == 0 ? GREEN :
		(philo->status.elapsed_time >= philo->args->ttd ? DEATH : RESET), // Set color
		philo->status.elapsed_time, // Value
		RESET); // Reset color
	printf("\n");
	// printf("%zu %d has taken a fork\n", get_curr_ms(philo->args->unix_start), philo->id);
	// printf("%zu %d is eating - timer: %zu\n", get_curr_ms(philo->args->unix_start), philo->id, (get_curr_ms(philo->args->unix_start) - philo->last_meal_time));
	//---- as soon as the philo grabs the 2nd fork it starts eating and changing status
	philo->last_meal_time = get_curr_ms(philo->args->unix_start);
	philo->status.is_sleeping = false;
	philo->status.is_thinking = false;
	philo->status.is_eating = true;
	philo->meals_n++;

	// 2. lock output and print - so that the philosophers eating won't race against eachothee
	print_activity(philo, philo->last_meal_time, "is eating");
	// pthread_mutex_lock(&philo->args->output_lock);
	// // printf("%zu %d is eating\n", philo->last_meal_time, philo->id);
	// printf("%zu %d is eating", philo->last_meal_time, philo->id);
	// if (philo->status.is_eating)
    // 	printf(GREEN" <---- eating"RESET);
	// else if (philo->status.is_sleeping)
	// 	printf(SLEEP" <---- sleeping"RESET);
	// else if (philo->status.is_thinking)
	// 	printf(THINK" <---- thinking"RESET);
	// else
	// 	printf(FORK1" <---- no status is active"RESET);
	// printf("\n");
	// pthread_mutex_unlock(&philo->args->output_lock);

	ft_usleep(philo->args->tte, philo);// still in the 2nd fork lock, being locked for the usleep time

	// 3. fork releasing - as the time of eating passed the philo can finally release the forsks to the
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	usleep(500);// <--------------------------------- I PUT THIS SO THE PHILO WILL WAIT BEFORE THINK AND SLEEP IF SOMEONE DIES AT THE SAME TIME






 }

