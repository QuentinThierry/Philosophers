/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_event.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:26:33 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/26 19:33:40 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	print_event(t_philo *philo, const char *message)
{
	pthread_mutex_lock(philo->mut_end);
	if (*philo->is_end)
		return (pthread_mutex_unlock(philo->mut_end), true);
	pthread_mutex_lock(philo->mut_print);
	printf("%-5ld %-3ld %s\n", get_timestamp(*philo->start_time),
		philo->id, message);
	pthread_mutex_unlock(philo->mut_print);
	pthread_mutex_unlock(philo->mut_end);
	return (true);
}
