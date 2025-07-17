/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 18:01:23 by anemet            #+#    #+#             */
/*   Updated: 2025/07/17 18:05:43 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
int	add_rgb(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}
