/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 13:58:30 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/20 17:28:12 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	print_activity(t_philos *philo, size_t time, char *message, size_t delay)
{
	pthread_mutex_lock(&philo->args->output_lock);// i would call it activity lock
	philo->elapsed_time = get_curr_ms(philo->args->unix_start) - philo->last_meal_time;

	pthread_mutex_lock(&philo->args->status_lock);// i would call it activity lock
	if(philo->args->found_dead)
	{
		// printf("[DEBUG] %d FOUND SOMEONE DEAD %s", philo->id, message);
		// printf("________ death - %d:%d or someone\n", philo->is_dead, philo->args->found_dead);
		pthread_mutex_unlock(&philo->args->status_lock);// i would call it activity lock
		pthread_mutex_unlock(&philo->args->output_lock);// i would call it activity lock
		return;
	}
	if( philo->elapsed_time >= philo->args->ttd && !philo->is_eating)
	{
		philo->is_dead = true;
		// printf(DEATH"[DEBUG] %d DIES %s ___ elapse %zu"RESET, philo->id, message, philo->elapsed_time);
		// printf("________ death - %d:%d or someone\n", philo->is_dead, philo->args->found_dead);
		philo->is_dead = true;
		philo->args->found_dead = true;
		pthread_mutex_unlock(&philo->args->status_lock);// i would call it activity lock
		pthread_mutex_unlock(&philo->args->output_lock);// i would call it activity lock
		return;
	}
	pthread_mutex_unlock(&philo->args->status_lock);// i would call it activity lock

	printf("%zu %d %s", time, philo->id, message);
	if (philo->is_eating)
		printf(GREEN" 🍝: %d <---- eating"RESET, philo->meals_n);
	else if (philo->is_sleeping)
		printf(SLEEP" <---- sleeping"RESET);
	else if (philo->is_thinking)
		printf(THINK" <---- thinking"RESET);
	else
		printf(FORK1" <---- no status is active"RESET);
	printf(" - timer %s%zums%s",
		philo->elapsed_time == 0 ? GREEN :
		(philo->elapsed_time >= philo->args->ttd ? DEATH"  XXXXXXX  " : RESET), // Set color
		philo->elapsed_time, // Value
		RESET); // Reset color
	printf("________ death - %d:%d or someone", philo->is_dead, philo->args->found_dead);
	printf("\n");
	pthread_mutex_unlock(&philo->args->output_lock);
	if(delay >= 1) /// unsure to include 0 here
		usleep(delay * 1000);
	else
		usleep(500);
}


 void thinks(t_philos *philo) // TO DO: need to understand why starvation freezes in thinks and sleeps --- SEEMS ONLY IN SLEEP??
 {
	pthread_mutex_lock(&philo->args->status_lock);
	philo->is_eating = false;
	philo->is_thinking = true;///// <--------
	philo->is_sleeping = false;
	pthread_mutex_unlock(&philo->args->status_lock);
	print_activity(philo, get_curr_ms(philo->args->unix_start), THINK"is thinking"RESET, 0); // message lok/unlock used for printing

	// ft_usleep(1, philo); // the sleep function needs to be put afterthe lock to avoid problems with bigger philo n
 }

 void sleeps(t_philos *philo)
 {
	pthread_mutex_lock(&philo->args->status_lock);
	philo->is_eating = false;
	philo->is_thinking = false;
	philo->is_sleeping = true; ///// <--------
	pthread_mutex_unlock(&philo->args->status_lock);
	print_activity(philo,  get_curr_ms(philo->args->unix_start), SLEEP"is sleeping"RESET, philo->args->tts); // message lok/unlock used for printing
}

 void eats(t_philos *philo, pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
 {
	//1. fork grabbing
	pthread_mutex_lock(first_fork);
	print_activity(philo, get_curr_ms(philo->args->unix_start), FORK2"has taken a fork"RESET, 0);  // output locks/unlocks to avoid racing for printing
	if (philo->args->philos_n == 1)
	{
		usleep(philo->args->ttd * 1000);
		pthread_mutex_unlock(first_fork);
		return;
	}
	//2. fork grabbing
	pthread_mutex_lock(second_fork);
	print_activity(philo, get_curr_ms(philo->args->unix_start), FORK2"has taken a fork"RESET, 0);  // output locks/unlocks to avoid racing for printing
	// 3. philo eats as soon as taken both forks
	pthread_mutex_lock(&philo->args->status_lock);
	philo->last_meal_time = get_curr_ms(philo->args->unix_start);
	philo->is_eating = true; ///// <--------
	philo->is_thinking = false;
	philo->is_sleeping = false;
	pthread_mutex_lock(&philo->args->meal_lock);// i would call it activity lock
	philo->meals_n++;
	pthread_mutex_unlock(&philo->args->meal_lock);// i would call it activity lock
	pthread_mutex_unlock(&philo->args->status_lock);
	print_activity(philo, philo->last_meal_time, "is eating", philo->args->tte);  // output locks/unlocks to avoid racing for printing
	usleep(500); // <----- I ADDED THIS DELAY TO HELP WITH  3 610 200 100 (which delay was coming from fork release delay)
	// 4. fork releasing
	// print_activity(philo, get_curr_ms(philo->args->unix_start), FORK1"has released 1st fork"RESET,0);  // output locks/unlocks to avoid racing for printing
	pthread_mutex_unlock(first_fork);
	usleep(500);// <----- I ADDED THIS DELAY TO HELP WITH  3 610 200 100 (which delay was coming from fork release delay
	// print_activity(philo, get_curr_ms(philo->args->unix_start), FORK1"has released 2nd fork"RESET,0);  // output locks/unlocks to avoid racing for printing
	pthread_mutex_unlock(second_fork);
	// usleep(500);// <--------------------------------- I PUT THIS SO THE PHILO WILL WAIT BEFORE THINK AND SLEEP IF SOMEONE DIES AT THE SAME TIME
 }

