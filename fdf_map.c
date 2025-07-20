/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anemet <anemet@student.42luxembourg.lu>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 11:05:16 by anemet            #+#    #+#             */
/*   Updated: 2025/07/20 23:25:59 by anemet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fdf.h"

static int	count_words(char const *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			count++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (count);
}

static int	get_map_dimensions(const char *file, t_map *map)
{
	char	*line;
	int		fd;
	int		height;
	int		width;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		return (-1);
	height = 0;
	width = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (!width)
			width = count_words(line, ' ');
		height++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	map->width = width;
	map->height = height;
	fd = open(file, O_RDONLY);
	return (fd);
}

// this fills both z and color values for a single row
// when `,` is in the splitted chunk
// *color_str = '\0'  // end the string at `,` position then read z and color
// otherwise read z and put DEFAULT_COLOR
static void	fill_map(char *line, int *z_row, int *color_row)
{
	char	**split;
	char	*color_str;
	int		i;

	split = ft_split(line, ' ');
	i = 0;
	while (split[i])
	{
		color_str = ft_strchr(split[i], ',');
		if (color_str)
		{
			*color_str = '\0';
			color_str++;
			z_row[i] = ft_atoi(split[i]);
			color_row[i] = ft_atoi_hex(color_str);
		}
		else
		{
			z_row[i] = ft_atoi(split[i]);
			color_row[i] = DEFAULT_COLOR;
		}
		free(split[i]);
		i++;
	}
	free(split);
}

static int	file_fault(const char *file)
{
	ft_printf("Can't open file %s\n", file);
	return (-1);
}

// TODO: proper error handling
int	read_map(const char *file, t_map *map)
{
	int		fd;
	char	*line;
	int		i;

	fd = get_map_dimensions(file, map);
	if (fd == -1)
		return (file_fault(file));
	map->z_grid = (int **)malloc(sizeof(int *) * map->height);
	map->color_grid = (int **)malloc(sizeof(int *) * map->height);
	if (!map->z_grid || !map->color_grid)
		return (malloc_fault(map, -1));
	i = 0;
	while (i < map->height)
	{
		line = get_next_line(fd);
		map->z_grid[i] = (int *)malloc(sizeof(int) * map->width);
		map->color_grid[i] = (int *)malloc(sizeof(int) * map->width);
		if (!map->z_grid[i] || !map->color_grid[i])
			return (malloc_fault(map, i));
		fill_map(line, map->z_grid[i], map->color_grid[i]);
		free(line);
		i++;
	}
	close(fd);
	return (calculate_z_range(map));
}
