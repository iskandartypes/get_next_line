#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int		main(int argc, char **argv)
{
	if (argc == 0)
		return (0);
	int		fd = open(argv[1], O_RDONLY);
	
	int		fd2 = open("text2.txt", O_RDONLY);

	int		fd3 = open("text3.txt", O_RDONLY);
	
	char	*line = NULL;

	//line = malloc(sizeof(char) * (12 + 1));
	if (get_next_line(fd, &line) > 0)
	{
		printf("%s\n", line);
		free(line);
	}
	while (get_next_line(fd2, &line) > 0)
	{
		printf("%s\n", line);
		free(line);
	}
	//if (line)
	//	free(line);
	if (get_next_line(fd, &line) > 0)
	{
		printf("%s\n", line);
		free(line);
	}
	if (get_next_line(fd3, &line) > 0)
	{
		printf("%s\n", line);
		free(line);
	}
	close(fd);
	close(fd2);
	close(fd3);
	return (0);
}
