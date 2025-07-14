#include "minilibx-linux/mlx.h"

int	main(void)
{
	void	*mlx;
	void	*mlx_win;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1920, 1080, "Hello world!");
	mlx_loop(mlx);
}
/* compile with:
cd minilibx-linux
make
# or: ./configure
cd ..
cc test.c -Iminilibx-linux -Lminilibx-linux -lmlx -lXext -lX11 -lbsd

-lmlx (the static archive just built) supplies the mlx_... symbols.
-lXext, -lX11 pull in the X-window routines MiniLibX relies on.
-lbsd is needed because the Linux port uses a few BSD helper calls.

*/
