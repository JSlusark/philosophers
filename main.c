/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:46:53 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/02 15:46:16 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	parse_args(int argc, char **argv, t_data *program)
{
	if(argc < 5 || argc > 6)
	{
		printf("Error: run ./philos philo_n time_to_die time_to_eat time_to_sleep\n");
		return (0);
	}
	printf("philo_n: %d, time_to_die: %d, time_to_eat: %d, time_to_sleep: %d\n", ft_atoi(argv[1]), ft_atoi(argv[2]), ft_atoi(argv[3]), ft_atoi(argv[4]));
	program->philo_n = ft_atoi(argv[1]);
	program->time_to_die = ft_atoi(argv[2]);
	program->time_to_eat = ft_atoi(argv[3]);
	program->time_to_sleep = ft_atoi(argv[4]);
	program->forks_n = program->philo_n;
	return (1);
}

int main(int argc, char **argv)
{
	t_data program;

	if(!parse_args(argc, argv, &program))
	{
		printf("Error: parsing arguments failed! \n");
		return(1);
	}
	if(program.philo_n == 0)
	{
		printf("Error: philo_n should be greater than 1\n");
		return(1);
	}
	printf("PROGRAM RUNNING...\n");
	if(program.philo_n == 1) // i guess this has to run as a thread once i handle them
	{
		printf("%d is dead\n", program.philo_n);
		return(1);
	}
	return(0);
}