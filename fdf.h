/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 09:00:39 by anemet            #+#    #+#             */
/*   Updated: 2025/07/16 11:37:53 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

# include "libft/libft.h"
# include "minilibx-linux/mlx.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

/*
Resolution	Aspect	Ratio	Common Name	Main Usage
800x600
1024x768	4:3		XGA		Legacy monitors, tablets
1280x720	16:9	HD		Entry-level monitors, TVs
1280x1024	5:4		SXGA	Legacy/business monitors
1440x900	16:10	WXGA+	Mid-range monitors, laptops
1366x768	16:9	--		Budget/older laptops, displays
1920x1080	16:9	Full HD	Standard monitors, TVs, laptops
2560x1440	16:9	QHD		High-end monitors, gaming laptops
3840x2160	16:9	4K UHD	Premium monitors, TVs, laptops
*/

# define WIN_WIDTH 800
# define WIN_HEIGHT 600

/* Keycodes for Linux */
# define ESC_KEY 65307

/* X11 event codes from <X11/X.h> */
# define KEY_PRESS 2
# define DESTROY_NOTIFY 17

/* X11 event masks from <X11/X.h> */
# define NO_MASK 0
# define KEY_PRESS_MASK 1L<<0

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

typedef struct s_map
{
	int		width;
	int		height;
	int		**z_grid;
}			t_map;

typedef struct s_fdf
{
	void	*mlx_ptr;
	void	*win_ptr;
	t_map	*map;
}			t_fdf;

/* fdf.c */
void		draw_map(t_fdf *fdf);

/* fdf_utils.c */
int			read_map(const char *file, t_map *map);

/* fdf_handle.c */
int			handle_close(t_fdf *fdf);
int			handle_key_press(int keycode, t_fdf *fdf);

#endif
