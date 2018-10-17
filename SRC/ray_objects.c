/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_objects.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dbolilyi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/17 16:20:24 by dbolilyi          #+#    #+#             */
/*   Updated: 2018/10/17 16:20:31 by dbolilyi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../rtv1.h"

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
	iw->p = (k.t1 < 1.0f) ? k.p2 : NESTED_TERNARY1;
	m = k.d_v * ((k.t1 < 1.0f) ? k.t2 : NESTED_TERNARY2) + k.oc_v;
	iw->n.x = iw->p.x - (float)cone->c.x - k2p1 * cone->v.x * m;
	iw->n.y = iw->p.y - (float)cone->c.y - k2p1 * cone->v.y * m;
	iw->n.z = iw->p.z - (float)cone->c.z - k2p1 * cone->v.z * m;
	ln = sqrtf(iw->n.x * iw->n.x + iw->n.y * iw->n.y + iw->n.z * iw->n.z);
	iw->n.x /= ln;
	iw->n.y /= ln;
	iw->n.z /= ln;
	return ((k.t1 < 1.0f) ? k.len2 : NESTED_TERNARY3);
}
