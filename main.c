#include "rtv1.h"

void	exit_x(t_rtv1 *iw)
{
	SDL_FreeSurface(iw->sur);
	SDL_DestroyWindow(iw->win);
	SDL_Quit();
	system("leaks RTv1");
	exit(0);
}

void	key_down(int code, t_rtv1 *iw)
{
	//printf("keydown = %d\n", code);
	if (code == 41)
		exit_x(iw);
}

void	loop(t_rtv1 *iw)
{

}

void	key_up(int code, t_rtv1 *iw)
{
	//printf("keyup = %d\n", code);
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
			else if (e.type == SDL_KEYDOWN)//&& e.key.repeat == 0)
				key_down(e.key.keysym.scancode, iw);
			else if (e.type == SDL_KEYUP)
				key_up(e.key.keysym.scancode, iw);
			/*else if (e.type == SDL_MOUSEMOTION)
				mouse_move(e.motion.xrel, e.motion.yrel, iw);*/
		loop(iw);
	}
}

void	get_scene1(t_rtv1 *iw)
{
	iw->cam.x = 0;
	iw->cam.y = 0;
	iw->cam.z = 0;
	iw->cam.rx = 0;
	iw->cam.ry = 0;
	iw->cam.rz = 0;
}

void	get

int		main(void)
{
	t_rtv1	iw;

	SDL_Init(SDL_INIT_EVERYTHING);
	//SDL_SetRelativeMouseMode(0);
	iw.win = SDL_CreateWindow("RTv1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	iw.sur = SDL_GetWindowSurface(iw.win);
	main_loop(&iw);
	SDL_FreeSurface(iw.sur);
	SDL_DestroyWindow(iw.win);
	SDL_Quit();
	return (0);
}