/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 09:15:52 by anemet            #+#    #+#             */
/*   Updated: 2025/07/20 17:00:20 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

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
	b->x0 = p1.x;
	b->y0 = p1.y;
	if (p1.color == p2.color)
		b->color = p1.color;
	else
		b->color = 0;
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
	int		color;

	init_bresenham(p1, p2, &b);
	while (1)
	{
		color = get_color(p1, p2, b);
		my_mlx_pixel_put(fdf, p1.x, p1.y, color);
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

// x_f, y_f, z_f: center point around map's middle
// rotate the point
// apply isometric projection
// apply scaling and offset
static t_point	project(int x, int y, t_fdf *fdf)
{
	t_point	p;
	float	x_f;
	float	y_f;
	float	z_f;

	p.color = fdf->map->color_grid[y][x];
	x_f = x - (fdf->map->width / 2.0f);
	y_f = y - (fdf->map->height / 2.0f);
	z_f = fdf->map->z_grid[y][x];
	rotate_point(&x_f, &y_f, &z_f, fdf->view);
	p.x = (x_f - y_f) * cos(ANGLE);
	p.y = (x_f + y_f) * sin(ANGLE) - z_f;
	p.x = (int)(p.x * fdf->view->scale + fdf->view->x_offset);
	p.y = (int)(p.y * fdf->view->scale + fdf->view->y_offset);
	return (p);
}

void	draw_map(t_fdf *fdf)
{
	t_point	t;
	t_point	p1;
	t_point	p2;

	t.y = 0;
	while (t.y < fdf->map->height)
	{
		t.x = 0;
		while (t.x < fdf->map->width)
		{
			p1 = project(t.x, t.y, fdf);
			if (t.x + 1 < fdf->map->width)
			{
				p2 = project((t.x + 1), t.y, fdf);
				bresenham(fdf, p1, p2);
			}
			if (t.y + 1 < fdf->map->height)
			{
				p2 = project(t.x, (t.y + 1), fdf);
				bresenham(fdf, p1, p2);
			}
			t.x++;
		}
		t.y++;
	}
}

int	main(int argc, char **argv)
{
	t_fdf	fdf;
	t_map	map;
	char	*title;

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
	title = make_title(argv[1], fdf.map->width, fdf.map->height);
	fdf.win_ptr = mlx_new_window(fdf.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, title);
	free(title);
	draw_map(&fdf);
	handle_hooks(&fdf);
	return (0);
}
