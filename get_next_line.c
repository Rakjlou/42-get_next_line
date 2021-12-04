/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 01:43:34 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/04 02:22:53 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static t_gnl	*get_fd_list(int fd, t_gnl **buffers_list)
{
	t_gnl	*cursor;
	t_gnl	*prev;
	t_gnl	*new;

	prev = NULL;
	cursor = *buffers_list;
	while (cursor)
	{
		if (cursor->fd == fd)
			return (cursor);
		prev = cursor;
		cursor = cursor->next;
	}
	new = new_init_fd_list(fd, NULL);
	if (new == NULL)
		return (NULL);
	else if (prev == NULL)
		*buffers_list = new;
	else
		prev->next = new;
	return (new);
}

char	*get_next_line(int fd)
{
	static t_gnl	*buffers_list = NULL;
	t_gnl			*gnl;
	char			buffer[BUFFER_SIZE];
	ssize_t			bytes_read;
	char			*next_line;

	gnl = get_fd_list(fd, &buffers_list);
	if (gnl == NULL)
		return (NULL);
	while (42)
	{
		if (gnl->last != NULL && gnl->last->nl_position >= 0)
			return (flush_buffer_list(gnl));
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read <= 0)
			return (flush_buffer_list(gnl));
		next_line = enqueue_buffer(gnl, buffer, bytes_read,
				find_nl_position(buffer, bytes_read));
		if (next_line != NULL)
			return (next_line);
	}
	return (NULL);
}
