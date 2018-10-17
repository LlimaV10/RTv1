/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 15:59:02 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/10/17 15:59:13 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../rtv1.h"

float	put_pixel_from_scene2(t_rtv1 *iw, t_vector *d, int i)
{
	if (iw->s.o[i].type == 0)
		return (get_sphere_len(iw,
			(t_sphere *)(iw->s.o[i].obj), d, &iw->cam.v));
	else if (iw->s.o[i].type == 1)
		return (get_plane_len(iw,
			(t_plane *)(iw->s.o[i].obj), d, &iw->cam.v));
	else if (iw->s.o[i].type == 2)
		return (get_cylinder_len(iw,
			(t_cylinder *)(iw->s.o[i].obj), d, &iw->cam.v));
	else if (iw->s.o[i].type == 3)
		return (get_cone_len(iw, (t_cone *)(iw->s.o[i].obj), d, &iw->cam.v));
	return (-1.0f);
}

void	put_pixel_from_scene_get_d(t_rtv1 *iw, t_vector *d)
{
	float	minlen;

	d->x = (float)((iw->i - WINDOW_W / 2) * CFD) / (float)WINDOW_W;
	d->y = (float)((iw->j - WINDOW_H / 2) * CFD) / (float)WINDOW_H;
	d->z = (float)(CAMERA_FOV_DISTANCE);
	multiply_vector_and_rotation_matrix(iw, d);
	minlen = sqrtf(powf(d->x, 2.0f) + powf(d->y, 2.0f) + powf(d->z, 2.0f));
	d->x /= minlen;
	d->y /= minlen;
	d->z /= minlen;
}

void	put_pixel_from_scene(t_rtv1 *iw)
{
	int			i;
	int			color;
	float		minlen;
	float		len;
	t_vector	d;

	put_pixel_from_scene_get_d(iw, &d);
	color = 0;
	minlen = -1.0f;
	i = -1;
	while (++i < iw->s.obj_count)
	{
		len = put_pixel_from_scene2(iw, &d, i);
		if (len > 0.0f)
			if (len < minlen || minlen < 0.0f)
			{
				minlen = len;
				color = get_light_color(iw,
					iw->s.o[i].color, i);
			}
	}
	set_pixel(iw->sur, iw->i, iw->j, color);
}

int		threads_draw2(t_rtv1 *iw)
{
	iw->i = iw->st - 1;
	while (++iw->i < iw->end)
	{
		iw->j = -1;
		while (++iw->j < WINDOW_H)
			put_pixel_from_scene(iw);
	}
	return (0);
}

void	threads_draw(t_rtv1 *iw)
{
	pthread_t	threads[THREADS];
	t_rtv1		iws[THREADS];
	int			i;
	int			j;

	i = -1;
	j = 0;
	while (++i < THREADS)
	{
		iws[i] = *iw;
		iws[i].st = j;
		j += WINDOW_W / THREADS;
		iws[i].end = j;
		pthread_create(&threads[i], NULL,
			(void *(*)(void *))threads_draw2, (void *)&iws[i]);
	}
	while (i-- > 0)
		pthread_join(threads[i], (void **)&iws[i]);
}
