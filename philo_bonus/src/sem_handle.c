/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sem_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 19:32:26 by qthierry          #+#    #+#             */
/*   Updated: 2023/08/11 19:41:38 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	create_semaphore_threads(t_philo *philo)
{
	const char	*error = "Error opening semaphore";
	char		*name;

	name = ft_strjoin("/philo_last_meal_", ft_itoa(philo->id));
	if (!name)
		return (close_semaphores(philo), printf("%s\n", error), false);
	philo->sem_last_meal = sem_open(name, O_CREAT, 0664, 1);
	free(name);
	if (philo->sem_last_meal == SEM_FAILED)
		return (close_semaphores(philo), printf("%s\n", error), false);
	return (true);
}

void	destroy_semaphore_threads(t_philo *philo)
{
	const char	*error = "Error closing semaphore";
	char		*name;
	char		*tmp;
	int			i;

	i = 1;
	while (i <= philo->nb_philos)
	{
		tmp = ft_itoa(i);
		name = ft_strjoin("philo_last_meal_", tmp);
		free(tmp);
		if (!name)
			return (close_semaphores(philo), (void)printf("%s\n", error));
		sem_unlink(name);
		free(name);
		i++;
	}
}

void	close_semaphores(t_philo *philo)
{
	if (philo)
	{
		if (philo->sem_end)
			sem_close(philo->sem_end);
		if (philo->sem_forks)
			sem_close(philo->sem_forks);
		if (philo->sem_print)
			sem_close(philo->sem_print);
		if (philo->sem_nb_eat_to_end)
			sem_close(philo->sem_nb_eat_to_end);
		if (philo->sem_last_meal)
			sem_close(philo->sem_last_meal);
		destroy_semaphore_threads(philo);
	}
	sem_unlink("/philo_end");
	sem_unlink("/philo_forks");
	sem_unlink("/philo_print");
	sem_unlink("/philo_nb_eat_to_end");
	sem_unlink("/philo_eat_to_end");
	sem_unlink("/philo_last_meal");
}

bool	open_semaphores(t_philo *philo)
{
	const char	*error = "Error opening semaphore";

	philo->sem_forks
		= sem_open("/philo_forks", O_CREAT, 0664, philo->nb_philos);
	if (philo->sem_forks == SEM_FAILED)
		return (close_semaphores(philo), printf("%s\n", error), false);
	philo->sem_end = sem_open("/philo_end", O_CREAT, 0664, 0);
	if (philo->sem_end == SEM_FAILED)
		return (close_semaphores(philo), printf("%s\n", error), false);
	philo->sem_print = sem_open("/philo_print", O_CREAT, 0664, 1);
	if (philo->sem_print == SEM_FAILED)
		return (close_semaphores(philo), printf("%s\n", error), false);
	philo->sem_nb_eat_to_end
		= sem_open("/philo_nb_eat_to_end", O_CREAT, 0664, 0);
	if (philo->sem_nb_eat_to_end == SEM_FAILED)
		return (close_semaphores(philo), printf("%s\n", error), false);
	return (true);
}
