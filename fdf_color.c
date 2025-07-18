/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:01:23 by anemet            #+#    #+#             */
/*   Updated: 2025/07/18 15:55:19 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// extracts the Red component from a color integer
int	get_r(int color)
{
	return ((color >> 16) & 0xFF);
}

// extracts the Green component
int	get_g(int color)
{
	return ((color >> 8) & 0xFF);
}

// extracts the Blue component
int	get_b(int color)
{
	return (color & 0xFF);
}

// combines R, G, B componenets back into a single color integer
int	combine_rgb(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}

// p1 is between b.p0 and b.p2
// when p1 is b.p0 ratio is 0, when p1 is b.p2 ratio is 1
int	get_color(t_point p1, t_point p2, t_bres b)
{
	float	ratio;
	int		color;

	ratio = (abs(p1.x - b.x0) + abs(p1.y - b.y0)) / (float)(b.dx - b.dy);
	color = combine_rgb(
			(int)(get_r(p1.color) * (1 - ratio) + get_r(p2.color) * ratio),
			(int)(get_g(p1.color) * (1 - ratio) + get_g(p2.color) * ratio),
			(int)(get_b(p1.color) * (1 - ratio) + get_b(p2.color) * ratio));
	return (color);
}
