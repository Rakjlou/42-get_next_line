/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 01:43:34 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/09 19:21:00 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

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
	buffer_node->size = (unsigned long)read_bytes;
	buffer_node->start = 0;
	buffer_node->buffer = buffer;
	buffer_node->next = NULL;
	return (buffer_node);
}

static unsigned long	get_nl_index(int fd, t_buffer *list)
{
	unsigned long	i;
	unsigned long	nl_index;

	nl_index = 0;
	i = list->start;
	if (list->eof == 1)
		return (ULONG_MAX);
	while (42)
	{
		if ((i < list->size && list->buffer[i] == TRIGGER_C) || list->eof == 1)
			return (nl_index + 1);
		else if (i + 1 >= list->size)
		{
			list->next = get_buffer_node(fd);
			if (list->next == NULL)
				return (ULONG_MAX);
			i = 0;
			list = list->next;
		}
		else
			++i;
		++nl_index;
	}
	return (ULONG_MAX);
}

static void	update_buffer_list(t_buffer *end, t_buffer **list, unsigned int i)
{
	if (end == NULL)
		*list = NULL;
	else if (i >= end->size)
		*list = next_node(end, NULL);
	else
	{
		end->start = i;
		*list = end;
	}
}

static char	*flush_buffer_list(unsigned long nl_index, t_buffer **list)
{
	t_buffer		*cursor;
	char			*new_line;
	char			*new_line_cursor;
	unsigned int	i;

	if (nl_index == ULONG_MAX)
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
			cursor = next_node(cursor, &i);
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
