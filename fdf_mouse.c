/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_mouse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:56:38 by anemet            #+#    #+#             */
/*   Updated: 2025/07/20 20:41:53 by anemet           ###   ########.fr       */
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

// zoom goal: invariant point under mouse cursor remains in that exact screen
// position after the zoom is applied
// map_x -- gets transformed -> screen_x
// screen_x = (map_x * scale) + x_offset
// screen_y = (map_y * sacle) + y_offset
// screen_x, screen_y = mouse_x, mouse_y
// which map_x is projected under the mouse?
// map_x = (mouse_x - old_x_offset) / old_scale
// after zoom if same map_x under the cursor =>
// (mouse_x - old_x_offset) / old_scale = (mouse_x - new_x_offset) / new_scale
// mouse_x and mouse_y are the x, y coming with the event
// new_x_offset = x - (x - old_x_offset) * (new_scale / old_scale)
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
