/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:31:27 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/30 16:18:15 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long	print_event(t_philo philo, const char *message)
{
	long	time;
	long	time2;

	sem_wait(philo.sem_print);
	time = get_timestamp(philo.start_time);
	printf("%ld %d %s\n",
		time, philo.id, message);
	sem_post(philo.sem_print);
	time2 = get_timestamp(philo.start_time);
	return (time2 - time);
}

bool	init(int argc, char **argv, t_philo *philo, pid_t **pids)
{
	close_semaphores(NULL);
	*philo = (t_philo){0};
	if (!parsing(argc, argv, philo))
		return (false);
	*pids = ft_calloc(philo->nb_philos + 1, sizeof(pid_t));
	if (!*pids)
		return (false);
	if (!open_semaphores(philo))
		return (free(*pids), false);
	return (true);
}
