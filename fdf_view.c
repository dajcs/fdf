/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_view.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 14:20:04 by anemet            #+#    #+#             */
/*   Updated: 2025/07/20 16:18:58 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// projects a single point to find its unscaled screen coordinates
void	raw_project(int x, int y, int z, t_point *p)
{
	p->x = (int)((x - y) * cos(ANGLE));
	p->y = (int)((x + y) * sin(ANGLE) - z);
}

// Iterates through the map to find the screen-space bounding box
static void	calculate_xy_range(t_map *map, t_bounds *bounds)
{
	int		x;
	int		y;
	t_point	p;

	raw_project(0, 0, map->z_grid[0][0], &p);
	bounds->min_x = p.x;
	bounds->max_x = p.x;
	bounds->min_y = p.y;
	bounds->max_y = p.y;
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			raw_project(x, y, map->z_grid[y][x], &p);
			bounds->min_x = min(bounds->min_x, p.x);
			bounds->max_x = max(bounds->max_x, p.x);
			bounds->min_y = min(bounds->min_y, p.y);
			bounds->max_y = max(bounds->max_y, p.y);
			x++;
		}
		y++;
	}
}

// setup_view continued
void	setup_view_25plus(t_fdf *fdf)
{
	fdf->view->is_dragging = 0;
	fdf->view->drag_start_x = 0;
	fdf->view->drag_start_y = 0;
	fdf->view->alpha = 0;
	fdf->view->beta = 0;
	fdf->view->gamma = 0;
}

// the main function to setup the final scale and offsets
void	setup_view(t_fdf *fdf)
{
	t_bounds	bounds;
	float		scale_x;
	float		scale_y;

	calculate_xy_range(fdf->map, &bounds);
	bounds.width = bounds.max_x - bounds.min_x;
	bounds.height = bounds.max_y - bounds.min_y;
	fdf->view = malloc(sizeof(t_view));
	if (!fdf->view)
		handle_close(fdf);
	if (bounds.width == 0 || bounds.height == 0)
		fdf->view->scale = 1;
	else
	{
		scale_x = (WIN_WIDTH * PADDING_FACTOR_X) / bounds.width;
		scale_y = (WIN_HEIGHT * PADDING_FACTOR_Y) / bounds.height;
		fdf->view->scale = minf(scale_x, scale_y);
	}
	fdf->view->x_offset = (WIN_WIDTH - (bounds.width * fdf->view->scale)) / 2;
	fdf->view->y_offset = (WIN_HEIGHT - (bounds.height * fdf->view->scale)) / 2;
	fdf->view->x_offset -= (bounds.min_x * fdf->view->scale);
	fdf->view->y_offset -= (bounds.min_y * fdf->view->scale);
	setup_view_25plus(fdf);
}
