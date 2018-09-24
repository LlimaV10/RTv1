#include "rtv1.h"
#include <stdio.h>

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
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

	i = -1;
	while (++i < iw->s.obj_count)
		free(iw->s.o[i].obj);
	free(iw->s.o);
	free(iw->s.l);
}

void	exit_x(t_rtv1 *iw)
{
	free_objects_lights(iw);
	SDL_FreeSurface(iw->sur);
	SDL_DestroyWindow(iw->win);
	SDL_Quit();
	system("leaks RTv1");
	exit(0);
}

void	draw(t_rtv1 *iw);
void	multiply_vector_and_rotation_matrix(t_rtv1 *iw, t_vector *v);
void	threads_draw(t_rtv1 *iw);

void	update_int(t_rtv1 *iw, int *change, int i)
{
	*change += i;
	threads_draw(iw);
	SDL_UpdateWindowSurface(iw->win);
}

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

void	key_down(int code, t_rtv1 *iw)
{
	printf("keydown = %d\n", code);
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
	else if (code == 4)
		update_vector(iw, 0, -3.0f);
	else if (code == 20)
		update_int(iw, &iw->cam.v.y, 2);
	else if (code == 8)
		update_int(iw, &iw->cam.v.y, -2);
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
	t_sphere		*tmp;
	t_plane			*tmp2;

	iw->cam.v.x = 0;
	iw->cam.v.y = 0;
	iw->cam.v.z = 0;
	iw->cam.rx = 0.0f;
	iw->cam.ry = 0.0f;
	iw->cam.light = 0.1f;
	//iw->cam.rz = 0;
	iw->s.obj_count = 4;
	iw->s.o = (t_objects *)malloc(iw->s.obj_count * sizeof(t_objects));
	tmp = (t_sphere *)malloc(sizeof(t_sphere));
	tmp->x = 0;
	tmp->y = -10;
	tmp->z = 40;
	tmp->r = 7;
	iw->s.o[0].obj = (void *)tmp;
	iw->s.o[0].type = 0;
	iw->s.o[0].color = 0x00FF00;

	tmp = (t_sphere *)malloc(sizeof(t_sphere));
	tmp->x = -12;
	tmp->y = -10;
	tmp->z = 40;
	tmp->r = 5;
	iw->s.o[1].obj = (void *)tmp;
	iw->s.o[1].type = 0;
	iw->s.o[1].color = 0xFF0000;

	tmp2 = (t_plane *)malloc(sizeof(t_plane));
	tmp2->p0.x = 0;
	tmp2->p0.y = 25;
	tmp2->p0.z = 0;
	tmp2->n.x = 0;
	tmp2->n.y = -1;
	tmp2->n.z = 0;
	iw->s.o[2].obj = (void *)tmp2;
	iw->s.o[2].type = 1;
	iw->s.o[2].color = 0xFFFF00;

	tmp2 = (t_plane *)malloc(sizeof(t_plane));
	tmp2->p0.x = 0;
	tmp2->p0.y = 0;
	tmp2->p0.z = 70;
	tmp2->n.x = 0;
	tmp2->n.y = 0;
	tmp2->n.z = 1;
	iw->s.o[3].obj = (void *)tmp2;
	iw->s.o[3].type = 1;
	iw->s.o[3].color = 0x0000FF;

	/*tmp2 = (t_plane *)malloc(sizeof(t_plane));
	tmp2->p0.x = -80;
	tmp2->p0.y = 0;
	tmp2->p0.z = 0;
	tmp2->n.x = -1;
	tmp2->n.y = 0;
	tmp2->n.z = 0;
	iw->s.o[4].obj = (void *)tmp2;
	iw->s.o[4].type = 1;
	iw->s.o[4].color = 0x0000FF;

	tmp2 = (t_plane *)malloc(sizeof(t_plane));
	tmp2->p0.x = 0;
	tmp2->p0.y = 80;
	tmp2->p0.z = 0;
	tmp2->n.x = 0;
	tmp2->n.y = 1;
	tmp2->n.z = 0;
	iw->s.o[5].obj = (void *)tmp2;
	iw->s.o[5].type = 1;
	iw->s.o[5].color = 0x00FFFF;

	tmp2 = (t_plane *)malloc(sizeof(t_plane));
	tmp2->p0.x = 0;
	tmp2->p0.y = -80;
	tmp2->p0.z = 0;
	tmp2->n.x = 0;
	tmp2->n.y = -1;
	tmp2->n.z = 0;
	iw->s.o[6].obj = (void *)tmp2;
	iw->s.o[6].type = 1;
	iw->s.o[6].color = 0x00FFFF;

	tmp2 = (t_plane *)malloc(sizeof(t_plane));
	tmp2->p0.x = 0;
	tmp2->p0.y = 0;
	tmp2->p0.z = -200;
	tmp2->n.x = 0;
	tmp2->n.y = 0;
	tmp2->n.z = -1;
	iw->s.o[7].obj = (void *)tmp2;
	iw->s.o[7].type = 1;
	iw->s.o[7].color = 0xFFFF00;*/

	iw->s.light_count = 2;
	iw->s.l = (t_point_light *)malloc(iw->s.light_count * sizeof(t_point_light));
	iw->s.l[0].v.x = 25;
	iw->s.l[0].v.y = -25;
	iw->s.l[0].v.z = 35;
	iw->s.l[0].strength = 0.9f;

	iw->s.l[1].v.x = -25;
	iw->s.l[1].v.y = -25;
	iw->s.l[1].v.z = 35;
	iw->s.l[1].strength = 0.9f;
	/*iw->s.l[1].v.x = -20;
	iw->s.l[1].v.y = 0;
	iw->s.l[1].v.z = 50;
	iw->s.l[1].strength = 1.0f;*/
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

float	get_sphere_len2(t_rtv1 *iw, t_variables1 *k, t_vector *d, t_int_vector *v)
{
	k->p1.x = ((float)v->x + k->t1 * d->x);
	k->p1.y = (float)v->y + k->t1 * d->y;
	k->p1.z = ((float)v->z + k->t1 * d->z);
	k->p2.x = ((float)v->x + k->t2 * d->x);
	k->p2.y = (float)v->y + k->t2 * d->y;
	k->p2.z = ((float)v->z + k->t2 * d->z);
	k->len1 = sqrtf(powf((float)v->x - k->p1.x, 2.0f) + powf((float)v->y - k->p1.y, 2.0f)
		+ powf((float)v->z - k->p1.z, 2.0f));
	k->len2 = sqrtf(powf((float)v->x - k->p2.x, 2.0f) + powf((float)v->y - k->p2.y, 2.0f)
		+ powf((float)v->z - k->p2.z, 2.0f));
}

float	get_sphere_len(t_rtv1 *iw, t_sphere *sphere, t_vector *d, t_int_vector *v)
{
	t_vector		oc;
	t_variables1	k;
	float			discriminant;

	k.k1 = d->x * d->x + d->y * d->y + d->z * d->z;
	oc.x = (float)(-sphere->x + v->x);
	oc.y = (float)(-sphere->y + v->y);
	oc.z = (float)(-sphere->z + v->z);
	k.k2 = 2.0f * (oc.x * d->x + oc.y * d->y + oc.z * d->z);
	k.k3 = (oc.x * oc.x + oc.y * oc.y + oc.z * oc.z) - (float)(sphere->r * sphere->r);
	discriminant = k.k2 * k.k2 - 4.0f * k.k1 * k.k3;
	if (discriminant < 0.0f)
		return (-1.0f);
	k.t1 = (-k.k2 + sqrtf(discriminant)) / (2.0f * k.k1);
	k.t2 = (-k.k2 - sqrtf(discriminant)) / (2.0f * k.k1);
	if (k.t1 < 1.0f || k.t2 < 1.0f)
		return (-1.0f);
	get_sphere_len2(iw, &k, d, v);
	iw->p = (k.len1 < k.len2) ? k.p1 : k.p2;
	/*iw->p.x = -iw->p.x + (float)(iw->cam.v.x * 2);
	iw->p.y = iw->p.y - (float)(iw->cam.v.y);
	iw->p.z = -iw->p.z + (float)(iw->cam.v.z * 2);*/
	return ((k.len1 < k.len2) ? k.len1 : k.len2);
}

float	get_plane_len(t_rtv1 *iw, t_plane *plane, t_vector *d, t_int_vector *v)
{
	float			t;
	t_int_vector	p0l0;

	p0l0.x = -plane->p0.x + v->x;
	p0l0.y = -plane->p0.y + v->y;
	p0l0.z = -plane->p0.z + v->z;
	t = (float)(-(p0l0.x * plane->n.x + p0l0.y * plane->n.y + p0l0.z * plane->n.z)) /
		(float)(d->x * (float)plane->n.x + d->y * (float)plane->n.y + d->z * (float)plane->n.z);
	if (t < 0)
		return (-1.0f);
	iw->p.x = (float)v->x + d->x * t;
	iw->p.y = (float)v->y + d->y * t;
	iw->p.z = (float)v->z + d->z * t;
	return (sqrtf(powf((float)v->x - iw->p.x, 2.0f) + powf((float)v->y - iw->p.y, 2.0f)
		+ powf((float)v->z - iw->p.z, 2.0f)));
	//printf("pt.x = (float)((plane->p0.x - v->x) * plane->n.x) / (d->x * (float)plane->n.x);
	// t.y = (float)((plane->p0.y - v->y) * plane->n.y) / (d->y * (float)plane->n.y);
	// t.z = (float)((plane->p0.z - v->z) * plane->n.z) / (d->z * (float)plane->n.z);
	// //printf("tx %f ty %f tz %f\n", t.x, t.y, t.z);
	// iw->p.x = (float)v->x + d->x * t.x;
	// iw->p.y = (float)v->y + d->y * t.y;
	// iw->p.z = (float)v->z + d->z * t.z;0x %d p0y %d p0z %d\n", plane->p0.x, plane->p0.y, plane->p0.z);
	// 
	//printf("px %f py %f pz %f\n", iw->p.x, iw->p.y, iw->p.z);
	//printf("%f\n", sqrtf(powf((float)v->x - iw->p.x, 2.0f) + powf((float)v->y - iw->p.y, 2.0f)
	// 	//+powf((float)v->z - iw->p.z, 2.0f)));
	// return (sqrtf(powf((float)v->x - iw->p.x, 2.0f) + powf((float)v->y - iw->p.y, 2.0f)
	// 	+powf((float)v->z - iw->p.z, 2.0f)));
}

int		get_light_object(t_rtv1 *iw, int light)
{
	int			i;
	int			obj;
	float		minlen;
	float		len;
	t_vector	d;

	minlen = sqrtf(powf(iw->p.x - (float)iw->s.l[light].v.x, 2.0f) +
		powf(iw->p.y - (float)iw->s.l[light].v.y, 2.0f) +
		powf(iw->p.z - (float)iw->s.l[light].v.z, 2.0f));
	d.x = (iw->p.x - (float)iw->s.l[light].v.x) / minlen / 2.0f;
	d.y = (iw->p.y - (float)iw->s.l[light].v.y) / minlen / 2.0f;
	d.z = (iw->p.z - (float)iw->s.l[light].v.z) / minlen / 2.0f;
	obj = -1;
	minlen = -1.0f;
	i = -1;
	while (++i < iw->s.obj_count)
	{
		if (iw->s.o[i].type == 0)
		{
			if ((len = get_sphere_len(iw, (t_sphere *)(iw->s.o[i].obj), &d, &(iw->s.l[light].v))) > 0.0f)
				if (len < minlen || minlen < 0.0f)
				{
					minlen = len;
					obj = i;
				}
		}
		else if (iw->s.o[i].type == 1)
		{
			if ((len = get_plane_len(iw, (t_plane *)(iw->s.o[i].obj), &d, &(iw->s.l[light].v))) > 0.0f)
				if (len < minlen || minlen < 0.0f)
				{
					minlen = len;
					obj = i;
				}
		}
	}
	return (obj);
}

float	get_min_max_len(t_rtv1 *iw, int obj, float *maxlen, int light)
{
	float	minlen;

	if (iw->s.o[obj].type == 0)
	{
		minlen = sqrtf(powf(((t_sphere *)iw->s.o[obj].obj)->x - iw->s.l[light].v.x, 2.0f) +
			powf(((t_sphere *)iw->s.o[obj].obj)->y - iw->s.l[light].v.y, 2.0f) +
			powf(((t_sphere *)iw->s.o[obj].obj)->z - iw->s.l[light].v.z, 2.0f));
		//printf("radlen %f\n", minlen);
		*maxlen = minlen + ((t_sphere *)iw->s.o[obj].obj)->r;
		return (minlen - ((t_sphere *)iw->s.o[obj].obj)->r);
	}
}

int		get_light_color(t_rtv1 *iw, int color, int obj, t_vector p)
{
	float	strength;
	float	len;
	float	minlen;
	float	maxlen;
	int		i;

	strength = 0.0f;
	i = -1;
	while (++i < iw->s.light_count)
	{
		if (get_light_object(iw, i) != obj)
			continue;
		if (iw->s.o[obj].type == 1)
		{
			len = -((t_plane *)(iw->s.o[obj].obj))->n.x * p.x -
				((t_plane *)(iw->s.o[obj].obj))->n.y * p.y -
				((t_plane *)(iw->s.o[obj].obj))->n.z * p.z;
			minlen = ((t_plane *)(iw->s.o[obj].obj))->n.x * iw->cam.v.x + ((t_plane *)(iw->s.o[obj].obj))->n.y * iw->cam.v.y +
				((t_plane *)(iw->s.o[obj].obj))->n.z * iw->cam.v.z + len;
			maxlen = ((t_plane *)(iw->s.o[obj].obj))->n.x * iw->s.l[i].v.x + ((t_plane *)(iw->s.o[obj].obj))->n.y * iw->s.l[i].v.y +
				((t_plane *)(iw->s.o[obj].obj))->n.z * iw->s.l[i].v.z + len;
			if ((minlen >= 0 && maxlen >= 0) || (minlen < 0 && maxlen < 0))
				strength += iw->s.l[i].strength;
			len = sqrtf(powf(p.x - iw->s.l[i].v.x, 2.0f) + powf(p.y - iw->s.l[i].v.y, 2.0f)
				+ powf(p.z - iw->s.l[i].v.z, 2.0f));
			//p.x -= iw->cam.v.x;
			//p.y -= iw->cam.v.y;
			//p.z -= iw->cam.v.z;
			//len = acosf((((t_plane *)(iw->s.o[obj].obj))->n.x * p.x + ((t_plane *)(iw->s.o[obj].obj))->n.y
			//	* p.y + ((t_plane *)(iw->s.o[obj].obj))->n.z * p.z) /
			//	(sqrtf(powf(((t_plane *)(iw->s.o[obj].obj))->n.x, 2.0f) + powf(((t_plane *)(iw->s.o[obj].obj))->n.y, 2.0f)/*sosi*/
			//		+ powf(((t_plane *)(iw->s.o[obj].obj))->n.z, 2.0f)) *
			//		sqrtf(powf(p.x, 2.0f) + powf(p.y, 2.0f) + powf(p.z, 2.0f))));
			//if (fabsf(len) < 90.0f)

		}
		else
		{
			minlen = get_min_max_len(iw, obj, &maxlen, i);
			len = sqrtf(powf(p.x - iw->s.l[i].v.x, 2.0f) + powf(p.y - iw->s.l[i].v.y, 2.0f)
				+ powf(p.z - iw->s.l[i].v.z, 2.0f));
			strength += (maxlen - len) / (maxlen - minlen) * iw->s.l[i].strength;
		}
		//`strength -= (len / 100.0f) * (1.0f - iw->s.l[i].strength);
	}
	if (strength < iw->cam.light) ////////
		strength = iw->cam.light;
	if (strength > 1.0f)
		return(color);
	i = (((int)((float)(color >> 16) * strength)) << 16) +
		(((int)((float)((color >> 8) - (color >> 16 << 8)) * strength)) << 8) +
		(int)((float)(color - (color >> 8 << 8)) * strength);
	return (i);
}

void	put_pixel_from_scene(t_rtv1 *iw)
{
	int			i;
	int			color;
	float		minlen;
	float		len;
	t_vector	d;

	d.x = (float)((iw->i - WINDOW_W / 2) * CAMERA_FOV_DISTANCE) / (float)WINDOW_W;
	d.y = (float)((iw->j - WINDOW_H / 2) * CAMERA_FOV_DISTANCE) / (float)WINDOW_H;
	d.z = (float)(CAMERA_FOV_DISTANCE);
	multiply_vector_and_rotation_matrix(iw, &d);
	color = 0;
	minlen = -1.0f;
	i = -1;
	while (++i < iw->s.obj_count)
	{
		if (iw->s.o[i].type == 0)
		{
			if ((len = get_sphere_len(iw, (t_sphere *)(iw->s.o[i].obj), &d, &iw->cam.v)) > 0.0f)
				if (len < minlen || minlen < 0.0f)
				{
					minlen = len;
					color = get_light_color(iw, iw->s.o[i].color, i, iw->p);//iw->s.o[i].color;
				}
		}
		else if (iw->s.o[i].type == 1)
		{
			if ((len = get_plane_len(iw, (t_plane *)(iw->s.o[i].obj), &d, &iw->cam.v)) > 0.0f)
				if (len < minlen || minlen < 0.0f)
				{
					minlen = len;
					color = get_light_color(iw, iw->s.o[i].color, i, iw->p);//iw->s.o[i].color;
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
	free_objects_lights(&iw);
	SDL_FreeSurface(iw.sur);
	SDL_DestroyWindow(iw.win);
	SDL_Quit();
	return (0);
}