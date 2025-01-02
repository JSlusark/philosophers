/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:46:53 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/02 17:19:21 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int check_values(t_data *program)
{
	if(program->philos_n < 1 || program->time_to_die < 1 || program->time_to_eat < 1 || program->time_to_sleep < 1 || program->eat_req < 1)
	{
		printf("Error: all values should be greater than 0\n"); // check if this also needs to be for the times
		return(0);
	}
	if(program->philos_n > 200)
	{
		printf("Error: philos_n should not greater than 200\n");
		return(0);
	}
	// what about intmax limit?
	return(1);
}

int	parse_args(int argc, char **argv, t_data *program)
{
	if(argc != 5 && argc != 6)
	{
		printf("Error: run ./philos philos_n time_to_die time_to_eat time_to_sleep and eats_n(optional)\n");
		return (0);
	}
	printf("philos_n: %d, time_to_die: %d, time_to_eat: %d, time_to_sleep: %d\n", ft_atoi(argv[1]), ft_atoi(argv[2]), ft_atoi(argv[3]), ft_atoi(argv[4]));
	program->philos_n = ft_atoi(argv[1]); // i have to hanldle letters? should not give 0??
	program->forks_n = program->philos_n;
	program->time_to_die = ft_atoi(argv[2]);
	program->time_to_eat = ft_atoi(argv[3]);
	program->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		program->eat_req = ft_atoi(argv[5]);
	else
		program->eat_req = 0; // what happens when this is not added?
	return (1);
}

int main(int argc, char **argv)
{
	t_data program; // needs to be malloced after parsing
	// t_philo philo; // needs to be malloced after parsing

	if(!parse_args(argc, argv, &program) || !check_values(&program))
		return(1);
	printf("PROGRAM RUNNING...\n");
	if(program.philos_n == 1) // i guess this has to run as a thread once i handle them
	{
		printf("%d is dead\n", program.philos_n);
		return(1);
	}
	return(0);
}