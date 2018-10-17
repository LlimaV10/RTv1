/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 16:56:23 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/10/17 16:56:31 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../rtv1.h"

void	update_float(t_rtv1 *iw, float *change, float i)
{
	*change += i;
	threads_draw(iw);
	SDL_UpdateWindowSurface(iw->win);
}

void	update_vector(t_rtv1 *iw, int bol, float i)
{
	t_vector	v;

	v.y = 0.0f;
	if (bol == 0)
	{
		v.x = i;
		v.z = 0.0f;
	}
	else
	{
		v.z = i;
		v.x = 0.0f;
	}
	multiply_vector_and_rotation_matrix(iw, &v);
	iw->cam.v.x += (int)(v.x);
	iw->cam.v.y += (int)(v.y);
	iw->cam.v.z += (int)(v.z);
	threads_draw(iw);
	SDL_UpdateWindowSurface(iw->win);
}

void	update_move_selected_y(t_rtv1 *iw, int i)
{
	if (iw->s.o[iw->f.sel].type == 0)
		update_int(iw, &(((t_sphere *)iw->s.o[iw->f.sel].obj)->y), i);
	else if (iw->s.o[iw->f.sel].type == 1)
		update_int(iw, &(((t_plane *)iw->s.o[iw->f.sel].obj)->p0.y), i);
	else if (iw->s.o[iw->f.sel].type == 2)
		update_int(iw, &(((t_cylinder *)iw->s.o[iw->f.sel].obj)->c.y), i);
	else if (iw->s.o[iw->f.sel].type == 3)
		update_int(iw, &(((t_cone *)iw->s.o[iw->f.sel].obj)->c.y), i);
}

void	update_move_selected_x(t_rtv1 *iw, int i)
{
	if (iw->s.o[iw->f.sel].type == 0)
		update_int(iw, &(((t_sphere *)iw->s.o[iw->f.sel].obj)->x), i);
	else if (iw->s.o[iw->f.sel].type == 1)
		update_int(iw, &(((t_plane *)iw->s.o[iw->f.sel].obj)->p0.x), i);
	else if (iw->s.o[iw->f.sel].type == 2)
		update_int(iw, &(((t_cylinder *)iw->s.o[iw->f.sel].obj)->c.x), i);
	else if (iw->s.o[iw->f.sel].type == 3)
		update_int(iw, &(((t_cone *)iw->s.o[iw->f.sel].obj)->c.x), i);
}

void	update_move_selected_z(t_rtv1 *iw, int i)
{
	if (iw->s.o[iw->f.sel].type == 0)
		update_int(iw, &(((t_sphere *)iw->s.o[iw->f.sel].obj)->z), i);
	else if (iw->s.o[iw->f.sel].type == 1)
		update_int(iw, &(((t_plane *)iw->s.o[iw->f.sel].obj)->p0.z), i);
	else if (iw->s.o[iw->f.sel].type == 2)
		update_int(iw, &(((t_cylinder *)iw->s.o[iw->f.sel].obj)->c.z), i);
	else if (iw->s.o[iw->f.sel].type == 3)
		update_int(iw, &(((t_cone *)iw->s.o[iw->f.sel].obj)->c.z), i);
}
