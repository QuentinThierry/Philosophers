/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 19:47:09 by qthierry          #+#    #+#             */
/*   Updated: 2023/07/29 16:14:32 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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

static int	end_checker(t_philo philo, pid_t *pids)
{
	int		i;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (printf("Error creating fork\n"), sem_post(philo.sem_end), 0);
	if (pid == 0)
	{
		free(pids);
		i = 0;
		while (i < philo.nb_philos)
		{
			sem_wait(philo.sem_eat_to_end);
			i++;
		}
		sem_post(philo.sem_end);
		sem_close(philo.sem_forks);
		sem_close(philo.sem_print);
		sem_close(philo.sem_eat_to_end);
		sem_close(philo.sem_end);
		return (1);
	}
	pids[philo.nb_philos] = pid;
	return (0);
}

static void	kill_all_philos(t_philo philo, pid_t *pids)
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

bool	exec_process(t_philo philo, int value, pid_t *pids, t_timeval time)
{
	if (value > 0)
	{
		free(pids);
		philo.start_time = time;
		philo.last_meal = get_timestamp(time);
		if (philo.id % 2 == 0)
			usleep(5000);
		philo_routine(philo);
	}
	else
	{
		if (end_checker(philo, pids))
			return (false);
		sem_wait(philo.sem_end);
		kill_all_philos(philo, pids);
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
	if (!exec_process(philo, value, pids, time))
		return (0);
	while (waitpid(-1, NULL, 0) > 0)
		;
	close_semaphores(&philo);
	return (0);
}
