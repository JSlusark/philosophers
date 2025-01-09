/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:46:53 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/09 13:26:50 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void cleanup(t_data *program) {
	// Destroy mutexes
	for (int i = 0; i < program->args.philos_n; i++) {
		pthread_mutex_destroy(&program->forks[i]);
	}

	// Free memory
	if (program->forks)
		free(program->forks);
	if (program->philo)
		free(program->philo);
}


int	parse_args(int argc)
{
	if(argc != 5 && argc != 6)
	{
		printf("Error: run ./philos philos_n ttd tte tts and eats_n(optional)\n");
		return (0);
	}
	return (1);
}

int main(int argc, char **argv)
{
	t_data				program; // needs to be malloced after parsing?
	t_philos			philosopher[200]; // tests will be done with no more than 200 philos

	(void) philosopher;
	// remember to add if philos_n is 1 and philos_n is even or odd conditions
	if(!parse_args(argc) || !init_data(argc, argv, &program, &program.args))
		return(1);
	// instead of int i can actually put the function here tbh as main is short
	if(!start_simulation(&program, philosopher))
	// 	return(1);
	cleanup(&program);
	return(0);
}