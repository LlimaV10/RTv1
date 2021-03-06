#include "rtv1.h"

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

void	get_scene1(t_rtv1 *iw)
{
	t_sphere		*tmp;
	t_plane			*tmp2;
	t_cylinder		*tmp3;
	t_cone			*tmp4;

	iw->f.sel = 0;
	iw->cam.v.x = 0;
	iw->cam.v.y = 0;
	iw->cam.v.z = 0;
	iw->cam.rx = 0.0f;
	iw->cam.ry = 0.0f;
	iw->cam.light = 0.1f;
	iw->s.obj_count = 6;
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
	tmp2->n.x = 0.0f;
	tmp2->n.y = -1.0f;
	tmp2->n.z = 0.0f;
	iw->s.o[2].obj = (void *)tmp2;
	iw->s.o[2].type = 1;
	iw->s.o[2].color = 0xFFFF00;

	tmp2 = (t_plane *)malloc(sizeof(t_plane));
	tmp2->p0.x = 0;
	tmp2->p0.y = 0;
	tmp2->p0.z = 70;
	tmp2->n.x = 0.0f;
	tmp2->n.y = -1.0f / sqrtf(2.0f);
	tmp2->n.z = -1.0f / sqrtf(2.0f);
	iw->s.o[3].obj = (void *)tmp2;
	iw->s.o[3].type = 1;
	iw->s.o[3].color = 0x0000FF;

	tmp3 = (t_cylinder *)malloc(sizeof(t_cylinder));
	tmp3->c.x = -10;
	tmp3->c.y = 0;
	tmp3->c.z = 30;
	tmp3->v.x = 1.0f / sqrtf(2.0f);
	tmp3->v.y = 1.0f / sqrtf(2.0f);
	tmp3->v.z = 0.0f;
	tmp3->r = 3;
	iw->s.o[4].obj = (void *)tmp3;
	iw->s.o[4].type = 2;
	iw->s.o[4].color = 0xFF00FF;

	tmp4 = (t_cone *)malloc(sizeof(t_cone));
	tmp4->c.x = -35;
	tmp4->c.y = 20;
	tmp4->c.z = 20;
	tmp4->v.x = 0.0f;
	tmp4->v.y = 1.0f / sqrtf(2.0f);
	tmp4->v.z = 1.0f / sqrtf(2.0f);
	tmp4->k = 0.161799f / 4;
	iw->s.o[5].obj = (void *)tmp4;
	iw->s.o[5].type = 3;
	iw->s.o[5].color = 0xFFFFFF;

	iw->s.light_count = 2;
	iw->s.l = (t_point_light *)malloc(iw->s.light_count * sizeof(t_point_light));
	iw->s.l[0].v.x = -25;
	iw->s.l[0].v.y = -25;
	iw->s.l[0].v.z = 35;
	iw->s.l[0].strength = 0.5f;

	iw->s.l[1].v.x = 25;
	iw->s.l[1].v.y = -25;
	iw->s.l[1].v.z = 35;
	iw->s.l[1].strength = 0.5f;
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
}

void	get_sphere_len2(t_variables1 *k, t_vector *d, t_int_vector *v)
{
	k->p1.x = ((float)v->x + k->t1 * d->x);
	k->p1.y = (float)v->y + k->t1 * d->y;
	k->p1.z = ((float)v->z + k->t1 * d->z);
	k->p2.x = ((float)v->x + k->t2 * d->x);
	k->p2.y = (float)v->y + k->t2 * d->y;
	k->p2.z = ((float)v->z + k->t2 * d->z);
	k->len1 = sqrtf(powf((float)v->x - k->p1.x, 2.0f)
		+ powf((float)v->y - k->p1.y, 2.0f)
		+ powf((float)v->z - k->p1.z, 2.0f));
	k->len2 = sqrtf(powf((float)v->x - k->p2.x, 2.0f)
		+ powf((float)v->y - k->p2.y, 2.0f)
		+ powf((float)v->z - k->p2.z, 2.0f));
}

