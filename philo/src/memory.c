/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:20:52 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/24 18:21:16 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static void	free_forks(t_fork *forks, size_t nb_philos)
{
	size_t	i;

	i = 0;
	while (i < nb_philos)
	{
		pthread_mutex_destroy(&forks[i].mut);
		i++;
	}
	free(forks);
}

void	free_philos(t_philo *philos)
{
	if (!philos)
		return ;
	free(philos->is_end);
	pthread_mutex_destroy(philos->mut_end);
	free(philos->mut_end);
	pthread_mutex_destroy(philos->mut_print);
	free(philos->mut_print);
	pthread_mutex_destroy(philos->mut_eat_end);
	free(philos->mut_eat_end);
	free_forks(philos->forks, philos->nb_philos);
	free(philos);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*res;
	size_t	i;

	if (!nmemb || !size)
		return (malloc(0));
	if ((size_t)-1 / nmemb < size)
		return (NULL);
	res = malloc(nmemb * size);
	if (!res)
		return (res);
	i = 0;
	while (i < (size * nmemb))
		((char *)res)[i++] = 0;
	return (res);
}
