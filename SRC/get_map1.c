/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 13:36:10 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/10/17 13:36:44 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../rtv1.h"

int		exit_map(int fd)
{
	if (fd != 0)
		close(fd);
	return (0);
}

void	get_camera_def(t_rtv1 *iw)
{
	iw->cam.v.x = 0;
	iw->cam.v.y = 0;
	iw->cam.v.z = 0;
	iw->cam.rx = 0.0f;
	iw->cam.ry = 0.0f;
	iw->cam.light = 0.1f;
}

int		get_camera(t_rtv1 *iw, int fd, t_get_map *t)
{
	if (ft_strcmp(*t->spl, "camera:") == 0)
	{
		if (t->spl_len != 7)
			return (free_spl(t->spl, t->s, fd));
		iw->cam.v.x = ft_atoi(t->spl[1]);
		iw->cam.v.y = ft_atoi(t->spl[2]);
		iw->cam.v.z = ft_atoi(t->spl[3]);
		iw->cam.rx = (float)ft_atoi(t->spl[4]) * 0.0174533f;
		iw->cam.ry = (float)ft_atoi(t->spl[5]) * 0.0174533f;
		iw->cam.light = (float)abs(ft_atoi(t->spl[6])) / 1000.0f;
		free_spl(t->spl, t->s, 0);
		if (get_next_line(fd, &t->s) < 0)
			return (0);
		t->spl = ft_strsplit(t->s, ' ');
		t->spl_len = split_len(t->spl);
		if (t->spl_len < 1)
			return (free_spl(t->spl, t->s, fd));
	}
	else
		get_camera_def(iw);
	return (1);
}

int		get_map_objects(t_rtv1 *iw, int fd, t_get_map *t)
{
	if ((ft_strcmp(*t->spl, "objects:") == 0) &&
		(t->spl_len == 2) && (get_objects(fd, ft_atoi(t->spl[1]), iw) != 0))
	{
		free_spl(t->spl, t->s, 0);
		if (get_next_line(fd, &t->s) < 0)
		{
			iw->s.light_count = 0;
			return (1);
		}
		t->spl = ft_strsplit(t->s, ' ');
		t->spl_len = split_len(t->spl);
		if (t->spl_len != 2 || ft_strcmp("lights:", t->spl[0]) != 0
			|| get_lights(fd, ft_atoi(t->spl[1]), iw) == 0)
			return (free_spl(t->spl, t->s, fd));
	}
	else
		return (free_spl(t->spl, t->s, fd));
	return (1);
}

int		get_map(t_rtv1 *iw, int fd)
{
	t_get_map	t;

	iw->f.sel = 0;
	if (get_next_line(fd, &t.s) < 0)
		return (exit_map(fd));
	t.spl = ft_strsplit(t.s, ' ');
	t.spl_len = split_len(t.spl);
	if (t.spl_len < 1)
		return (free_spl(t.spl, t.s, fd));
	if (!get_camera(iw, fd, &t))
		return (0);
	if (!get_map_objects(iw, fd, &t))
		return (0);
	free_spl(t.spl, t.s, fd);
	return (1);
}
