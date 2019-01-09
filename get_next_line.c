/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikourkji <ikourkji@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 01:59:07 by ikourkji          #+#    #+#             */
/*   Updated: 2019/01/09 04:44:05 by ikourkji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
** using t_list content size as fd storage
** storing line in content until we run out of line
*/

static t_list	*add_and_ret(t_list **fd_list, int fd)
{
	t_list	*run;

	run = *fd_list;
	while (run)
	{
		if (run->content_size == (size_t)fd)
			return (run);
		run = run->next;
	}
	if (!(run = ft_lstnew("\0", 1)))
		return (NULL);
	run->content_size = (size_t)fd;
	ft_lstadd(fd_list, run);
	return (run);
}

static int		del_node_and_line(t_list **fd_list, int fd, char **line)
{
	t_list	*run;
	t_list	*tmp;

	run = *fd_list;
	if (run->content_size == (size_t)fd)
	{
		ft_strdel((char **)&run->content);
		*fd_list = run->next;
		free(run);
	}
	else
		while ((tmp = run->next))
		{
			if (tmp->content_size == (size_t)fd)
			{
				ft_strdel((char **)&tmp->content);
				run = tmp->next;
				free(tmp);
				break ;
			}
			run = run->next;
		}
	ft_strdel(line);
	return (0);
}

int				get_next_line(const int fd, char **line)
{
	char			buf[BUFF_SIZE + 1];
	static t_list	*fd_list;
	t_list			*cur;
	ssize_t			len;
	int				i;

	CHECK(fd < 0 || !line || (read(fd, buf, 0) < 0));
	*line = NULL;
	CHECK(!(cur = add_and_ret(&fd_list, fd)));
	i = ft_charat(cur->content, '\n');
	while (i == -1 && (len = read(fd, buf, BUFF_SIZE)))
	{
		buf[len] = '\0';
		CHECK(!(cur->content = ft_strjoindel(cur->content, buf)));
		i = ft_charat(cur->content, '\n');
	}
	len = (ssize_t)ft_strlen(cur->content);
	ft_strdel(line);
	CHECK(!(*line = ft_strsub((const char *)cur->content, 0,
					(i == -1 ? (size_t)len : (size_t)i))));
	if (len == 0)
		return (del_node_and_line(&fd_list, fd, line));
	i = (i != -1 ? (i + 1) : len);
	cur->content = ft_strsubdel(cur->content, i, len - i);
	return (1);
}
