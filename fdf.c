/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 09:15:52 by anemet            #+#    #+#             */
/*   Updated: 2025/07/21 13:57:30 by anemet           ###   ########.fr       */
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
	if (view->color_by_alt)
		p.color = calculate_altitude_color(z, fdf->map);
	else
		p.color = fdf->map->color_grid[y][x];
	return (p);
}

void	draw_map(t_fdf *fdf)
{
	t_culling	c;
	int			x;
	int			y;

	y = 0;
	while (y < fdf->map->height - 1)
	{
		x = 0;
		while (x < fdf->map->width - 1)
		{
			c.p1 = project(x, y, fdf, fdf->view);
			c.p2 = project(x + 1, y, fdf, fdf->view);
			c.p3 = project(x, y + 1, fdf, fdf->view);
			c.p4 = project(x + 1, y + 1, fdf, fdf->view);
			if (!fdf->view->culling || is_face_visible(c.p1, c.p2, c.p3))
			{
				bresenham(fdf, c.p1, c.p2);
				bresenham(fdf, c.p1, c.p3);
				bresenham(fdf, c.p2, c.p4);
				bresenham(fdf, c.p3, c.p4);
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
	t_img	img;
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
	fdf.img = &img;
	setup_view(&fdf);
	fdf.mlx_ptr = mlx_init();
	title = make_title(argv[1], fdf.map->width, fdf.map->height);
	fdf.win_ptr = mlx_new_window(fdf.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, title);
	free(title);
	img.img_ptr = mlx_new_image(fdf.mlx_ptr, WIN_WIDTH, WIN_HEIGHT);
	img.addr = mlx_get_data_addr(img.img_ptr, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	handle_hooks(&fdf);
	return (0);
}
