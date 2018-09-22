#include "rtv1.h"
#include <stdio.h>
void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	if (x >= 0 && x < WINDOW_W)
	{
		Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * 4;
		*(Uint32 *)target_pixel = pixel;
	}
}

void	exit_x(t_rtv1 *iw)
{
	SDL_FreeSurface(iw->sur);
	SDL_DestroyWindow(iw->win);
	SDL_Quit();
	system("leaks RTv1");
	exit(0);
}

void	draw(t_rtv1 *iw);
void	multiply_vector_and_rotation_matrix(t_rtv1 *iw, t_vector *v);

void	key_down(int code, t_rtv1 *iw)
{
	//printf("keydown = %d\n", code);
	if (code == 41)
		exit_x(iw);
	else if (code == 80)
	{
		iw->cam.ry -= 0.0349066f;
		draw(iw);
		SDL_UpdateWindowSurface(iw->win);
	}
	else if (code == 79)
	{
		iw->cam.ry += 0.0349066f;
		draw(iw);
		SDL_UpdateWindowSurface(iw->win);
	}
	else if (code == 81)
	{
		iw->cam.rx -= 0.0349066f;
		draw(iw);
		SDL_UpdateWindowSurface(iw->win);
	}
	else if (code == 82)
	{
		iw->cam.rx += 0.0349066f;
		draw(iw);
		SDL_UpdateWindowSurface(iw->win);
	}
	else if (code == 26)
	{
		t_vector	v;
		v.x = 0.0f;
		v.y = 0.0f;
		v.z = 1.0f;
		multiply_vector_and_rotation_matrix(iw, &v);
		iw->cam.x += (int)(3.0f * v.x);
		iw->cam.y += (int)(3.0f * v.y);
		iw->cam.z += (int)(3.0f * v.z);
		draw(iw);
		SDL_UpdateWindowSurface(iw->win);
	}
	else if (code == 22)
	{
		t_vector	v;
		v.x = 0.0f;
		v.y = 0.0f;
		v.z = -1.0f;
		multiply_vector_and_rotation_matrix(iw, &v);
		iw->cam.x += (int)(3.0f * v.x);
		iw->cam.y += (int)(3.0f * v.y);
		iw->cam.z += (int)(3.0f * v.z);
		draw(iw);
		SDL_UpdateWindowSurface(iw->win);
	}
	else if (code == 7)
	{
		t_vector	v;
		v.x = 1.0f;
		v.y = 0.0f;
		v.z = 0.0f;
		multiply_vector_and_rotation_matrix(iw, &v);
		iw->cam.x += (int)(3.0f * v.x);
		iw->cam.y += (int)(3.0f * v.y);
		iw->cam.z += (int)(3.0f * v.z);
		draw(iw);
		SDL_UpdateWindowSurface(iw->win);
	}
	else if (code == 4)
	{
		t_vector	v;
		v.x = -1.0f;
		v.y = 0.0f;
		v.z = 0.0f;
		multiply_vector_and_rotation_matrix(iw, &v);
		iw->cam.x += (int)(3.0f * v.x);
		iw->cam.y += (int)(3.0f * v.y);
		iw->cam.z += (int)(3.0f * v.z);
		draw(iw);
		SDL_UpdateWindowSurface(iw->win);
	}
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
	t_sphere	*tmp;

	iw->cam.x = 0;
	iw->cam.y = 0;
	iw->cam.z = 0;
	iw->cam.rx = 0.174533f;
	iw->cam.ry = 0.174533f;
	//iw->cam.rz = 0;
	iw->s.obj_count = 2;
	iw->s.o = (t_objects *)malloc(iw->s.obj_count * sizeof(t_objects));
	tmp = (t_sphere *)malloc(sizeof(t_sphere));
	tmp->x = 0;
	tmp->y = 0;
	tmp->z = 50;
	tmp->r = 5;
	iw->s.o[0].obj = (void *)tmp;
	iw->s.o[0].type = 0;
	iw->s.o[0].color = 0x00FF00;

	tmp = (t_sphere *)malloc(sizeof(t_sphere));
	tmp->x = 1;
	tmp->y = 0;
	tmp->z = 60;
	tmp->r = 7;
	iw->s.o[1].obj = (void *)tmp;
	iw->s.o[1].type = 0;
	iw->s.o[1].color = 0xFF0000;
}

void	multiply_vector_and_rotation_matrix(t_rtv1 *iw, t_vector *v)
{
	float	tmp;

	tmp = v->y * cosf(iw->cam.rx) - v->z * sinf(iw->cam.rx);
	v->z = v->y * sinf(iw->cam.rx) + v->z * cosf(iw->cam.rx);
	v->y = tmp;
	tmp = v->x * cosf(iw->cam.ry) + v->z * sinf(iw->cam.ry);
	v->z = -v->x * sinf(iw->cam.ry) + v->z * cosf(iw->cam.ry);
	v->x = tmp;
	/*v->x = v->x * cosf(iw->cam.rz) - v->y * sinf(iw->cam.rz);
	v->y = v->x * sinf(iw->cam.rz) + v->y * cosf(iw->cam.rz);*/
}

