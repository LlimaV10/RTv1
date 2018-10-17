/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 16:59:40 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/10/17 16:59:48 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../rtv1.h"

int		check4err(int argc, char **argv)
{
	int		fd;

	if (argc != 2)
	{
		write(1, "usage: RTv1 map_name\n", 21);
		return (-1);
	}
	if ((fd = open(argv[1], O_DIRECTORY)) > 0)
	{
		write(1, "Error\n", 6);
		return (-1);
	}
	close(fd);
	if ((fd = open(argv[1], O_RDONLY)) < 0)
		write(1, "Error\n", 6);
	return (fd);
}

void	main_def(t_rtv1 *iw)
{
	iw->s.o = 0;
	iw->s.l = 0;
	iw->s.light_count = 0;
	iw->s.obj_count = 0;
}

int		main(int argc, char **argv)
{
	t_rtv1	iw;
	int		fd;

	if ((fd = check4err(argc, argv)) < 0)
		return (0);
	main_def(&iw);
	if (get_map(&iw, fd) == 0)
		write(1, "Error\n", 6);
	else
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		iw.win = SDL_CreateWindow("RTv1", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
		iw.sur = SDL_GetWindowSurface(iw.win);
		threads_draw(&iw);
		SDL_UpdateWindowSurface(iw.win);
		main_loop(&iw);
		SDL_FreeSurface(iw.sur);
		SDL_DestroyWindow(iw.win);
		SDL_Quit();
	}
	free_objects_lights(&iw);
	return (0);
}
