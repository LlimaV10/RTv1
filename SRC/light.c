/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 16:17:52 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/10/17 16:18:01 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../rtv1.h"

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

float	get_cone_len_light(t_rtv1 *iw, t_cone *cone,
	t_vector *d, t_int_vector *v)
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
