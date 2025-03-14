/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:13:22 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/15 18:10:48 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// adding debug printing for checking purposes at the end of the simulation

// prints a list of philos and their status at the end of the simulation

// 1. eating times - time waited before started eating
// 2. sleeping times
// 3. thinking times

#include "philos.h"

void monitor_alert(t_philos *philo)
{
    size_t time_since_last_meal = get_curr_ms(philo->args->unix_start) - philo->last_meal_time;

    // Print only if the time since the last meal is greater than or equal to time to die
    if (time_since_last_meal >= philo->args->ttd)
    {
        printf(DEATH"ALERT: %zu %d HAS TO DIE! Last meal timer: %zums\n"RESET,
               get_curr_ms(philo->args->unix_start), philo->id, time_since_last_meal);
    }
}

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

        // Timer stopped (fixing the issue here)
        printf("\n    Timer since end of last meal: %s%zums%s\n",
            philo->status.elapsed_time >= philo->args->ttd ? DEATH : RESET, // Set color
            philo->status.elapsed_time, // Value
            RESET); // Reset color
    }

    printf("------------------------------------------------------------\n");
}
