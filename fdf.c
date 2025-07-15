/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 09:15:52 by anemet            #+#    #+#             */
/*   Updated: 2025/07/15 15:49:55 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	my_mlx_pixel_put(t_fdf *fdf, int x, int y, int color)
{
	if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
		mlx_pixel_put(fdf->mlx_ptr, fdf->win_ptr, x, y, color);
}

static void	bresenham(t_fdf *fdf, t_point p1, t_point p2)
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;

	dx = abs(p2.x - p1.x);
	dy = -abs(p2.y - p1.y);
	sx = 1;
	if (p1.x > p2.x)
		sx = -1;
	sy = 1;
	if (p1.y > p2.y)
		sy = -1;
	err = dx + dy;
	while (1)
	{
		my_mlx_pixel_put(fdf, p1.x, p1.y, 0x00FFFFFF);
		if (p1.x == p2.x && p1.y == p2.y)
			break ;
		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			p1.x += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			p1.y += sy;
		}
	}
}

static t_point	project(int x, int y, int z)
{
	t_point	p;
	double	angle;

	angle = 0.523599;
	p.x = (int)((x - y) * cos(angle));
	p.y = (int)((x + y) * sin(angle) - z);
	p.x += WIN_WIDTH / 2;
	p.y += WIN_HEIGHT / 4;
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
			p1 = project(x * 20, y * 20, fdf->map->z_grid[y][x]);
			if (x + 1 < fdf->map->width)
			{
				p2 = project((x + 1) * 20, y * 20, fdf->map->z_grid[y][x + 1]);
				bresenham(fdf, p1, p2);
			}
			if (y + 1 < fdf->map->height)
			{
				p2 = project(x * 20, (y + 1) * 20, fdf->map->z_grid[y + 1][x]);
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
	fdf.mlx_ptr = mlx_init();
	fdf.win_ptr = mlx_new_window(fdf.mlx_ptr, WIN_WIDTH, WIN_HEIGHT, "FdF");
	draw_map(&fdf);
	mlx_hook(fdf.win_ptr, DESTROY_NOTIFY, NO_MASK, &handle_close, &fdf);
	mlx_key_hook(fdf.win_ptr, &handle_key_press, &fdf);
	mlx_loop(fdf.mlx_ptr);
	return (0);
}