int		get_sphere_len_n(t_sphere *sphere, t_vector *d,
	t_variables1 *k, float *discriminant)
{
	k->k2 = 2.0f * (k->oc.x * d->x + k->oc.y * d->y + k->oc.z * d->z);
	k->k3 = (k->oc.x * k->oc.x + k->oc.y * k->oc.y + k->oc.z * k->oc.z)
		- (float)(sphere->r * sphere->r);
	*discriminant = k->k2 * k->k2 - 4.0f * k->k1 * k->k3;
	if (*discriminant < 0.0f)
		return (-1);
	k->t1 = (-k->k2 + sqrtf(*discriminant)) / (2.0f * k->k1);
	k->t2 = (-k->k2 - sqrtf(*discriminant)) / (2.0f * k->k1);
	return (1);
}

float	get_sphere_len(t_rtv1 *iw, t_sphere *sphere,
	t_vector *d, t_int_vector *v)
{
	t_variables1	k;
	t_vector		p_c;
	float			lp_c;
	float			discriminant;

	k.k1 = d->x * d->x + d->y * d->y + d->z * d->z;
	k.oc.x = (float)(-sphere->x + v->x);
	k.oc.y = (float)(-sphere->y + v->y);
	k.oc.z = (float)(-sphere->z + v->z);
	if (get_sphere_len_n(sphere, d, &k, &discriminant) < 0
		|| k.t1 < 1.0f || k.t2 < 1.0f)
		return (-1.0f);
	get_sphere_len2(&k, d, v);
	iw->p = (k.len1 < k.len2) ? k.p1 : k.p2;
	p_c.x = iw->p.x - (float)sphere->x;
	p_c.y = iw->p.y - (float)sphere->y;
	p_c.z = iw->p.z - (float)sphere->z;
	lp_c = sqrtf(p_c.x * p_c.x + p_c.y * p_c.y + p_c.z * p_c.z);
	iw->n.x = p_c.x / lp_c;
	iw->n.y = p_c.y / lp_c;
	iw->n.z = p_c.z / lp_c;
	return ((k.len1 < k.len2) ? k.len1 : k.len2);
}

float	get_plane_len(t_rtv1 *iw, t_plane *plane, t_vector *d, t_int_vector *v)
{
	float			t;
	t_int_vector	p0l0;
	float			d_v;

	p0l0.x = -plane->p0.x + v->x;
	p0l0.y = -plane->p0.y + v->y;
	p0l0.z = -plane->p0.z + v->z;
	d_v = (d->x * plane->n.x + d->y * plane->n.y + d->z * plane->n.z);
	t = (-((float)p0l0.x * plane->n.x + (float)p0l0.y *
		plane->n.y + (float)p0l0.z * plane->n.z)) / d_v;
	if (t < 0)
		return (-1.0f);
	iw->p.x = (float)v->x + d->x * t;
	iw->p.y = (float)v->y + d->y * t;
	iw->p.z = (float)v->z + d->z * t;
	if (d_v > 0)
	{
		iw->n.x = -plane->n.x;
		iw->n.y = -plane->n.y;
		iw->n.z = -plane->n.z;
	}
	else
		iw->n = plane->n;
	return (sqrtf(powf((float)v->x - iw->p.x, 2.0f) + powf((float)v->y
		- iw->p.y, 2.0f) + powf((float)v->z - iw->p.z, 2.0f)));
}

void	get_cylinder_len2(t_variables2 *k, t_vector *d, t_int_vector *v)
{
	k->p1.x = (float)v->x + k->t1 * d->x;
	k->p1.y = (float)v->y + k->t1 * d->y;
	k->p1.z = (float)v->z + k->t1 * d->z;
	k->p2.x = (float)v->x + k->t2 * d->x;
	k->p2.y = (float)v->y + k->t2 * d->y;
	k->p2.z = (float)v->z + k->t2 * d->z;
	k->len1 = sqrtf(powf((float)v->x - k->p1.x, 2.0f) +
		powf((float)v->y - k->p1.y, 2.0f)
		+ powf((float)v->z - k->p1.z, 2.0f));
	k->len2 = sqrtf(powf((float)v->x - k->p2.x, 2.0f) +
		powf((float)v->y - k->p2.y, 2.0f)
		+ powf((float)v->z - k->p2.z, 2.0f));
}

