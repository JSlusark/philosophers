/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:46:53 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/21 12:22:27 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

bool	check_values(t_rules *args)
{
	if (args->philos_n < 1 || args->ttd < 1 || args->tte < 1 || args->tts < 1)
	{
		printf("Error: the first 4 values should be greater than 0\n");
		return (false);
	}
	if (args->philos_n > 200)
	{
		printf("Error: not allowed to test philos_n greater than 200\n");
		return (false);
	}
	// what about intmax limit for args???
	return (true);
}

bool	init_data(int argc, char **argv, t_data *program)
{
	program->args.philos_n = ft_atoi(argv[1]);
	program->args.ttd = ft_atoi(argv[2]);
	program->args.tte = ft_atoi(argv[3]);
	program->args.tts = ft_atoi(argv[4]);
	if (argc == 6)
		program->args.meals_limit = ft_atoi(argv[5]);
	else
		program->args.meals_limit = -1;
	if (!check_values(&program->args))
		return (false);
	if (!init_forks(program))
		return (false);
	if (!init_philos(program))
		return (false);
	if (program->args.meals_limit == 0)
		return (false);
	program->args.found_dead = false;
	pthread_mutex_init(&program->args.dead_lock, NULL);
	pthread_mutex_init(&program->args.status_lock, NULL);
	pthread_mutex_init(&program->args.meal_lock, NULL);
	pthread_mutex_init(&program->args.output_lock, NULL);
	program->args.unix_start = get_unix_timestamp();
	return (true);
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
	if (!parse_args(argc, argv) || !init_data(argc, argv, program))
	{
		free(program);
		return (1);
	}
	else if (!start_simulation(program))
		return_value = 1;
	else
		return_value = 0;
	cleanup(program);
	return (return_value);
}
