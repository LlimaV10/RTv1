/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map_objects.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 15:52:55 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/10/17 15:53:03 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../rtv1.h"

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
	t_cone		*tmp;
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
