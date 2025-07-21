/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 13:32:52 by anemet            #+#    #+#             */
/*   Updated: 2025/07/21 13:39:58 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// dst = ... // calculate the memory address for the pixel (x,y)
// *(unsigned int *)dst = color;
	// write the color integer to the calculated address
void	my_mlx_pixel_put(t_fdf *fdf, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
	{
		dst = fdf->img->addr + (y * fdf->img->line_length + x
				* (fdf->img->bits_per_pixel / 8));
		*(unsigned int *)dst = color;
	}
}

// converts a single hex character to its int value (0-15)
static int	hex_char_to_int(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (0);
}

// converts hex color string (e.g., "0xFF00AA") to an integer
int	ft_atoi_hex(char *str)
{
	int	result;
	int	i;

	result = 0;
	i = 0;
	if (str[i] == '0' && (str[i + 1] == 'x' || str[i + 1] == 'X'))
		i += 2;
	while (str[i])
	{
		result = result * 16 + hex_char_to_int(str[i]);
		i++;
	}
	return (result);
}

char	*make_title(char *fname, int width, int height)
{
	char	*res;
	char	*w;
	char	*h;
	char	*tmp;

	w = ft_itoa(width);
	h = ft_itoa(height);
	res = ft_strjoin(fname, "  ");
	tmp = res;
	res = ft_strjoin(res, w);
	free(tmp);
	tmp = res;
	res = ft_strjoin(res, "x");
	free(tmp);
	tmp = res;
	res = ft_strjoin(res, h);
	free(tmp);
	free(w);
	free(h);
	return (res);
}
