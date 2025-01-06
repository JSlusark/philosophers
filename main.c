/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 13:46:53 by jslusark          #+#    #+#             */
/*   Updated: 2025/01/06 17:34:06 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

int	parse_args(int argc)
{
	if(argc != 5 && argc != 6)
	{
		printf("Error: run ./philos philos_n time_to_die time_to_eat time_to_sleep and eats_n(optional)\n");
		return (0);
	}
	return (1);
}

int main(int argc, char **argv)
{
	t_data				program; // needs to be malloced after parsing?
	t_philos			philosopher[200]; // tests will be done with no more than 200 philos
	(void) philosopher;

	if(!parse_args(argc) || !init_data(argc, argv, &program))
		return(1);
	start_simulation(&program, philosopher); // remember to add if philos_n is 1 and philos_n is even or odd conditions
	return(0);
}