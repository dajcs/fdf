/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_mouse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:56:38 by anemet            #+#    #+#             */
/*   Updated: 2025/07/19 17:33:55 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

int	handle_mouse_left_click(int x, int y, t_fdf *fdf)
{
	fdf->view->is_dragging = 1;
	fdf->view->drag_start_x = x;
	fdf->view->drag_start_y = y;
	return (0);
}

int	handle_mouse_press(int button, int x, int y, t_fdf *fdf)
{
	float	zoom_factor;
	float	old_scale;
	float	new_scale;

	if (button == MOUSE_WHEEL_UP)
		zoom_factor = 1.1f;
	else if (button == MOUSE_WHEEL_DOWN)
		zoom_factor = 0.9f;
	else if (button == MOUSE_LEFT_BUTTON)
		return (handle_mouse_left_click(x, y, fdf));
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

int	handle_mouse_release(int button, int x, int y, t_fdf *fdf)
{
	(void)x;
	(void)y;
	if (button == MOUSE_LEFT_BUTTON)
		fdf->view->is_dragging = 0;
	return (0);
}

int	handle_mouse_move(int x, int y, t_fdf *fdf)
{
	int	delta_x;
	int	delta_y;

	if (fdf->view->is_dragging)
	{
		delta_x = x - fdf->view->drag_start_x;
		delta_y = y - fdf->view->drag_start_y;
		fdf->view->x_offset += delta_x;
		fdf->view->y_offset += delta_y;
		fdf->view->drag_start_x = x;
		fdf->view->drag_start_y = y;
		mlx_clear_window(fdf->mlx_ptr, fdf->win_ptr);
		draw_map(fdf);
	}
	return (0);
}
