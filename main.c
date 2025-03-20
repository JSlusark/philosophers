/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:46:53 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/20 18:26:02 by jslusark         ###   ########.fr       */
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
	t_data				program;
	t_philos			philosopher[200];

	if (!parse_args(argc, argv) || !init_data(argc, argv, &program))
		return (1);
	if (!start_simulation(&program))
		return (1);
	cleanup(&program);
	return (0);
}
