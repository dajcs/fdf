/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 09:15:52 by anemet            #+#    #+#             */
/*   Updated: 2025/07/17 15:59:04 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	my_mlx_pixel_put(t_fdf *fdf, int x, int y, int color)
{
	if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
		mlx_pixel_put(fdf->mlx_ptr, fdf->win_ptr, x, y, color);
}

static void	init_bresenham(t_point p1, t_point p2, t_bres *b)
{
	b->dx = abs(p2.x - p1.x);
	b->dy = -abs(p2.y - p1.y);
	b->sx = 1;
	if (p1.x > p2.x)
		b->sx = -1;
	b->sy = 1;
	if (p1.y > p2.y)
		b->sy = -1;
	b->err = b->dx + b->dy;
}

// dx positive, dy negative, so later comparison of
//           e2 >= dy and e2 <= dx work for lines in all directions (quadrants)
// sx - step x, +1 to right, -1 to left
// sy - step y, +1 to down, -1 to up
// err - error accumulator, initialized to dx + dy
// e2 temp var holding 2 * err to keep algorithm in integer domain
//                            (original Bresenham was err checking against 0.5)
static void	bresenham(t_fdf *fdf, t_point p1, t_point p2)
{
	t_bres	b;

	init_bresenham(p1, p2, &b);
	while (1)
	{
		my_mlx_pixel_put(fdf, p1.x, p1.y, p1.color);
		if (p1.x == p2.x && p1.y == p2.y)
			break ;
		b.e2 = 2 * b.err;
		if (b.e2 >= b.dy)
		{
			b.err += b.dy;
			p1.x += b.sx;
		}
		if (b.e2 <= b.dx)
		{
			b.err += b.dx;
			p1.y += b.sy;
		}
	}
}

static t_point	project(int x, int y, t_fdf *fdf, t_view *view)
{
	t_point	p;
	float	raw_x;
	float	raw_y;
	int		z;

	z = fdf->map->z_grid[y][x];
	raw_x = (x - y) * cos(ANGLE);
	raw_y = (x + y) * sin(ANGLE) - z;
	p.x = (int)(raw_x * view->scale + view->x_offset);
	p.y = (int)(raw_y * view->scale + view->y_offset);
	p.color = fdf->map->color_grid[y][x];
	return (p);
}

void	draw_map(t_fdf *fdf)
{
	int		x;
	int		y;
	t_point	p1;
	t_point	p2;

	y = 0;
	while (y < fdf->map->height)
	{
		x = 0;
		while (x < fdf->map->width)
		{
			p1 = project(x, y, fdf, fdf->view);
			if (x + 1 < fdf->map->width)
			{
				p2 = project((x + 1), y, fdf, fdf->view);
				bresenham(fdf, p1, p2);
			}
			if (y + 1 < fdf->map->height)
			{
				p2 = project(x, (y + 1), fdf, fdf->view);
				bresenham(fdf, p1, p2);
			}
			x++;
		}
		y++;
	}
}

int	main(int argc, char **argv)
{
	t_fdf	fdf;
	t_map	map;

	if (argc != 2)
	{
		ft_printf("Usage: ./fdf <filename.fdf>\n");
		return (1);
	}
	map.z_grid = NULL;
	if (read_map(argv[1], &map) == -1)
		return (1);
	fdf.map = &map;
	setup_view(&fdf);
	fdf.mlx_ptr = mlx_init();
	fdf.win_ptr = mlx_new_window(fdf.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, argv[1]);
	draw_map(&fdf);
	mlx_hook(fdf.win_ptr, DESTROY_NOTIFY, NO_MASK, &handle_close, &fdf);
	mlx_key_hook(fdf.win_ptr, &handle_key_press, &fdf);
	mlx_loop(fdf.mlx_ptr);
	return (0);
}
