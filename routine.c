/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/18 12:41:45 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
// philosophers race only for shared resources/data:
// forks
// prints
// they do not compete for statuses, as each has their own data which cannot be acceessed from the others


void	print_activity(t_philos *philo, size_t time, char *message, size_t delay)
{
	// pthread_mutex_lock(&philo->args->status_lock);
	philo->elapsed_time = get_curr_ms(philo->args->unix_start) - philo->last_meal_time;
	// pthread_mutex_unlock(&philo->args->status_lock);
	pthread_mutex_lock(&philo->args->output_lock);// i would call it activity lock
	printf("%zu %d %s", time, philo->id, message);
	if (philo->is_eating)
    	printf(GREEN" 🍝: %d <---- eating"RESET, philo->meals_n);
	else if (philo->is_sleeping)
		printf(SLEEP" <---- sleeping"RESET);
	else if (philo->is_thinking)
		printf(THINK" <---- thinking"RESET);
	else
		printf(FORK1" <---- no status is active"RESET);
	// elapsed time to check death
	printf(" - timer %s%zums%s",
		philo->elapsed_time == 0 ? GREEN :
		(philo->elapsed_time >= philo->args->ttd ? DEATH : RESET), // Set color
		philo->elapsed_time, // Value
		RESET); // Reset color
	printf("\n");
	pthread_mutex_unlock(&philo->args->output_lock);
	if(delay > 0)
		ft_usleep(delay, philo);
}


bool starvation(t_philos *philo, pthread_mutex_t *locked_mutex) // if someone does not die left fork shoudl not be released
{
	// do not store this in the locked mutex perhaps?
	// pthread_mutex_lock(&philo->args->status_lock);
	philo->elapsed_time = get_curr_ms(philo->args->unix_start) - philo->last_meal_time;
	// pthread_mutex_unlock(&philo->args->status_lock);
	// if (philo->is_eating) // this doesnt matter
	// 	return false;
	if( philo->elapsed_time >= philo->args->ttd && !philo->is_eating)
	{
		// lock deadlock?
		philo->is_dead = true;
		philo->args->found_dead = true;
		// unlock deadlock?
		/// need to pass normal time not elapsed time
		print_activity(philo, get_curr_ms(philo->args->unix_start), DEATH"died"RESET,0);
		if (locked_mutex)  // Check if mutex is valid before unlocking - sleeps an thinks pass NULL as no mutex is active
			pthread_mutex_unlock(locked_mutex);
		return true;
	}
	return false;
}


 void thinks(t_philos *philo) // TO DO: need to understand why starvation freezes in thinks and sleeps --- SEEMS ONLY IN SLEEP??
 {
	philo->is_eating = false;
	philo->is_thinking = true;///// <--------
	philo->is_sleeping = false;
	// philo->start_activity = get_curr_ms(philo->args->unix_start);

	if( philo->args->found_dead)// checks if philo starved before printing
		return;
	print_activity(philo, get_curr_ms(philo->args->unix_start), "is thinking", 1); // message lok/unlock used for printing
	// ft_usleep(1, philo); // the sleep function needs to be put afterthe lock to avoid problems with bigger philo n
 }

 void sleeps(t_philos *philo)
 {
	philo->is_eating = false;
	philo->is_thinking = false;
	philo->is_sleeping = true; ///// <--------
	// philo->start_activity = get_curr_ms(philo->args->unix_start);
	if( philo->args->found_dead ) // checks if philo starved before printing
		return;
	print_activity(philo,  get_curr_ms(philo->args->unix_start), "is sleeping", philo->args->tts); // message lok/unlock used for printing
}

 void eats(t_philos *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
 {
	//1. fork grabbing
	pthread_mutex_lock(first_fork);
	// philo->start_activity = get_curr_ms(philo->args->unix_start);
	// if( philo->args->found_dead || starvation(philo, first_fork)) // checks if philo starved before printing
	if( philo->args->found_dead) // checks if philo starved before printing
	{
		pthread_mutex_unlock(first_fork);
		return; // 1st fork is unlocked only if philo is dead
	}
	// printf(FORK2"%zu %d %s <--- %zu\n"RESET, get_curr_ms(philo->args->unix_start), philo->id, "took 1st fork", get_curr_ms(philo->args->unix_start) - philo->last_meal_time);
	print_activity(philo, get_curr_ms(philo->args->unix_start), FORK2"has taken 1st fork"RESET, 0);  // output locks/unlocks to avoid racing for printing



	//2. fork grabbing
	pthread_mutex_lock(second_fork);
	// philo->start_activity = get_curr_ms(philo->args->unix_start);
	// printf(FORK2"%zu %d %s <--- %zu\n"RESET, get_curr_ms(philo->args->unix_start), philo->id, "took 2nd fork", get_curr_ms(philo->args->unix_start) - philo->last_meal_time);
	print_activity(philo, get_curr_ms(philo->args->unix_start), FORK2"has taken 2nd fork"RESET, 0);  // output locks/unlocks to avoid racing for printing

	philo->last_meal_time = get_curr_ms(philo->args->unix_start);

	// 3. philo eats as soon as taken both forks
	// pthread_mutex_lock(&philo->args->status_lock);
	philo->is_eating = true; ///// <--------
	philo->is_thinking = false;
	philo->is_sleeping = false;
	philo->meals_n++;
	// pthread_mutex_unlock(&philo->args->status_lock);
	print_activity(philo, philo->last_meal_time, "is eating", philo->args->tte);  // output locks/unlocks to avoid racing for printing

	// 4. fork releasing
	print_activity(philo, get_curr_ms(philo->args->unix_start), FORK1"has released 1st fork"RESET,0);  // output locks/unlocks to avoid racing for printing
	pthread_mutex_unlock(first_fork);
	print_activity(philo, get_curr_ms(philo->args->unix_start), FORK1"has released 2nd fork"RESET,0);  // output locks/unlocks to avoid racing for printing
	pthread_mutex_unlock(second_fork);
	// usleep(500);// <--------------------------------- I PUT THIS SO THE PHILO WILL WAIT BEFORE THINK AND SLEEP IF SOMEONE DIES AT THE SAME TIME
 }

