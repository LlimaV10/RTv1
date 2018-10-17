/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_loop.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 16:54:49 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/10/17 16:54:59 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../rtv1.h"

void	key_down2(int code, t_rtv1 *iw)
{
	if (code == 4)
		update_vector(iw, 0, -3.0f);
	else if (code == 20)
		update_int(iw, &iw->cam.v.y, 2);
	else if (code == 8)
		update_int(iw, &iw->cam.v.y, -2);
	else if (code == 56)
		update_move_selected_x(iw, 1);
	else if (code == 54)
		update_move_selected_x(iw, -1);
	else if (code == 15)
		update_move_selected_y(iw, -1);
	else if (code == 55)
		update_move_selected_y(iw, 1);
	else if (code == 14)
		update_move_selected_z(iw, 1);
	else if (code == 51)
		update_move_selected_z(iw, -1);
}

void	key_down(int code, t_rtv1 *iw)
{
	if (code == 41)
		exit_x(iw);
	else if (code == 80)
		update_float(iw, &iw->cam.ry, -0.0698132f);
	else if (code == 79)
		update_float(iw, &iw->cam.ry, 0.0698132f);
	else if (code == 81)
		update_float(iw, &iw->cam.rx, -0.0698132f);
	else if (code == 82)
		update_float(iw, &iw->cam.rx, 0.0698132f);
	else if (code == 26)
		update_vector(iw, 1, 3.0f);
	else if (code == 22)
		update_vector(iw, 1, -3.0f);
	else if (code == 7)
		update_vector(iw, 0, 3.0f);
	else
		key_down2(code, iw);
}

int		select_object2(t_rtv1 *iw, int i, t_vector *d)
{
	if (iw->s.o[i].type == 0)
		return (get_sphere_len(iw, (t_sphere *)(iw->s.o[i].obj),
			d, &iw->cam.v));
	else if (iw->s.o[i].type == 1)
		return (get_plane_len(iw, (t_plane *)(iw->s.o[i].obj), d, &iw->cam.v));
	else if (iw->s.o[i].type == 2)
		return (get_cylinder_len(iw, (t_cylinder *)(iw->s.o[i].obj),
			d, &iw->cam.v));
	else if (iw->s.o[i].type == 3)
		return (get_cone_len(iw, (t_cone *)(iw->s.o[i].obj), d, &iw->cam.v));
	return (-1.0f);
}

void	select_object(t_rtv1 *iw, int x, int y)
{
	int			i;
	int			obj;
	float		minlen;
	float		len;
	t_vector	d;

	d.x = (float)((x - WINDOW_W / 2) * CAMERA_FOV_DISTANCE) / (float)WINDOW_W;
	d.y = (float)((y - WINDOW_H / 2) * CAMERA_FOV_DISTANCE) / (float)WINDOW_H;
	d.z = (float)(CAMERA_FOV_DISTANCE);
	multiply_vector_and_rotation_matrix(iw, &d);
	obj = -1;
	minlen = -1.0f;
	i = -1;
	while (++i < iw->s.obj_count)
	{
		len = select_object2(iw, i, &d);
		if (len > 0.0f)
			if (len < minlen || minlen < 0.0f)
			{
				minlen = len;
				obj = i;
			}
	}
	if (obj >= 0)
		iw->f.sel = obj;
}

void	main_loop(t_rtv1 *iw)
{
	SDL_Event e;

	iw->quit = 0;
	while (!iw->quit)
	{
		while (SDL_PollEvent(&e) != 0)
			if (e.type == SDL_QUIT)
				iw->quit = 1;
			else if (e.type == SDL_KEYDOWN)
				key_down(e.key.keysym.scancode, iw);
			else if (e.type == SDL_MOUSEBUTTONDOWN)
				select_object(iw, e.button.x, e.button.y);
	}
}
