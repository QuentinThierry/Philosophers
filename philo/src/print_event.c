/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_event.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:26:33 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/01 20:34:49 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	print_event(t_philo *philo, const char *message, long time)
{
	pthread_mutex_lock(philo->mut_end);
	if (*philo->is_end)
		return (pthread_mutex_unlock(philo->mut_end), true);
	pthread_mutex_lock(philo->mut_print);
	printf("%-ld %-ld %s\n", time,
		philo->id, message);
	pthread_mutex_unlock(philo->mut_print);
	pthread_mutex_unlock(philo->mut_end);
	return (true);
}
