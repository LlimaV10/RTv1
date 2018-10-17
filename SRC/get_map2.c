/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 14:23:55 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/10/17 14:24:04 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../rtv1.h"

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

int		get_objects_ret(t_rtv1 *iw, t_get_map *t, int i)
{
	if (ft_strcmp(*t->spl, "sphere:") == 0)
		return (add_sphere(t->spl, t->spl_len, iw, i));
	else if (ft_strcmp(*t->spl, "plane:") == 0)
		return (add_plane(t->spl, t->spl_len, iw, i));
	else if (ft_strcmp(*t->spl, "cylinder:") == 0)
		return (add_cylinder(t->spl, t->spl_len, iw, i));
	else if (ft_strcmp(*t->spl, "cone:") == 0)
		return (add_cone(t->spl, t->spl_len, iw, i));
	return (0);
}

int		get_objects(int fd, int count, t_rtv1 *iw)
{
	t_get_map	t;
	int			i;
	int			ret;

	if (count < 1 || count > 10)
		return (0);
	iw->s.o = (t_objects *)malloc(count * sizeof(t_objects));
	i = -1;
	while (++i < count)
	{
		if (get_next_line(fd, &t.s) < 0)
			return (0);
		t.spl = ft_strsplit(t.s, ' ');
		t.spl_len = split_len(t.spl);
		ret = get_objects_ret(iw, &t, i);
		free_spl(t.spl, t.s, 0);
		if (ret == 0)
			return (0);
		iw->s.obj_count = i + 1;
	}
	return (1);
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