int		get_cylinder_len_n(t_cylinder *cyl, t_vector *d, t_variables2 *k)
{
	k->a = (d->x * d->x + d->y * d->y + d->z * d->z) -
		k->d_v * k->d_v;
	k->b = (d->x * k->oc.x + d->y * k->oc.y + d->z * k->oc.z) -
		k->d_v * k->oc_v;
	k->c = (k->oc.x * k->oc.x + k->oc.y * k->oc.y + k->oc.z * k->oc.z) -
		k->oc_v * k->oc_v - (float)(cyl->r * cyl->r);
	k->disk = k->b * k->b - k->a * k->c;
	if (k->disk < 0.0f)
		return (-1);
	k->t1 = (-k->b + sqrtf(k->disk)) / k->a;
	k->t2 = (-k->b - sqrtf(k->disk)) / k->a;
	return (1);
}

float	get_cylinder_len(t_rtv1 *iw, t_cylinder *cyl,
	t_vector *d, t_int_vector *v)
{
	t_variables2	k;
	t_vector		nrm;
	float			lnrm;

	k.oc.x = (float)(-cyl->c.x + v->x);
	k.oc.y = (float)(-cyl->c.y + v->y);
	k.oc.z = (float)(-cyl->c.z + v->z);
	k.d_v = d->x * cyl->v.x + d->y * cyl->v.y + d->z * cyl->v.z;
	k.oc_v = k.oc.x * cyl->v.x + k.oc.y * cyl->v.y + k.oc.z * cyl->v.z;
	if (get_cylinder_len_n(cyl, d, &k) < 0 || (k.t1 < 1.0f && k.t2 < 1.0f))
		return (-1.0f);
	get_cylinder_len2(&k, d, v);
	iw->p = (k.len1 < k.len2) ? k.p1 : k.p2;
	nrm.x = iw->p.x - (float)cyl->c.x - cyl->v.x *
		(k.d_v * ((k.len1 < k.len2) ? k.t1 : k.t2) + k.oc_v);
	nrm.y = iw->p.y - (float)cyl->c.y - cyl->v.y *
		(k.d_v * ((k.len1 < k.len2) ? k.t1 : k.t2) + k.oc_v);
	nrm.z = iw->p.z - (float)cyl->c.z - cyl->v.z *
		(k.d_v * ((k.len1 < k.len2) ? k.t1 : k.t2) + k.oc_v);
	lnrm = sqrtf(nrm.x * nrm.x + nrm.y * nrm.y + nrm.z * nrm.z);
	iw->n.x = nrm.x / lnrm;
	iw->n.y = nrm.y / lnrm;
	iw->n.z = nrm.z / lnrm;
	return ((k.len1 < k.len2) ? k.len1 : k.len2);
}

int		get_cone_len_n(t_vector *d, t_variables2 *k, float k2p1)
{
	k->a = (d->x * d->x + d->y * d->y + d->z * d->z) -
		k2p1 * k->d_v * k->d_v;
	k->b = (d->x * k->oc.x + d->y * k->oc.y + d->z * k->oc.z) -
		k2p1 * k->d_v * k->oc_v;
	k->c = (k->oc.x * k->oc.x + k->oc.y * k->oc.y + k->oc.z * k->oc.z) -
		k2p1 * k->oc_v * k->oc_v;
	k->disk = k->b * k->b - k->a * k->c;
	if (k->disk < 0.0f)
		return (-1);
	k->t1 = (-k->b + sqrtf(k->disk)) / k->a;
	k->t2 = (-k->b - sqrtf(k->disk)) / k->a;
	return (1);
}

