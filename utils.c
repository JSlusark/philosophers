/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jslusark <jslusark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:05:39 by jslusark          #+#    #+#             */
/*   Updated: 2025/03/20 18:31:26 by jslusark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philos.h"

/*
	get_unix_timestamp()
	returns the current time in milliseconds since the Unix epoch
	long is unlikely to overflow as the simiulation will not run indefinitely
	builtin struct that stores time in seconds and microseconds
	tv_sec: Seconds since January 1, 1970 (the "unix epoch").
	tv_usec: Microseconds (one_millionth of a second).
	1 second = 1,000,000 microseconds.
	conversion to milliseconds as more precise than seconds,
	it is enough to solve the dining philosophers problem
*/

size_t	get_curr_ms(long start)
{
	return (get_unix_timestamp() - start);
}

long	get_unix_timestamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_atoi(const char *nptr)
{
	int		num;
	int		sign;
	size_t	i;

	num = 0;
	sign = 1;
	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+' )
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = (num * 10) + (nptr[i] - '0');
		i++;
	}
	return (num * sign);
}

void	ft_usleep(size_t milliseconds, t_philos *philo)
{
	size_t	start;

	start = get_curr_ms(philo->args->unix_start);
	while ((get_curr_ms(philo->args->unix_start) - start) < milliseconds)
		usleep(500);
}
