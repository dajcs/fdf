/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_mouse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:56:38 by anemet            #+#    #+#             */
/*   Updated: 2025/07/19 15:57:26 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	handle_mouse(int button, int x, int y, t_fdf *fdf)
{
	float	zoom_factor;
	float	old_scale;
	float	new_scale;

	if (button == MOUSE_WHEEL_UP)
		zoom_factor = 1.1f;
	else if (button == MOUSE_WHEEL_DOWN)
		zoom_factor = 0.9f;
	else
		return (0);
	old_scale = fdf->view->scale;
	new_scale = old_scale * zoom_factor;
	fdf->view->x_offset = x - (x - fdf->view->x_offset) * (new_scale
			/ old_scale);
	fdf->view->y_offset = y - (y - fdf->view->y_offset) * (new_scale
			/ old_scale);
	fdf->view->scale = new_scale;
	mlx_clear_window(fdf->mlx_ptr, fdf->win_ptr);
	draw_map(fdf);
	return (0);
}
