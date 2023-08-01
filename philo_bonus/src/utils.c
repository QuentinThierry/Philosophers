/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:31:27 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/01 16:31:48 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	kill_all_philos(t_philo philo, pid_t *pids)
{
	int	i;

	i = 0;
	close_semaphores(&philo);
	while (i < philo.nb_philos + 1)
	{
		if (pids[i] != 0)
			kill(pids[i], SIGKILL);
		i++;
	}
}

void	print_event(t_philo philo, const char *message, long time)
{
	sem_wait(philo.sem_print);
	printf("%ld %d %s\n",
		time, philo.id, message);
	sem_post(philo.sem_print);
}

bool	init(int argc, char **argv, t_philo *philo, pid_t **pids)
{
	*philo = (t_philo){0};
	if (!parsing(argc, argv, philo))
		return (false);
	*pids = ft_calloc(philo->nb_philos + 1, sizeof(pid_t));
	if (!*pids)
		return (false);
	close_semaphores(philo);
	if (!open_semaphores(philo))
		return (free(*pids), false);
	return (true);
}
