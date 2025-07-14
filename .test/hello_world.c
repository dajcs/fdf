#include "../minilibx-linux/mlx.h"

int	main(void)
{
	void	*mlx;
	void	*mlx_win;

	mlx = mlx_init();
	mlx_win = mlx_new_window(mlx, 1920, 1080, "Hello world!");
	mlx_loop(mlx);
}

/* compile with:

unpack the project resource `minilibx-linux.tgz` to workspace folder
or get it from github
git clone https://github.com/42paris/minilibx-linux

cd minilibx-linux
make
# or: ./configure
cd ..

cd .test
cc hello_world.c -I../minilibx-linux -L../minilibx-linux -lmlx -lXext -lX11 -lm -lbsd

cc test.c -I../minilibx-linux \       # include directory of the header file mlx.h
   -L../minilibx-linux -lmlx \        # Lib directory `minilibx-linux`, the archive: libmlx.a
   -lXext -lX11 \                  # X-window routines MiniLibX relies on
   -lm \                           # eventually include the math module
   -lbsd                           # needed because the Linux port uses a few BSD helper calls

on WSL2 / win11 install the missing X dependencies first:

sudo apt update
sudo apt install build-essential libx11-dev libxext-dev libbsd-dev zlib1g-dev   # meta-pkg xorg-dev works too

*/