float	get_cone_len_light(t_rtv1 *iw, t_cone *cone, t_vector *d, t_int_vector *v)
{
	t_variables2	k;
	float			k2p1;
	float			m;
	float			ln;

	k.oc.x = v->x - cone->c.x;
	k.oc.y = -cone->c.y + v->y;
	k.oc.z = -cone->c.z + v->z;
	k.d_v = d->x * cone->v.x + d->y * cone->v.y + d->z * cone->v.z;
	k.oc_v = k.oc.x * cone->v.x + k.oc.y * cone->v.y + k.oc.z * cone->v.z;
	k2p1 = cone->k * cone->k + 1.0f;
	if (get_cone_len_n(d, &k, k2p1) < 0 || k.t1 < 1.0f || k.t2 < 1.0f)
		return (-1.0f);
	get_cylinder_len2(&k, d, v);
	iw->p = (k.len1 < k.len2) ? k.p1 : k.p2;
	m = k.d_v * ((k.len1 < k.len2) ? k.t1 : k.t2) + k.oc_v;
	iw->n.x = iw->p.x - (float)cone->c.x - k2p1 * cone->v.x * m;
	iw->n.y = iw->p.y - (float)cone->c.y - k2p1 * cone->v.y * m;
	iw->n.z = iw->p.z - (float)cone->c.z - k2p1 * cone->v.z * m;
	ln = sqrtf(iw->n.x * iw->n.x + iw->n.y * iw->n.y + iw->n.z * iw->n.z);
	iw->n.x /= ln;
	iw->n.y /= ln;
	iw->n.z /= ln;
	return ((k.len1 < k.len2) ? k.len1 : k.len2);
}

float	get_cone_len(t_rtv1 *iw, t_cone *cone, t_vector *d, t_int_vector *v)
{
	t_variables2	k;
	float			k2p1;
	float			m;
	float			ln;

	k.oc.x = v->x - cone->c.x;
	k.oc.y = -cone->c.y + v->y;
	k.oc.z = -cone->c.z + v->z;
	k.d_v = d->x * cone->v.x + d->y * cone->v.y + d->z * cone->v.z;
	k.oc_v = k.oc.x * cone->v.x + k.oc.y * cone->v.y + k.oc.z * cone->v.z;
	k2p1 = cone->k * cone->k + 1.0f;
	if (get_cone_len_n(d, &k, k2p1) < 0 || (k.t1 < 1.0f && k.t2 < 1.0f))
		return (-1.0f);
	get_cylinder_len2(&k, d, v);
	iw->p = (k.t1 < 1.0f) ? k.p2 : ((k.t2 < 1.0f) ? k.p1 : ((k.len1 < k.len2) ? k.p1 : k.p2));
	m = k.d_v * ((k.t1 < 1.0f) ? k.t2 : ((k.t2 < 1.0f) ? k.t1
		: ((k.len1 < k.len2) ? k.t1 : k.t2))) + k.oc_v;
	iw->n.x = iw->p.x - (float)cone->c.x - k2p1 * cone->v.x * m;
	iw->n.y = iw->p.y - (float)cone->c.y - k2p1 * cone->v.y * m;
	iw->n.z = iw->p.z - (float)cone->c.z - k2p1 * cone->v.z * m;
	ln = sqrtf(iw->n.x * iw->n.x + iw->n.y * iw->n.y + iw->n.z * iw->n.z);
	iw->n.x /= ln;
	iw->n.y /= ln;
	iw->n.z /= ln;
	return ((k.t1 < 1.0f) ? k.len2 : ((k.t2 < 1.0f) ? k.len1 : ((k.len1 < k.len2) ? k.len1 : k.len2)));
}

float	get_light_object2(t_rtv1 *iw, int i, t_vector *d, int light)
{
	if (iw->s.o[i].type == 0)
		return (get_sphere_len(iw, (t_sphere *)(iw->s.o[i].obj),
			d, &(iw->s.l[light].v)));
	else if (iw->s.o[i].type == 1)
		return (get_plane_len(iw, (t_plane *)(iw->s.o[i].obj),
			d, &(iw->s.l[light].v)));
	else if (iw->s.o[i].type == 2)
		return (get_cylinder_len(iw, (t_cylinder *)(iw->s.o[i].obj),
			d, &(iw->s.l[light].v)));
	else if (iw->s.o[i].type == 3)
		return (get_cone_len_light(iw, (t_cone *)(iw->s.o[i].obj),
			d, &(iw->s.l[light].v)));
		return (-1.0f);
}

