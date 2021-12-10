/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 01:43:34 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/10 05:42:43 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

static t_buffer	*get_buffer_node(int fd)
{
	t_buffer	*buffer_node;
	char		*buffer;
	ssize_t		read_bytes;

	buffer_node = malloc(sizeof(t_buffer));
	if (buffer_node == NULL)
		return (NULL);
	buffer = malloc(sizeof(char) * BUFFER_SIZE);
	if (buffer == NULL)
		return (free(buffer_node), NULL);
	read_bytes = read(fd, buffer, BUFFER_SIZE);
	if (read_bytes < 0)
		return (free(buffer_node), free(buffer), NULL);
	else if (read_bytes == 0)
	{
		buffer_node->eof = 1;
		buffer[0] = '\0';
	}
	else
		buffer_node->eof = 0;
	buffer_node->size = (unsigned int)read_bytes;
	buffer_node->start = 0;
	buffer_node->buffer = buffer;
	buffer_node->next = NULL;
	return (buffer_node);
}

static unsigned int	get_nl_index(int fd, register t_buffer *list)
{
	register unsigned int	i;
	register unsigned int	nl_index;

	nl_index = 0;
	i = list->start;
	if (list->eof == 1)
		return (UINT_MAX);
	while (42)
	{
		if ((i < list->size && list->buffer[i] == TRIGGER_C) || list->eof == 1)
			return (nl_index + 1);
		else if (i + 1 >= list->size)
		{
			list->next = get_buffer_node(fd);
			if (list->next == NULL)
				return (UINT_MAX);
			i = 0;
			list = list->next;
		}
		else
			++i;
		++nl_index;
	}
	return (UINT_MAX);
}

static void	update_buffer_list(t_buffer *end, t_buffer **list, unsigned int i)
{
	if (end == NULL)
		*list = NULL;
	else if (i >= end->size)
		*list = next_node(end);
	else
	{
		end->start = i;
		*list = end;
	}
}

static char	*flush_buffer_list(register unsigned int nl_index,
	register t_buffer **list)
{
	t_buffer				*cursor;
	char					*new_line;
	register char			*new_line_cursor;
	register unsigned int	i;

	if (nl_index == UINT_MAX)
		return (free_all(list));
	new_line = malloc(sizeof(char) * (nl_index + 1));
	if (new_line == NULL)
		return (free_all(list));
	new_line_cursor = new_line;
	cursor = *list;
	i = cursor->start;
	while (nl_index--)
	{
		*new_line_cursor++ = cursor->buffer[i++];
		if (i >= cursor->size)
		{
			cursor = next_node(cursor);
			i = 0;
		}
	}
	*new_line_cursor = '\0';
	update_buffer_list(cursor, list, i);
	return (new_line);
}

char	*get_next_line(int fd)
{
	static t_buffer	*list = NULL;

	if (list == NULL)
	{
		list = get_buffer_node(fd);
		if (list == NULL)
			return (NULL);
	}
	return (flush_buffer_list(get_nl_index(fd, list), &list));
}
