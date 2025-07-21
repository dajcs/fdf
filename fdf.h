/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 09:00:39 by anemet            #+#    #+#             */
/*   Updated: 2025/07/21 14:03:05 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "libft/libft.h"
# include "minilibx-linux/mlx.h"
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

/*
Resolution	Aspect	Ratio	Common Name	Main Usage
800x600		4:3
1024x768	4:3		XGA		Legacy monitors, tablets
1280x720	16:9	HD		Entry-level monitors, TVs
1280x1024	5:4		SXGA	Legacy/business monitors
1366x768	16:9	--		Budget/older laptops, displays
1440x900	16:10	WXGA+	Mid-range monitors, laptops
1920x1080	16:9	Full HD	Standard monitors, TVs, laptops
2560x1440	16:9	QHD		High-end monitors, gaming laptops
3840x2160	16:9	4K UHD	Premium monitors, TVs, laptops
*/

# define WIN_WIDTH 1280
# define WIN_HEIGHT 1000
# define PADDING_FACTOR_X 0.8
# define PADDING_FACTOR_Y 0.75

/* isometric projection angle
   angle = 0.6154797086703873 -> magic angle: atan(sin(pi/4)) (35.264 degr)
			rotate around z 45 degr, then rotate around x by arctan(sqrt(2))
*/
# define ANGLE 0.6154797086703873

// default color: white for points without color info
# define DEFAULT_COLOR 0xFFFFFF

/* Keycodes for Linux */
# define ESC_KEY 65307
# define BACKSPACE_KEY 65288
# define KEY_C 99
# define MOUSE_LEFT_BUTTON 1
# define MOUSE_WHEEL_UP 4
# define MOUSE_WHEEL_DOWN 5

/* X11 event codes from <X11/X.h> */
# define KEY_PRESS 2
# define BUTTON_PRESS 4
# define BUTTON_RELEASE 5
# define MOTION_NOTIFY 6
# define DESTROY_NOTIFY 17

/* X11 event masks from <X11/X.h> */
# define NO_MASK 0
# define KEY_PRESS_MASK 1
# define BUTTON_PRESS_MASK 4
# define BUTTON_RELEASE_MASK 8
# define POINTER_MOTION_MASK 64
# define STRUCTURE_NOTIFY_MASK 131072

/*
Event Name      x_event     x_mask      mlx_..._hook       Description
KeyPress          2         1L<<0       mlx_key_hook       key pressed down
KeyRelease        3         1L<<1       (no equivalent)    key was released
DestroyNotify    17         1L<<17      (no equivalent)    window closed by X
Expose           12         1L<<15      mlx_expose_hook    part of window
																to be redrawn
*/

typedef struct s_point
{
	int		x;
	int		y;
	int		z;
	int		color;
}			t_point;

typedef struct s_color
{
	int		r;
	int		g;
	int		b;
}			t_color;

/*
struct collecting variables to draw a color gradient line with Bresenham alg.
  - dx, dy: endpoints difference on x and y
  - sx, sy: step x, step y, can be +1 or -1 depending on quadrant
  - err: deviation from ideal, updated step-by-step, starts with dx - dy
  - e2: 2 * err to keep the algorithm in integer domain
  - x0, y0: starting coordinates of the line, helps to get progress p1 -> p2
*/
typedef struct s_bres
{
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	int		e2;
	int		x0;
	int		y0;
	int		color;
}			t_bres;

typedef struct s_map
{
	int		width;
	int		height;
	int		**z_grid;
	int		**color_grid;
	int		min_z;
	int		max_z;
}			t_map;

typedef struct s_bounds
{
	int		min_x;
	int		max_x;
	int		min_y;
	int		max_y;
	int		width;
	int		height;
}			t_bounds;

typedef struct s_culling
{
	t_point	p1;
	t_point	p2;
	t_point	p3;
	t_point	p4;
}			t_culling;

typedef struct s_view
{
	float	scale;
	int		x_offset;
	int		y_offset;
	int		is_dragging;
	int		drag_start_x;
	int		drag_start_y;
	int		culling;
	int		color_by_alt;
}			t_view;

typedef struct s_img
{
	void	*img_ptr;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_img;

typedef struct s_fdf
{
	void	*mlx_ptr;
	void	*win_ptr;
	t_map	*map;
	t_view	*view;
	t_img	*img;
}			t_fdf;

/* fdf.c */
void		draw_map(t_fdf *fdf);

/* fdf_map.c */
int			read_map(const char *file, t_map *map);

/* fdf_alt.c */
int			calculate_z_range(t_map *map);
int			calculate_altitude_color(int z, t_map *map);
int			render_frame(t_fdf *fdf);

/* fdf_view.c */
void		setup_view(t_fdf *fdf);
int			is_face_visible(t_point p1, t_point p2, t_point p3);

/* fdf_handle.c */
int			handle_close(t_fdf *fdf);
int			handle_key_press(int keycode, t_fdf *fdf);
int			malloc_fault(t_map *map, int nr_lines);
void		handle_hooks(t_fdf *fdf);

/* fdf_mouse.c */
int			handle_mouse_press(int button, int x, int y, t_fdf *fdf);
int			handle_mouse_release(int button, int x, int y, t_fdf *fdf);
int			handle_mouse_move(int x, int y, t_fdf *fdf);

/* fdf_utils.c */
int			ft_atoi_hex(char *str);
void		my_mlx_pixel_put(t_fdf *fdf, int x, int y, int color);
char		*make_title(char *fname, int width, int height);

/* fdf_color.c */
int			get_r(int color);
int			get_g(int color);
int			get_b(int color);
int			combine_rgb(int r, int g, int b);
int			get_color(t_point p1, t_point p2, t_bres b);

#endif