int		get_light_object(t_rtv1 *iw, int light, int curr_obj)
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
	minlen += 0.1f;
	obj = curr_obj;
	i = -1;
	while (++i < iw->s.obj_count)
	{
		len = get_light_object2(iw, i, &d, light);
		if (len > 0.0f && len < minlen)
		{
			minlen = len;
			obj = i;
		}
	}
	return (obj);
}

void	get_light_color2(t_get_light_color *l, t_rtv1 *iw)
{
	l->l.x = (float)iw->s.l[l->i].v.x - l->sp.x;
	l->l.y = (float)iw->s.l[l->i].v.y - l->sp.y;
	l->l.z = (float)iw->s.l[l->i].v.z - l->sp.z;
	l->lenl = sqrtf(l->l.x * l->l.x + l->l.y * l->l.y + l->l.z * l->l.z);
	l->l.x /= l->lenl;
	l->l.y /= l->lenl;
	l->l.z /= l->lenl;
	l->lenl = l->sn.x * l->l.x + l->sn.y * l->l.y + l->sn.z * l->l.z;
}

int		get_light_color(t_rtv1 *iw, int color, int obj)
{
	t_get_light_color	l;

	l.sp = iw->p;
	l.sn = iw->n;
	l.strength = iw->cam.light;
	l.i = -1;
	while (++l.i < iw->s.light_count)
	{
		iw->p = l.sp;
		iw->n = l.sn;
		if (get_light_object(iw, l.i, obj) != obj)
			continue;
		get_light_color2(&l, iw);
		if (l.lenl > 0)
			l.strength += iw->s.l[l.i].strength * l.lenl;
	}
	if (l.strength < iw->cam.light)
		l.strength = iw->cam.light;
	if (l.strength > 1.0f)
		return (color);
	l.i = (((int)((float)(color >> 16) * l.strength)) << 16) +
		(((int)((float)((color >> 8) - (color >> 16 << 8))
		* l.strength)) << 8) + (int)((float)(color - (color
		>> 8 << 8)) * l.strength);
	return (l.i);
}

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

int		split_len(char **s)
{
	int		i;

	if (s == 0)
		return (0);
	i = 0;
	while (*s != 0)
	{
		i++;
		s++;
	}
	return (i);
}

int		exit_map(int fd)
{
	if (fd != 0)
		close(fd);
	return (0);
}

int		free_spl(char **spl, char *s, int fd)
{
	char	**tmp;

	if (s == 0)
		return (0);
	free(s);
	if (spl == 0)
		return (0);
	tmp = spl;
	while (*spl != 0)
	{
		free(*spl);
		spl++;
	}
	free(tmp);
	return (exit_map(fd));
}

int		atoi_16(char *s)
{
	int		col;

	col = 0;
	while (*(++s))
	{
		if (*s >= '0' && *s <= '9')
			col = col * 16 + *s - '0';
		else if (*s >= 'A' && *s <= 'F')
			col = col * 16 + *s - 55;
		else if (*s >= 'a' && *s <= 'f')
			col = col * 16 + *s - 87;
		else
			return (0xFFFFFF);
	}
	return (col);
}

int		add_sphere(char **spl, int spl_len, t_rtv1 *iw, int obj)
{
	t_sphere	*tmp;

	if (spl_len != 6)
		return (0);
	if (spl[5][0] != '0' || spl[5][1] != 'x' || ft_strlen(spl[5]) != 8)
		return (0);
	tmp = (t_sphere *)malloc(sizeof(t_sphere));
	tmp->x = ft_atoi(spl[1]);
	tmp->y = ft_atoi(spl[2]);
	tmp->z = ft_atoi(spl[3]);
	tmp->r = ft_atoi(spl[4]);
	iw->s.o[obj].type = 0;
	iw->s.o[obj].color = atoi_16(spl[5] + 1);
	iw->s.o[obj].obj = (void *)tmp;
	return (1);
}

