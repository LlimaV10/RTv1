/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_objects2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 16:23:14 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/10/17 16:23:29 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../rtv1.h"

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
