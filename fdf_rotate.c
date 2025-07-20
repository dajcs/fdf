/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_rotate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 16:29:41 by anemet            #+#    #+#             */
/*   Updated: 2025/07/20 16:56:23 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

// rotate around Y (beta), X (alpha) and Z (gamma)
void	rotate_point(float *x, float *y, float *z, t_view *view)
{
	float	prev_x;
	float	prev_y;

	prev_x = *x;
	*x = prev_x * cos(view->beta) + *z * sin(view->beta);
	*z = -prev_x * sin(view->beta) + *z * cos(view->beta);
	prev_y = *y;
	*y = prev_y * cos(view->alpha) - *z * sin(view->alpha);
	*z = prev_y * sin(view->alpha) + *z * cos(view->alpha);
	prev_x = *x;
	prev_y = *y;
	*x = prev_x * cos(view->gamma) - prev_y * sin(view->gamma);
	*y = prev_x * sin(view->gamma) + prev_y * cos(view->gamma);
}

void	handle_rotation(int keycode, t_fdf *fdf)
{
	if (keycode == ARROW_UP)
		fdf->view->alpha -= 0.05;
	else if (keycode == ARROW_DOWN)
		fdf->view->alpha += 0.05;
	else if (keycode == ARROW_LEFT)
		fdf->view->beta -= 0.05;
	else if (keycode == ARROW_RIGHT)
		fdf->view->beta += 0.05;
	else if (keycode == KEY_MINUS)
		fdf->view->gamma -= 0.05;
	else if (keycode == KEY_PLUS)
		fdf->view->gamma += 0.05;
	mlx_clear_window(fdf->mlx_ptr, fdf->win_ptr);
	draw_map(fdf);
}