int		add_plane(char **spl, int spl_len, t_rtv1 *iw, int obj)
{
	t_plane	*tmp;
	float	vlen;

	if (spl_len != 8)
		return (0);
	if (spl[7][0] != '0' || spl[7][1] != 'x' || ft_strlen(spl[7]) != 8)
		return (0);
	tmp = (t_plane *)malloc(sizeof(t_plane));
	tmp->p0.x = ft_atoi(spl[1]);
	tmp->p0.y = ft_atoi(spl[2]);
	tmp->p0.z = ft_atoi(spl[3]);
	tmp->n.x = (float)ft_atoi(spl[4]);
	tmp->n.y = (float)ft_atoi(spl[5]);
	tmp->n.z = (float)ft_atoi(spl[6]);
	vlen = sqrtf(tmp->n.x * tmp->n.x + tmp->n.y * tmp->n.y +
		tmp->n.z * tmp->n.z);
	tmp->n.x /= vlen;
	tmp->n.y /= vlen;
	tmp->n.z /= vlen;
	iw->s.o[obj].type = 1;
	iw->s.o[obj].color = atoi_16(spl[7] + 1);
	iw->s.o[obj].obj = (void *)tmp;
	return (1);
}

int		add_cylinder(char **spl, int spl_len, t_rtv1 *iw, int obj)
{
	t_cylinder	*tmp;
	float		vlen;

	if (spl_len != 9)
		return (0);
	if (spl[8][0] != '0' || spl[8][1] != 'x' || ft_strlen(spl[8]) != 8)
		return (0);
	tmp = (t_cylinder *)malloc(sizeof(t_cylinder));
	tmp->c.x = ft_atoi(spl[1]);
	tmp->c.y = ft_atoi(spl[2]);
	tmp->c.z = ft_atoi(spl[3]);
	tmp->v.x = (float)ft_atoi(spl[4]);
	tmp->v.y = (float)ft_atoi(spl[5]);
	tmp->v.z = (float)ft_atoi(spl[6]);
	vlen = sqrtf(tmp->v.x * tmp->v.x + tmp->v.y * tmp->v.y +
		tmp->v.z * tmp->v.z);
	tmp->v.x /= vlen;
	tmp->v.y /= vlen;
	tmp->v.z /= vlen;
	tmp->r = ft_atoi(spl[7]);
	iw->s.o[obj].type = 2;
	iw->s.o[obj].color = atoi_16(spl[8] + 1);
	iw->s.o[obj].obj = (void *)tmp;
	return (1);
}

int		add_cone(char **spl, int spl_len, t_rtv1 *iw, int obj)
{
	t_cone	*tmp;
	float		vlen;

	if (spl_len != 9)
		return (0);
	if (spl[8][0] != '0' || spl[8][1] != 'x' || ft_strlen(spl[8]) != 8)
		return (0);
	tmp = (t_cone *)malloc(sizeof(t_cone));
	tmp->c.x = ft_atoi(spl[1]);
	tmp->c.y = ft_atoi(spl[2]);
	tmp->c.z = ft_atoi(spl[3]);
	tmp->v.x = (float)ft_atoi(spl[4]);
	tmp->v.y = (float)ft_atoi(spl[5]);
	tmp->v.z = (float)ft_atoi(spl[6]);
	vlen = sqrtf(tmp->v.x * tmp->v.x + tmp->v.y * tmp->v.y +
		tmp->v.z * tmp->v.z);
	tmp->v.x /= vlen;
	tmp->v.y /= vlen;
	tmp->v.z /= vlen;
	tmp->k = tanf((float)ft_atoi(spl[7]) * 0.0174533f);
	iw->s.o[obj].type = 3;
	iw->s.o[obj].color = atoi_16(spl[8] + 1);
	iw->s.o[obj].obj = (void *)tmp;
	return (1);
}

int		get_objects(int fd, int count, t_rtv1 *iw)
{
	char	*s;
	char	**spl;
	int		spl_len;
	int		i;
	int		ret;

	if (count < 1 || count > 10)
		return (0);
	iw->s.o = (t_objects *)malloc(count * sizeof(t_objects));
	i = -1;
	while (++i < count)
	{
		if (get_next_line(fd, &s) < 0)
			return (0);
		spl = ft_strsplit(s, ' ');
		spl_len = split_len(spl);
		if (spl_len < 1)
			return (free_spl(spl, s, 0));
		if (ft_strcmp(*spl, "sphere:") == 0)
			ret = add_sphere(spl, spl_len, iw, i);
		else if (ft_strcmp(*spl, "plane:") == 0)
			ret = add_plane(spl, spl_len, iw, i);
		else if (ft_strcmp(*spl, "cylinder:") == 0)
			ret = add_cylinder(spl, spl_len, iw, i);
		else if (ft_strcmp(*spl, "cone:") == 0)
			ret = add_cone(spl, spl_len, iw, i);
		else
			return (free_spl(spl, s, 0));
		free_spl(spl, s, 0);
		if (ret == 0)
			return (0);
		iw->s.obj_count = i + 1;
	}
	return (1);
}

