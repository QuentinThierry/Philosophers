/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 18:22:48 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/26 19:33:34 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	try_to_eat(t_philo *philo)
{
	try_take_forks(philo);
	if (philo->has_l_fork && philo->has_r_fork)
	{
		philo->state = eating;
		philo->begin_eat = get_timestamp(*philo->start_time);
		print_event(philo, "is eating");
	}
}

bool	has_all_eaten(t_philo *philos)
{
	size_t	i;

	i = 0;
	while (i < philos->nb_philos)
	{
		if (philos[i].eat_to_end > 0)
			return (false);
		i++;
	}
	return (true);
}
