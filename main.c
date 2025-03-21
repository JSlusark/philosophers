/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:46:53 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/21 10:09:37 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	cleanup(t_data *program)
{
	int	i;

	i = 0;
	while (i < program->args.philos_n)
	{
		pthread_mutex_destroy(&program->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&program->args.dead_lock);
	pthread_mutex_destroy(&program->args.status_lock);
	pthread_mutex_destroy(&program->args.meal_lock);
	pthread_mutex_destroy(&program->args.output_lock);
	if (program->forks)
		free(program->forks);
	if (program->philo)
		free(program->philo);
	free(program);
}

bool	parse_args(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc != 5 && argc != 6)
	{
		printf("Error: run ./philos #n ttd tte tts and eats_n(optional)\n");
		return (false);
	}
	while (i < argc)
	{
		if (ft_atoi(argv[i]) < 0)
		{
			printf("Error: ./philo args cannot be of negative value\n");
			return (false);
		}
		i++;
	}
	return (true);
}

int	main(int argc, char **argv)
{
	t_data				*program;
	int					return_value;

	program = malloc(sizeof(t_data));
	if (!parse_args(argc, argv))
	{
		free(program);
		return (1);
	}
	else if (!init_data(argc, argv, program) || !start_simulation(program))
		return_value = 1;
	else
		return_value = 0;
	cleanup(program);
	return (return_value);
}