int		get_lights(int fd, int count, t_rtv1 *iw)
{
	char	*s;
	char	**spl;
	int		spl_len;
	int		i;

	if (count < 1 || count > 5)
		return (0);
	iw->s.light_count = count;
	iw->s.l = (t_point_light *)malloc(count * sizeof(t_point_light));
	i = -1;
	while (++i < count)
	{
		if (get_next_line(fd, &s) < 0)
			return (0);
		spl = ft_strsplit(s, ' ');
		spl_len = split_len(spl);
		if (spl_len != 4)
			return (free_spl(spl, s, 0));
		iw->s.l[i].v.x = ft_atoi(spl[0]);
		iw->s.l[i].v.y = ft_atoi(spl[1]);
		iw->s.l[i].v.z = ft_atoi(spl[2]);
		iw->s.l[i].strength = (float)abs(ft_atoi(spl[3])) / 1000.0f;
		free_spl(spl, s, 0);
	}
	return (1);
}

int		get_map(t_rtv1 *iw, int fd)
{
	char	*s;
	char	**spl;
	int		spl_len;

	iw->f.sel = 0;
	if (get_next_line(fd, &s) < 0)
		return (exit_map(fd));
	spl = ft_strsplit(s, ' ');
	spl_len = split_len(spl);
	if (spl_len < 1)
		return (free_spl(spl, s, fd));
	if (ft_strcmp(*spl, "camera:") == 0)
	{
		if (spl_len != 7)
			return (free_spl(spl, s, fd));
		iw->cam.v.x = ft_atoi(spl[1]);
		iw->cam.v.y = ft_atoi(spl[2]);
		iw->cam.v.z = ft_atoi(spl[3]);
		iw->cam.rx = (float)ft_atoi(spl[4]) * 0.0174533f;
		iw->cam.ry = (float)ft_atoi(spl[5]) * 0.0174533f;
		iw->cam.light = (float)abs(ft_atoi(spl[6])) / 1000.0f;
		free_spl(spl, s, 0);
		if (get_next_line(fd, &s) < 0)
			return (0);
		spl = ft_strsplit(s, ' ');
		spl_len = split_len(spl);
		if (spl_len < 1)
			return (free_spl(spl, s, fd));
	}
	else
	{
		iw->cam.v.x = 0;
		iw->cam.v.y = 0;
		iw->cam.v.z = 0;
		iw->cam.rx = 0.0f;
		iw->cam.ry = 0.0f;
		iw->cam.light = 0.1f;
	}
	if ((ft_strcmp(*spl, "objects:") == 0) &&
		(spl_len == 2) && (get_objects(fd, ft_atoi(spl[1]), iw) != 0))
	{
		free_spl(spl, s, 0);
		if (get_next_line(fd, &s) < 0)
		{
			iw->s.light_count = 0;
			return (1);
		}
		spl = ft_strsplit(s, ' ');
		spl_len = split_len(spl);
		if (get_lights(fd, ft_atoi(spl[1]), iw) == 0)
			return(free_spl(spl, s, fd));
	}
	else
		return(free_spl(spl, s, fd));
	free_spl(spl, s, fd);
	return (1);
}

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

int		main(int argc, char **argv)
{
	t_rtv1	iw;
	int		fd;

	if ((fd = check4err(argc, argv)) < 0)
		return (0);
	iw.s.o = 0;
	iw.s.l = 0;
	iw.s.light_count = 0;
	iw.s.obj_count = 0;
	if (get_map(&iw, fd) == 0)
		write(1, "Error\n", 6);
	else
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		iw.win = SDL_CreateWindow("RTv1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
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