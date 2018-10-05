#include "rtv1.h"

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
			return (free_spl(spl, s, fd));
	}
	else
		return (free_spl(spl, s, fd));
	free_spl(spl, s, fd);
	return (1);
}
