/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 01:43:34 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/03 08:20:56 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>

ssize_t	find_nl_position(char *buffer, ssize_t size)
{
	ssize_t	i;

	i = 0;
	while (i < size)
	{
		if (buffer[i] == TRIGGER_CHAR)
			return (i);
		i++;
	}
	return (-1);
}

char	*flush_buffer_list(t_gnl *gnl)
{
	t_gnl_node	*cursor;
	char		*next_line;
	size_t		next_line_size;
	size_t		i;
	size_t		j;

	i = 0;
	cursor = gnl->last;
	if (cursor->nl_position >= 0)
		next_line_size = gnl->total - (cursor->size - cursor->nl_position) + 1;
	else
		next_line_size = gnl->total;
	next_line = malloc(sizeof(char) * (next_line_size + 1));
	cursor = gnl->first;
	while (i < next_line_size)
	{
		j = 0;
		while (j < cursor->size && i < next_line_size)
			next_line[i++] = cursor->buffer[j++];
		cursor = cursor->next;
	}
	next_line[i] = '\0';
	return (next_line);
}

char	*enqueue_buffer(t_gnl *gnl, char *buffer, ssize_t size, int nl_pos)
{
	t_gnl_node	*node;
	ssize_t		i;

	i = 0;
	node = malloc(sizeof(t_gnl_node));
	if (node == NULL)
		return (NULL);
	while (i < size)
	{
		node->buffer[i] = buffer[i];
		i++;
	}
	node->size = (size_t)size;
	node->next = NULL;
	node->nl_position = nl_pos;
	if (gnl->first == NULL)
		gnl->first = node;
	else
		gnl->last->next = node;
	gnl->last = node;
	gnl->total += node->size;
	if (nl_pos >= 0)
		return (flush_buffer_list(gnl));
	return (NULL);
}
