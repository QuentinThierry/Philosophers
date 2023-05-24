/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:21:31 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/24 19:28:33 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static inline double	calculate_delay(t_timeval *before)
{
	t_timeval	time;
	double		delay;

	gettimeofday(&time, NULL);
	delay = (time.tv_usec - before->tv_usec)
		+ (time.tv_sec - before->tv_sec) * CLOCKS_PER_SEC;
	*before = time;
	return (delay / 1000);
}

void	refresh_time(t_timeval *before, double *timestamps)
{
	double	tmp;

	tmp = calculate_delay(before);
	timestamps[delay] += tmp;
	timestamps[timestamp] += tmp;
	timestamps[last_meal] += tmp;
}
