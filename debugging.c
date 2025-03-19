/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:13:22 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/16 15:11:17 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void print_mealcount(t_data *program)
{
    printf(FORK1"\nPHILO MEAL COUNT:\n"RESET);
    printf("-----------------------------------------\n");

    for (int i = 0; i < program->args.philos_n; i++)
    {
        printf("Philosopher %d ate %d times.\n", program->philo[i].id, program->philo[i].meals_n);
    }

    printf("-----------------------------------------\n");
}
void print_status(t_data *program)
{
    printf(GREEN"\nSimulation Parameters:\n"RESET);
    printf("PHILOSOPHERS: %s%d%s | ", GREEN, program->args.philos_n, RESET);
    printf("TTD: %s%zums%s | ", DEATH, program->args.ttd, RESET);
    printf("TTE: %zums | ", program->args.tte);
    printf("TTS: %zums | ", program->args.tts);
    printf("Meal Limit: %s%d%s\n",
        (program->args.meals_limit > 0 ? GREEN : RESET),
        program->args.meals_limit,
        RESET);
    printf("\nPhilosopher Status at End of Simulation:\n");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < program->args.philos_n; i++)
    {
        t_philos *philo = &program->philo[i];

        printf(FORK1"[%d] "RESET, philo->id);

        // Eating status
        printf("Eat: %s%s%s | ",
               philo->is_eating ? GREEN : RESET,
               philo->is_eating ? "true " : "false",
               RESET);

        // Sleeping status
        printf("Sleep: %s%s%s | ",
               philo->is_sleeping ? GREEN : RESET,
               philo->is_sleeping ? "true " : "false",
               RESET);

        // Thinking status
        printf("Think: %s%s%s | ",
               philo->is_thinking ? GREEN : RESET,
               philo->is_thinking ? "true " : "false",
               RESET);

        // Death status
        printf("Death: %s%s%s | ",
               philo->is_dead ? DEATH : RESET,
               philo->is_dead ? "yes" : "no ",
               RESET);

        printf("Last Meal Time: %zums", philo->last_meal_time);

        // Timer stopped (fixing the issue here)
        printf("\n    Timer since end of last meal: %s%zums%s\n",
            philo->elapsed_time >= philo->args->ttd ? DEATH : RESET, // Set color
            philo->elapsed_time, // Value
            RESET); // Reset color
    }

    printf("------------------------------------------------------------\n");
}
