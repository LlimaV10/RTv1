#ifndef __RTV1
# define __RTV1

# include "SDL2/SDL2.framework/Headers/SDL.h"
//# include <SDL.h>
# include <stdlib.h>
//# include <unistd.h>
# include <fcntl.h>
# include <math.h>
//# define HAVE_STRUCT_TIMESPEC
# include <pthread.h>
# define WINDOW_W 400
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
	t_int_vector	n;
}				t_plane;

typedef struct	s_objects
{
	void	*obj;
	int		color;
	char	type; // 0 - sphere, 1 - plane
}				t_objects;

typedef struct	s_camera
{
	int		x;
	int		y;
	int		z;
	float	rx;
	float	ry;
	float	light;
	//float	rz;
}				t_camera;

typedef struct	s_point_light
{
	int		x;
	int		y;
	int		z;
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
}				t_rtv1;
#endif
