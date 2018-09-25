#ifndef __RTV1
# define __RTV1

//# include "SDL2/SDL2.framework/Headers/SDL.h"
# include <SDL.h>
# include <stdlib.h>
//# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# define HAVE_STRUCT_TIMESPEC
# include <pthread.h>
# define WINDOW_W 200
# define WINDOW_H WINDOW_W
# define THREADS 4
# define CAMERA_FOV_DISTANCE 1

typedef struct	s_vector
{
	float	x;
	float	y;
	float	z;
}				t_vector;

typedef struct	s_int_vector
{
	int	x;
	int	y;
	int	z;
}				t_int_vector;

typedef struct	s_variables1
{
	float		k1;
	float		k2;
	float		k3;
	float		t1;
	float		t2;
	float		len1;
	float		len2;
	t_vector	p1;
	t_vector	p2;
}				t_variables1;

typedef struct	s_variables2
{
	float		a;
	float		b;
	float		c;
	float		t1;
	float		t2;
	float		disk;
	float		len1;
	float		len2;
	t_vector	p1;
	t_vector	p2;
	t_vector	oc;
	float		d_v;
	float		oc_v;
}				t_variables2;

typedef struct	s_get_light_color
{
	float	strength;
	float	len;
	int		i;
	t_vector	sp;
	t_vector	sn;
	t_vector	l;
	float		lenl;
}				t_get_light_color;

typedef struct	s_sphere
{
	int		x;
	int		y;
	int		z;
	int		r;
}				t_sphere;

typedef struct	s_plane
{
	t_int_vector	p0;
	t_vector		n;
}				t_plane;

typedef struct	s_cylinder
{
	t_vector		v;
	t_int_vector	c;
	int				r;
}				t_cylinder;

typedef struct	s_cone
{
	t_int_vector	c;
	t_vector		v;
	float			k; // tanf(alpha / 2)
}				t_cone;

typedef struct	s_objects
{
	void	*obj;
	int		color;
	char	type; // 0 - sphere, 1 - plane, 2 - cylinder, 3 - cone
}				t_objects;

typedef struct	s_camera
{
	t_int_vector	v;
	/*int		x;
	int		y;
	int		z;*/
	float	rx;
	float	ry;
	float	light;
	//float	rz;
}				t_camera;

typedef struct	s_point_light
{
	t_int_vector	v;
	/*int		x;
	int		y;
	int		z;*/
	float	strength;
}				t_point_light;

typedef struct	s_scene
{
	int				obj_count;
	t_objects		*o;
	int				light_count;
	t_point_light	*l;
}				t_scene;

typedef struct	s_rtv1
{
	SDL_Window	*win;
	SDL_Surface	*sur;
	char		quit;
	t_camera	cam;
	t_scene		s;
	int			i;
	int			st;
	int			end;
	int			j;
	int			color;
	t_vector	p;
	t_vector	n;
}				t_rtv1;
#endif
