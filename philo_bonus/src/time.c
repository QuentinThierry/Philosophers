/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:21:31 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/18 20:48:24 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	my_usleep(t_philo *philo, long delay, long start_time)
{
	long	time;

	time = get_timestamp(philo->origin_time);
	while (time - start_time < delay)
	{
		usleep(50);
		time = get_timestamp(philo->origin_time);
	}
}

long	get_timestamp(t_timeval origin_time)
{
	t_timeval	time;

	gettimeofday(&time, NULL);
	time.tv_sec -= origin_time.tv_sec;
	time.tv_usec -= origin_time.tv_usec;
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long	get_time_diff(t_timeval start_time, long t_before)
{
	return (get_timestamp(start_time) - t_before);
}
