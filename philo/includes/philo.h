/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 00:43:55 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/22 02:24:34 by qthierry         ###   ########.fr       */
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

typedef struct s_main
{
	bool	is_end;
	int		*nb_meals;
}	t_main;

typedef struct s_philo
{
	pthread_t		thread;
	size_t			id;
	long			times[3];
	enum e_state	state;
	bool			has_l_fork;
	bool			has_r_fork;
	bool			is_dead;
}	t_philo;

#endif