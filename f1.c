#include "rtv1.h"

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
		l.l.x = (float)iw->s.l[l.i].v.x - l.sp.x;
		l.l.y = (float)iw->s.l[l.i].v.y - l.sp.y;
		l.l.z = (float)iw->s.l[l.i].v.z - l.sp.z;
		l.lenl = sqrtf(l.l.x * l.l.x + l.l.y * l.l.y + l.l.z * l.l.z);
		l.l.x /= l.lenl;
		l.l.y /= l.lenl;
		l.l.z /= l.lenl;
		l.lenl = l.sn.x * l.l.x + l.sn.y * l.l.y + l.sn.z * l.l.z;
		if (l.lenl > 0)
			l.strength += iw->s.l[l.i].strength * l.lenl;
		// l.len = sqrtf(powf(l.sp.x - iw->s.l[l.i].v.x, 2.0f) + powf(l.sp.y - iw->s.l[l.i].v.y, 2.0f)
		// 	+ powf(l.sp.z - iw->s.l[l.i].v.z, 2.0f));
		//l.strength -= (l.len / 500.0f) * (1.0f - iw->s.l[l.i].strength);
	}
	if (l.strength < iw->cam.light)
		l.strength = iw->cam.light;
	if (l.strength > 1.0f)
		return(color);
	l.i = (((int)((float)(color >> 16) * l.strength)) << 16) +
		(((int)((float)((color >> 8) - (color >> 16 << 8)) * l.strength)) << 8) +
		(int)((float)(color - (color >> 8 << 8)) * l.strength);
	return (l.i);
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
			len = get_sphere_len(iw, (t_sphere *)(iw->s.o[i].obj), &d, &iw->cam.v);
		else if (iw->s.o[i].type == 1)
			len = get_plane_len(iw, (t_plane *)(iw->s.o[i].obj), &d, &iw->cam.v);
		else if (iw->s.o[i].type == 2)
			len = get_cylinder_len(iw, (t_cylinder *)(iw->s.o[i].obj), &d, &iw->cam.v);
		else if (iw->s.o[i].type == 3)
			len = get_cone_len(iw, (t_cone *)(iw->s.o[i].obj), &d, &iw->cam.v);
		if (len > 0.0f)
			if (len < minlen || minlen < 0.0f)
			{
				minlen = len;
				color = get_light_color(iw, iw->s.o[i].color, i);//iw->s.o[i].color;
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

	if (s != 0)
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
	iw->s.o[obj].color = atoi_16(spl[5] + 1);//0xFFFFFF;/////////////spl[5]
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
	iw->s.o[obj].color = atoi_16(spl[7] + 1);//0xFFFFFF;/////////////spl[7]
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
	iw->s.o[obj].color = atoi_16(spl[8] + 1);//0xFFFFFF;/////////////spl[8]
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
	tmp->k = (float)ft_atoi(spl[7]) * 0.0174533f;
	iw->s.o[obj].type = 3;
	iw->s.o[obj].color = atoi_16(spl[8] + 1);//0xFFFFFF;/////////////spl[8]
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

void	get_default_camera(t_rtv1 *iw)
{
	iw->cam.v.x = 0;
	iw->cam.v.y = 0;
	iw->cam.v.z = 0;
	iw->cam.rx = 0.0f;
	iw->cam.ry = 0.0f;
	iw->cam.light = 0.1f;
}

int		get_map_camera(char **spl, int spl_len, t_rtv1 *iw, int fd)
{
	char	*s;

	s = 0;
	if (ft_strcmp(*spl, "camera:") == 0)
	{
		if (spl_len != 7)
			return (-1);
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
		free(s);
		if (spl_len < 1)
			return (-1);
	}
	else
		get_default_camera(iw);
	return (1);
}

int		get_map(t_rtv1 *iw, const char *name)
{
	char	*s;
	char	**spl;
	int		fd;
	int		spl_len;

	fd = 0;
	iw->f.sel = 0;
	if ((fd = open(name, O_RDONLY)) < 0)
		return (0);
	if (get_next_line(fd, &s) < 0)
		return (exit_map(fd));
	spl = ft_strsplit(s, ' ');
	spl_len = split_len(spl);
	if (spl_len < 1)
		return (free_spl(spl, s, fd));
	if (get_map_camera(spl, spl_len, iw, fd) < 0)
		return (free_spl(spl, s, fd));
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
			return (free_spl(spl, s, fd));
	}
	else
		return (free_spl(spl, s, fd));
	free_spl(spl, s, fd);
	return (1);
}

int		main(void)
{
	t_rtv1	iw;

	//get_scene1(&iw);
	iw.s.o = 0;
	iw.s.l = 0;
	iw.s.light_count = 0;
	iw.s.obj_count = 0;
	if (get_map(&iw, "maps/map") == 0)
		write(1, "Error\n", 6);
	else
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		//SDL_SetRelativeMouseMode(0);
		iw.win = SDL_CreateWindow("RTv1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
		iw.sur = SDL_GetWindowSurface(iw.win);
		//get_scene1(&iw);
		threads_draw(&iw);
		SDL_UpdateWindowSurface(iw.win);
		main_loop(&iw);
		SDL_FreeSurface(iw.sur);
		SDL_DestroyWindow(iw.win);
		SDL_Quit();
	}
	free_objects_lights(&iw);
	//system("PAUSE");
	system("leaks RTv1");
	return (0);
}