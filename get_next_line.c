/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ikourkji <ikourkji@student.42.us.or>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/08 01:59:07 by ikourkji          #+#    #+#             */
/*   Updated: 2018/12/17 20:34:42 by ikourkji         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
/*
* using t_list content size as fd storage
* storing line in content until we run out of line
* TODO destroy list when reach EOF
* TODO just make a free function for any errors
* maybe make macros for errnos
* maybe have it return NULL or -1 depending on errno
* TODO change MAL to call ^
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
	if (!(run = ft_lstnew(NULL, 1)) || !(run->content = ft_strnew(1)))
		return (NULL);
	run->content_size = (size_t)fd;
	ft_lstadd(fd_list, run);
	return (run);
}

//need: function that takes string, substring section, frees old string, allocates new one, returns that
char			*inc_free(char *s, int st, int end)
{
	char	*tmp;

	if (!(tmp = ft_strsub((const char *)s, st, end)))
		return (NULL);
	free(s);
	s = tmp;
	return (s);
}

int				get_next_line(const int fd, char **line)
{
	char			buf[BUFF_SIZE + 1];
	static t_list	*fd_list;
	t_list			*cur;
	ssize_t			len;
	int				i;


	if (fd < 0 || !line || (read(fd, buf, 0) < 0))
		return (-1);
	if (!(cur = add_and_ret(&fd_list, fd)))
		return (-1);
	i = ft_charat(cur->content, '\n');
	while (i == -1 && (len = read(fd, buf, BUFF_SIZE)))
	{
		buf[len] = '\0';
//it's because you're jumping forward for the cur->cont
//that's why you can't use strjoindel
//you need to do something else so that you're keeping a pointer 
//	to the beginning of cur->cont so you can free it; maybe del old cur instead of skip?
//so, you know, you have a tmp that stores the new ptr, free old ptr, store tmp, free tmp
		if (!(cur->content = ft_strjoin(cur->content, buf)))//you are mallocing
			return (-1);//should you be freeing stuff?
		i = ft_charat(cur->content, '\n');
	}
	len = (ssize_t)ft_strlen(cur->content);
	if (!(*line = ft_strsub((const char *)cur->content, 0,
					(i == -1 ? (size_t)len : (size_t)i))))
		return (-1);//free stuff
	if (len == 0)
		return (0);//delete list
//	cur->content += (i != -1 ? (i + 1) : len);//need to replace
	if (!(inc_free(cur->content, (i != -1 ? (i + 1) : len), len + 1)))
		return (-1);
	return (1);
}
