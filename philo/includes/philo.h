/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:43:55 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/23 02:18:03 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>

typedef struct timeval t_timeval;

enum	e_state
{
	eating,
	sleeping,
	thinking
};

enum	e_times
{
	t_eat,
	t_sleep,
	t_die
};

typedef struct s_fork
{
	pthread_mutex_t	mut;
	bool			is_taken;
}	t_fork;

typedef struct s_philo
{
	pthread_t		thread;
	size_t			id;
	long			times[3];
	enum e_state	state;
	bool			has_l_fork;
	bool			has_r_fork;
	bool			*is_end;
	pthread_mutex_t	*mut_end;
	t_fork			*forks;
	struct s_philo	*all_philos;
	size_t			nb_philos;
}	t_philo;

#endif