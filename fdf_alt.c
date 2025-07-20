/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_alt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 23:22:16 by anemet            #+#    #+#             */
/*   Updated: 2025/07/21 00:08:29 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	calculate_z_range(t_map *map)
{
	int	x;
	int	y;

	map->min_z = INT_MAX;
	map->max_z = INT_MIN;
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if (map->z_grid[y][x] < map->min_z)
				map->min_z = map->z_grid[y][x];
			if (map->z_grid[y][x] > map->max_z)
				map->max_z = map->z_grid[y][x];
			x++;
		}
		y++;
	}
	return (0);
}

// linear interpolation between two color component values
static int	lerp_color(int low, int high, float ratio)
{
	return ((int)(low * (1.0f - ratio) + high * ratio));
}

// calculates a color based on a Z-altitude withing the map's range
// Implements a 3-point gradient: Blue -> White -> Red
int	calculate_altitude_color(int z, t_map *map)
{
	float	ratio;
	int		blue;
	int		white;
	int		red;
	t_color	c;

	blue = 0x0000FF;
	white = 0xFFFFFF;
	red = 0xFF0000;
	if (map->max_z == map->min_z)
		return (white);
	ratio = (float)(z - map->min_z) / (float)(map->max_z - map->min_z);
	if (ratio < 0.5f)
	{
		c.r = lerp_color(get_r(blue), get_r(white), ratio * 2.0f);
		c.g = lerp_color(get_g(blue), get_g(white), ratio * 2.0f);
		c.b = lerp_color(get_b(blue), get_b(white), ratio * 2.0f);
	}
	else
	{
		c.r = lerp_color(get_r(white), get_r(red), (ratio - 0.5f) * 2.0f);
		c.g = lerp_color(get_g(white), get_g(red), (ratio - 0.5f) * 2.0f);
		c.b = lerp_color(get_b(white), get_b(red), (ratio - 0.5f) * 2.0f);
	}
	return (combine_rgb(c.r, c.g, c.b));
}
