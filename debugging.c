/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:13:22 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/12 16:52:37 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// adding debug printing for checking purposes at the end of the simulation

// prints a list of philos and their status at the end of the simulation

// 1. eating times - time waited before started eating
// 2. sleeping times
// 3. thinking times

#include "philos.h"
void print_mealcount(t_data *program)
{
    printf("\nPHILO MEAL COUNT:\n");
    printf("-----------------------------------------\n");

    for (int i = 0; i < program->args.philos_n; i++)
    {
        printf("Philosopher %d ate %d times.\n", program->philo[i].id, program->philo[i].meals_n);
    }

    printf("-----------------------------------------\n");
}
