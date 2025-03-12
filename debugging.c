/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:13:22 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/12 17:55:28 by jslusark         ###   ########.fr       */
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

        printf("[%d] ", philo->id);

        // Eating status
        printf("Eat: %s%s%s | ",
               philo->status.is_eating ? GREEN : RESET,
               philo->status.is_eating ? "true " : "false",
               RESET);

        // Sleeping status
        printf("Sleep: %s%s%s | ",
               philo->status.is_sleeping ? GREEN : RESET,
               philo->status.is_sleeping ? "true " : "false",
               RESET);

        // Thinking status
        printf("Think: %s%s%s | ",
               philo->status.is_thinking ? GREEN : RESET,
               philo->status.is_thinking ? "true " : "false",
               RESET);

        // Death status
        printf("Death: %s%s%s | ",
               philo->status.is_dead ? DEATH : RESET,
               philo->status.is_dead ? "yes" : "no ",
               RESET);

        printf("Last Meal Time: %zums", philo->last_meal_time);

        // // Timer stopped
        // printf("\n    Timer stopped: %zums\n",
        //     philo->status.timer_stopped >= philo->args->ttd ? DEATH : RESET, // Set color
        //     philo->status.timer_stopped, // Value
        //     RESET);

           // Timer stopped (fixing the issue here)
        printf("\n    Timer stopped: %s%zums%s\n",
            philo->status.timer_stopped >= philo->args->ttd ? DEATH : RESET, // Set color
            philo->status.timer_stopped, // Value
            RESET); // Reset color
        // printf("\n    Timer Stopped: %zums\n", philo->status.timer_stopped);
    }

    printf("------------------------------------------------------------\n");
}