float	get_sphere_len(t_rtv1 *iw, t_sphere *sphere)
{
	t_vector		d;
	t_vector		oc;
	t_variables1	k;
	float			discriminant;

	d.x = (float)((iw->i - WINDOW_W / 2) * CAMERA_FOV_DISTANCE) / (float)WINDOW_W;
	d.y = (float)((iw->j - WINDOW_H / 2) * CAMERA_FOV_DISTANCE) / (float)WINDOW_H;
	d.z = (float)(CAMERA_FOV_DISTANCE);
	multiply_vector_and_rotation_matrix(iw, &d);
	k.k1 = d.x * d.x + d.y * d.y + d.z * d.z;
	oc.x = (float)(sphere->x - iw->cam.x);
	oc.y = (float)(sphere->y - iw->cam.y);
	oc.z = (float)(sphere->z - iw->cam.z);
	//printf("ocx %f ocy %f ocz %f\n", oc.x, oc.y, oc.z);
	k.k2 = 2.0f * (oc.x * d.x + oc.y * d.y + oc.z * d.z);
	k.k3 = (oc.x * oc.x + oc.y * oc.y + oc.z * oc.z) - (float)(sphere->r * sphere->r);
	discriminant = k.k2 * k.k2 - 4.0f * k.k1 * k.k3;
	if (discriminant < 0.0f)
		return (-1.0f);
	k.t1 = (-k.k2 + sqrtf(discriminant)) / (2.0f * k.k1);
	k.t2 = (-k.k2 - sqrtf(discriminant)) / (2.0f * k.k1);
	k.p1.x = (float)iw->cam.x + k.t1 * d.x;
	k.p1.y = (float)iw->cam.y + k.t1 * d.y;
	k.p1.z = (float)iw->cam.z + k.t1 * d.z;
	k.p2.x = (float)iw->cam.x + k.t2 * d.x;
	k.p2.y = (float)iw->cam.y + k.t2 * d.y;
	k.p2.z = (float)iw->cam.z + k.t2 * d.z;
	k.len1 = sqrtf(powf((float)iw->cam.x - k.p1.x, 2.0f) + powf((float)iw->cam.y - k.p1.y, 2.0f)
		+ powf((float)iw->cam.z - k.p1.z, 2.0f));
	k.len2 = sqrtf(powf((float)iw->cam.x - k.p2.x, 2.0f) + powf((float)iw->cam.y - k.p2.y, 2.0f)
		+ powf((float)iw->cam.z - k.p2.z, 2.0f));
//	printf("kek\n");
	return ((k.len1 < k.len2) ? k.len1 : k.len2);
}

void	put_pixel_from_scene(t_rtv1 *iw)
{
	int		i;
	int		color;
	float	minlen;
	float	len;

	color = 0;
	minlen = -1.0f;
	i = -1;
	while (++i < iw->s.obj_count)
	{
		if (iw->s.o[i].type == 0)
		{
			if ((len = get_sphere_len(iw, (t_sphere *)(iw->s.o[i].obj))) > 0.0f)
			{
				//printf("spx = %f\n", ((t_sphere *)(&iw->s.o[i].obj))->x);
				if (len < minlen || minlen < 0.0f)
				{
					minlen = len;
					color = iw->s.o[i].color;
				}
			}
		}
	}
	set_pixel(iw->sur, iw->i, iw->j, color);
}

void	draw(t_rtv1 *iw)
{
	iw->i = -1;
	while (++iw->i < WINDOW_W)
	{
		iw->j = -1;
		while (++iw->j < WINDOW_H)
			put_pixel_from_scene(iw);
	}
}

void	threads_draw2(t_rtv1 *iw)
{
	iw->i = iw->st - 1;
	while (++iw->i < iw->end)
	{
		iw->j = -1;
		while (++iw->j < WINDOW_H)
			put_pixel_from_scene(iw);
	}
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

int		main(void)
{
	t_rtv1	iw;

	SDL_Init(SDL_INIT_EVERYTHING);
	//SDL_SetRelativeMouseMode(0);
	iw.win = SDL_CreateWindow("RTv1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	iw.sur = SDL_GetWindowSurface(iw.win);
	get_scene1(&iw);
	threads_draw(&iw);
	SDL_UpdateWindowSurface(iw.win);
	main_loop(&iw);
	SDL_FreeSurface(iw.sur);
	SDL_DestroyWindow(iw.win);
	SDL_Quit();
	return (0);
}