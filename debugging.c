/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debugging.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 17:30:23 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/21 11:48:47 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

void	routine_debugging(t_philos *philo)
{
	if (philo->is_eating)
		printf(GREEN"          <---- eating 🍝 #%d  "RESET, philo->meals_n);
	else if (philo->is_sleeping)
		printf(SLEEP"          <---- sleeping      "RESET);
	else if (philo->is_thinking)
		printf(THINK"     <---- thinking      "RESET);
	else
		printf(FORK1"      <---- no status     "RESET);
	printf(" - timer ");
	if (philo->elapsed_time == 0)
		printf("%s%zums%s      ", GREEN, philo->elapsed_time, RESET);
	else if (philo->elapsed_time >= philo->args->ttd)
		printf("%s%zums%s      ", DEATH"  XXXXXXX  ",
			philo->elapsed_time, RESET);
	else
		printf("%s%zums%s      ", RESET, philo->elapsed_time, RESET);
	printf("p:%d s:%d", philo->is_dead,
		philo->args->found_dead);
	printf("\n");
}

void	print_philo(t_philos *philo)
{
	printf(FORK1"[%d] "RESET, philo->id);
	if (philo->is_eating)
		printf("Eat: %strue %s | ", GREEN, RESET);
	else
		printf("Eat: %sfalse%s | ", RESET, RESET);
	if (philo->is_sleeping)
		printf("Sleep: %strue %s | ", GREEN, RESET);
	else
		printf("Sleep: %sfalse%s | ", RESET, RESET);
	if (philo->is_thinking)
		printf("Think: %strue %s | ", GREEN, RESET);
	else
		printf("Think: %sfalse%s | ", RESET, RESET);
	if (philo->is_dead)
		printf("Death: %syes%s | ", DEATH, RESET);
	else
		printf("Death: %sno %s | ", RESET, RESET);
	printf("Last Meal Time: %zums", philo->last_meal_time);
	if (philo->elapsed_time >= philo->args->ttd)
		printf("\n    Timer since end of last meal: %s%zums%s\n",
			DEATH, philo->elapsed_time, RESET);
	else
		printf("\n    Timer since end of last meal: %s%zums%s\n",
			RESET, philo->elapsed_time, RESET);
}

void	print_mealcount(t_data *program)
{
	int	i;

	printf(FORK1"\nPHILO MEAL COUNT:\n"RESET);
	printf("-----------------------------------------\n");
	i = 0;
	while (i < program->args.philos_n)
	{
		printf("Philosopher %d ate %d times.\n",
			program->philo[i].id, program->philo[i].meals_n);
		i++;
	}
	printf("-----------------------------------------\n");
}

void	print_status(t_data *program)
{
	int			i;
	t_philos	*philo;

	i = 0;
	printf(GREEN"\nSimulation Parameters:\n"RESET);
	printf("PHILOSOPHERS: %s%d%s | ", GREEN, program->args.philos_n, RESET);
	printf("TTD: %s%zums%s | ", DEATH, program->args.ttd, RESET);
	printf("TTE: %zums | ", program->args.tte);
	printf("TTS: %zums | ", program->args.tts);
	printf("Meal Limit: %d\n", program->args.meals_limit);
	printf("\nPhilosopher Status at End of Simulation:\n");
	printf("------------------------------------------------------------\n");
	while (i < program->args.philos_n)
	{
		philo = &program->philo[i];
		print_philo(philo);
		i++;
	}
	printf("------------------------------------------------------------\n");
}
