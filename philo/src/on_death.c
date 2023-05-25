/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   on_death.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:27:08 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/25 22:07:20 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	broadcast_death(t_philo	*philo, long time, bool has_print)
{
	pthread_mutex_lock(philo->mut_end);
	if (*philo->is_end)
		return ((void)pthread_mutex_unlock(philo->mut_end));
	pthread_mutex_lock(philo->mut_print);
	*philo->is_end = true;
	time = get_timestamp(philo->start_time);
	if (has_print)
		printf("%-5ld %-3ld %s\n", time, philo->id, "is dead");
	pthread_mutex_unlock(philo->mut_print);
	pthread_mutex_unlock(philo->mut_end);
}
