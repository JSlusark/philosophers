/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/16 16:18:53 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"
// philosophers race only for shared resources/data:
// forks
// prints
// they do not compete for statuses, as each has their own data which cannot be acceessed from the others


void	print_activity(t_philos *philo, size_t time, char *message, size_t delay)
{
	philo->elapsed_time = get_curr_ms(philo->args->unix_start) - philo->last_meal_time;
	pthread_mutex_lock(&philo->args->output_lock);// i would call it activity lock
	printf("%zu %d %s", time, philo->id, message);
	if (philo->is_eating)
    	printf(GREEN" <---- eating"RESET);
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


bool starvation(t_philos * philo, pthread_mutex_t *locked_mutex) // if someone does not die left fork shoudl not be released
{
	philo->elapsed_time = get_curr_ms(philo->args->unix_start) - philo->last_meal_time;
	if( philo->elapsed_time >= philo->args->ttd)
	{
		philo->is_dead = true;
		print_activity(philo, philo->elapsed_time, "IS DEAD ☠️ ☠️ ☠️ ☠️!!!!",0);
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
	if(starvation(philo, NULL))// checks if philo starved before printing
		return;
	print_activity(philo, get_curr_ms(philo->args->unix_start), "is thinking", 1); // message lok/unlock used for printing
	// ft_usleep(1, philo); // the sleep function needs to be put afterthe lock to avoid problems with bigger philo n
	// usleep(500); // As i use a lag of 500ms in ft_usleep, i need to add this to avoid problems
 }

 void sleeps(t_philos *philo)
 {
	philo->is_eating = false;
	philo->is_thinking = false;
	philo->is_sleeping = true; ///// <--------
	if(starvation(philo, NULL)) // checks if philo starved before printing
		return;
	print_activity(philo, get_curr_ms(philo->args->unix_start), "is sleeping", philo->args->tts); // message lok/unlock used for printing
	// ft_usleep(philo->args->tts, philo);
 }

 void eats(t_philos *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
 {

	//1. fork grabbing
	pthread_mutex_lock(first_fork);
	if(starvation(philo, first_fork)) // checks if philo starved before printing
		return; // 1st fork is unlocked only if philo is dead
	print_activity(philo, get_curr_ms(philo->args->unix_start), "has taken 1ST fork",0);

	//2. fork grabbing
	pthread_mutex_lock(second_fork);
	print_activity(philo, get_curr_ms(philo->args->unix_start), "has taken 2ND fork",0); // output locks/unlocks to avoid racing for printing

	// 3. philo eats as soon as taken both forks
	philo->last_meal_time = get_curr_ms(philo->args->unix_start);
	philo->is_eating = true; ///// <--------
	philo->is_thinking = false;
	philo->is_sleeping = false;
	philo->meals_n++;
	print_activity(philo, philo->last_meal_time, "is eating", philo->args->tte);  // output locks/unlocks to avoid racing for printing
	// ft_usleep(philo->args->tte, philo);// philo has to eat (hold forks) for  this amount of time

	// 4. fork releasing
	print_activity(philo, get_curr_ms(philo->args->unix_start), "has released 1ST fork",0); // output locks/unlocks to avoid racing for printing
	pthread_mutex_unlock(first_fork);
	// pthread_mutex_lock(&philo->args->output_lock);
	// this should be printed after philo dies too no?
	// printf(FORK2"%zu %d has released second fork - FOUND DEAD: %d\n"RESET, get_curr_ms(philo->args->unix_start), philo->id, philo->args->found_dead);
	// pthread_mutex_unlock(&philo->args->output_lock);
	print_activity(philo, get_curr_ms(philo->args->unix_start), "has released 2nD fork",0); // output locks/unlocks to avoid racing for printing
	pthread_mutex_unlock(second_fork);
	usleep(500);// <--------------------------------- I PUT THIS SO THE PHILO WILL WAIT BEFORE THINK AND SLEEP IF SOMEONE DIES AT THE SAME TIME






 }

