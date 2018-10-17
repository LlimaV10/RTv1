/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 16:58:15 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/10/17 16:58:25 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../rtv1.h"

void	set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint8	*target_pixel;

	if (x >= 0 && x < WINDOW_W)
	{
		target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
		*(Uint32 *)target_pixel = pixel;
	}
}

void	free_objects_lights(t_rtv1 *iw)
{
	int		i;

	if (iw->s.o)
	{
		i = -1;
		while (++i < iw->s.obj_count)
			free(iw->s.o[i].obj);
		free(iw->s.o);
	}
	if (iw->s.l)
		free(iw->s.l);
}

void	exit_x(t_rtv1 *iw)
{
	free_objects_lights(iw);
	SDL_FreeSurface(iw->sur);
	SDL_DestroyWindow(iw->win);
	SDL_Quit();
	exit(0);
}

void	update_int(t_rtv1 *iw, int *change, int i)
{
	*change += i;
	threads_draw(iw);
	SDL_UpdateWindowSurface(iw->win);
}
