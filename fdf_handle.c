/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:18:08 by anemet            #+#    #+#             */
/*   Updated: 2025/07/16 14:26:09 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static void	free_map_data(t_fdf *fdf)
{
	int	i;

	i = 0;
	while (i < fdf->map->height)
	{
		free(fdf->map->z_grid[i]);
		i++;
	}
	free(fdf->map->z_grid);
}

int	handle_close(t_fdf *fdf)
{
	mlx_destroy_window(fdf->mlx_ptr, fdf->win_ptr);
	mlx_destroy_display(fdf->mlx_ptr);
	free(fdf->mlx_ptr);
	free_map_data(fdf);
	free(fdf->view);
	exit(0);
	return (0);
}

int	handle_key_press(int keycode, t_fdf *fdf)
{
	if (keycode == ESC_KEY)
	{
		handle_close(fdf);
	}
	return (0);
}
