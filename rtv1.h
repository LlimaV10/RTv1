#ifndef __RTV1
# define __RTV1

# include "SDL2/SDL2.framework/Headers/SDL.h"
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>

# define WINDOW_W 700
# define WINDOW_H 700
# define CAMERA_W 100
# define CAMERA_H 100
# define CAMERA_FOV_DISTANCE 1

typedef struct	s_sphere
{
	int		x;
	int		y;
	int		z;
	int		r;
}				t_sphere;

typedef struct	s_objects
{
	void	*obj;
	int		type; // 0 - sphere
}				t_objects;

typedef struct	s_camera
{
	int		x;
	int		y;
	int		z;
	int		rx;
	int		ry;
	int		rz;
}				t_camera;

typedef struct	s_rtv1
{
	SDL_Window	*win;
	SDL_Surface	*sur;
	char		quit;
	t_objects	*o;
	t_camera	cam;
}				t_rtv1;
#endif