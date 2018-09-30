#ifndef __RTV1
# define __RTV1

# include "SDL2/SDL2.framework/Headers/SDL.h"
//# define _CRT_SECURE_NO_WARNINGS
# include "libft/libft.h"
//# include <SDL.h>
# include <stdlib.h>
# include <unistd.h>
//# include <io.h>
# include <fcntl.h>
// # define read _read
// # define write _write
// # define open _open
// # define close _close
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
	t_vector	oc;
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
	float	rx;
	float	ry;
	float	light;
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

typedef struct	s_flags
{
	int		sel;
}				t_flags;

typedef struct	s_rtv1
{
	SDL_Window	*win;
	SDL_Surface	*sur;
	char		quit;
	t_flags		f;
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

void	set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void	free_objects_lights(t_rtv1 *iw);
void	exit_x(t_rtv1 *iw);
void	update_int(t_rtv1 *iw, int *change, int i);
void	update_float(t_rtv1 *iw, float *change, float i);
void	update_vector(t_rtv1 *iw, int bol, float i);
void	update_move_selected_y(t_rtv1 *iw, int i);
void	update_move_selected_x(t_rtv1 *iw, int i);
void	update_move_selected_z(t_rtv1 *iw, int i);
void	key_down(int code, t_rtv1 *iw);
void	select_object(t_rtv1 *iw, int x, int y);
void	main_loop(t_rtv1 *iw);
void	multiply_vector_and_rotation_matrix(t_rtv1 *iw, t_vector *v);
void	get_sphere_len2(t_variables1 *k, t_vector *d, t_int_vector *v);
float	get_sphere_len(t_rtv1 *iw, t_sphere *sphere, t_vector *d, t_int_vector *v);
float	get_plane_len(t_rtv1 *iw, t_plane *plane, t_vector *d, t_int_vector *v);
void	get_cylinder_len2(t_variables2 *k, t_vector *d, t_int_vector *v);
float	get_cylinder_len(t_rtv1 *iw, t_cylinder *cyl, t_vector *d, t_int_vector *v);
float	get_cone_len(t_rtv1 *iw, t_cone *cone, t_vector *d, t_int_vector *v);
int		get_light_object(t_rtv1 *iw, int light, int curr_obj);
int		get_light_color(t_rtv1 *iw, int color, int obj);
void	put_pixel_from_scene(t_rtv1 *iw);
int		threads_draw2(t_rtv1 *iw);
void	threads_draw(t_rtv1 *iw);
int		split_len(char **s);
int		exit_map(int fd);
int		free_spl(char **spl, char *s, int fd);
int		atoi_16(char *s);
int		add_sphere(char **spl, int spl_len, t_rtv1 *iw, int obj);
int		add_plane(char **spl, int spl_len, t_rtv1 *iw, int obj);
int		add_cylinder(char **spl, int spl_len, t_rtv1 *iw, int obj);
int		add_cone(char **spl, int spl_len, t_rtv1 *iw, int obj);
int		get_objects(int fd, int count, t_rtv1 *iw);
int		get_lights(int fd, int count, t_rtv1 *iw);
int		get_map(t_rtv1 *iw, const char *name);
#endif
