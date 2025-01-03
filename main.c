/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:46:53 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/03 17:26:13 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	parse_args(int argc, char **argv)
{
	if(argc != 5 && argc != 6)
	{
		printf("Error: run ./philos philos_n time_to_die time_to_eat time_to_sleep and eats_n(optional)\n");
		return (0);
	}
	(void)argv; // to remove after
	// printf("philos_n: %d, time_to_die: %d, time_to_eat: %d, time_to_sleep: %d\n", ft_atoi(argv[1]), ft_atoi(argv[2]), ft_atoi(argv[3]), ft_atoi(argv[4]));
	return (1);
}

int main(int argc, char **argv)
{
	t_data			program; // needs to be malloced after parsing?
	t_philo			philosopher[200]; // tests will be done with no more than 200 philos
	(void) philosopher;

	if(!parse_args(argc, argv) || !init_data(argc, argv, &program))
		return(1);

	/*
		NOTES:
		1. parse the arguments
		2. initialize the struct data program: atoi args, init forks, init philos
		3. create threads for each philosopher
		4. join threads
		5. free all malloced memory
		6. exit
	 */



	printf("XXX philos_n: %d, time_to_die: %d, time_to_eat: %d, time_to_sleep: %d\n", program.philos_n, program.time_to_die, program.time_to_eat, program.time_to_sleep); // PRINTS WRONG
	printf("PROGRAM RUNNING...\n");
	printf("time: %ld\n", program.philo[0].started_eating);
	printf("time: %ld\n", program.philo[50].started_eating);
	printf("PHIL1: %d\n", program.philo[1].id);
	printf("PHIL50: %d\n", program.philo[50].id);
	// if(program.philos_n == 1) // i guess this has to run as a thread once i handle them
	// {
	// 	printf("%d is dead\n", program.philos_n);
	// 	return(1);
	// }
	return(0);
}