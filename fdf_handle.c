/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_handle.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:18:08 by anemet            #+#    #+#             */
/*   Updated: 2025/07/20 22:02:24 by anemet           ###   ########.fr       */
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
		free(fdf->map->color_grid[i]);
		i++;
	}
	free(fdf->map->z_grid);
	free(fdf->map->color_grid);
}

int	malloc_fault(t_map *map, int nr_lines)
{
	int	i;

	if (nr_lines == -1)
		ft_printf("malloc grid allocation failed\n");
	else
	{
		ft_printf("malloc line nr %d allocation failed\n", nr_lines);
		i = 0;
		while (i < nr_lines)
		{
			free(map->z_grid[i]);
			free(map->color_grid[i]);
			i++;
		}
	}
	if (map->z_grid)
		free(map->z_grid);
	if (map->color_grid)
		free(map->color_grid);
	return (-1);
}

int	handle_close(t_fdf *fdf)
{
	mlx_destroy_window(fdf->mlx_ptr, fdf->win_ptr);
	mlx_destroy_display(fdf->mlx_ptr);
	free(fdf->mlx_ptr);
	free(fdf->view);
	free_map_data(fdf);
	exit(0);
	return (0);
}

int	handle_key_press(int keycode, t_fdf *fdf)
{
	if (keycode == ESC_KEY)
	{
		handle_close(fdf);
	}
	else if (keycode == BACKSPACE_KEY)
	{
		fdf->view->culling_on = !fdf->view->culling_on;
		mlx_clear_window(fdf->mlx_ptr, fdf->win_ptr);
		draw_map(fdf);
	}
	return (0);
}

void	handle_hooks(t_fdf *fdf)
{
	mlx_hook(fdf->win_ptr, DESTROY_NOTIFY, NO_MASK, &handle_close, fdf);
	mlx_hook(fdf->win_ptr, KEY_PRESS, KEY_PRESS_MASK, &handle_key_press, fdf);
	mlx_hook(fdf->win_ptr, BUTTON_PRESS, BUTTON_PRESS_MASK, &handle_mouse_press,
		fdf);
	mlx_hook(fdf->win_ptr, BUTTON_RELEASE, BUTTON_RELEASE_MASK,
		&handle_mouse_release, fdf);
	mlx_hook(fdf->win_ptr, MOTION_NOTIFY, POINTER_MOTION_MASK,
		&handle_mouse_move, fdf);
	mlx_loop(fdf->mlx_ptr);
}
