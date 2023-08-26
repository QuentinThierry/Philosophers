/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:47:09 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/26 19:30:04 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

// returns -1 for error, 0 for main and 1 for child
static int	create_philos_processes(int nb_philos, pid_t *pids, t_philo *philo)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < nb_philos)
	{
		pid = fork();
		if (pid < 0)
			return (printf("Error creating fork\n"), -1);
		if (pid == 0)
		{
			philo->id = i + 1;
			return (1);
		}
		pids[i] = pid;
		i++;
	}
	return (0);
}

static bool	create_thread(t_philo *philo)
{
	if (pthread_create(&philo->thread, NULL, philo_thread_routine, philo) != 0)
		return (printf("Error on creating thread\n"), false);
	pthread_detach(philo->thread);
	return (true);
}

void	wait_eat_to_end(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->nb_philos)
	{
		sem_wait(philo->sem_nb_eat_to_end);
		i++;
	}
	sem_post(philo->sem_end);
}

bool	exec_process(t_philo *philo, int value, pid_t *pids, t_timeval time)
{
	if (value > 0)
	{
		free(pids);
		philo->origin_time = time;
		philo->last_meal = get_timestamp(time);
		if (!create_semaphore_threads(philo))
			return (false);
		(create_thread(philo), philo_routine(philo));
	}
	else
	{
		if (philo->eat_to_end > -2)
		{
			value = fork();
			if (value == -1)
				return (free(pids), false);
			if (value == 0)
				return (free(pids), wait_eat_to_end(philo), false);
		}
		sem_wait(philo->sem_end);
		kill_all_philos(*philo, pids);
		if (value != 0 && philo->eat_to_end > -2)
			kill(value, SIGKILL);
		free(pids);
	}
	return (true);
}

int	main(int argc, char **argv)
{
	pid_t		*pids;
	t_philo		philo;
	int			value;
	t_timeval	time;

	if (!init(argc, argv, &philo, &pids))
		return (1);
	gettimeofday(&time, NULL);
	value = create_philos_processes(philo.nb_philos, pids, &philo);
	if (value < 0)
		return (kill_all_philos(philo, pids), free(pids), 0);
	if (!exec_process(&philo, value, pids, time))
		return (0);
	while (waitpid(-1, NULL, 0) > 0)
		;
	return (0);
}
