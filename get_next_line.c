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
#include <stdlib.h>
#include <unistd.h>

static t_gnl_node	*get_buffer(int fd)
{
	t_gnl_node  *new;
	ssize_t read_len;

	new = malloc(sizeof(t_gnl_node));
	if (new == NULL)
		return (NULL);
	new->buffer = malloc(sizeof(char) * BUFFER_SIZE);
	if (new->buffer == NULL)
		return (free(new), NULL);
	read_len = read(fd, new->buffer, BUFFER_SIZE);
	if (read_len <= 0)
		return (free(new->buffer), free(new), NULL);
	new->len = (size_t)read_len;
	new->start = 0;
	new->end = 0;
	new->next = NULL;
	return (new);
}

static void	free_list(t_gnl_node **list, t_gnl_node *stop)
{
	t_gnl_node	*prev;
	t_gnl_node	*cursor;

	cursor = *list;
	while (cursor != stop)
	{
		prev = cursor;
		cursor = cursor->next;
		free(prev->buffer);
		free(prev);
	}
	*list = stop;
}

static char	*flush_list(t_gnl_node **list, size_t total)
{
	t_gnl_node	*node;
	char	*newline;
	size_t	i;

	i = 0;
	node = *list;
	newline = malloc(sizeof(char) * (total + 1));
	if (newline == NULL)
		return (free_list(list, NULL), NULL);
	while (i < total)
	{
		while (i < total && node->start < node->end)
			newline[i++] = node->buffer[node->start++];
		if (node->start >= node->len)
			node = node->next;
	}
	newline[i] = '\0';
	return (free_list(list, node), newline);
}

static char	*find_newline(t_gnl_node **list, int fd)
{
	t_gnl_node	*node;
	size_t	total;

	node = *list;
	total = 0;
	while (node)
	{
		while (node->end < node->len)
		{
			if (node->buffer[node->end] == '\n')
				return (node->end++, flush_list(list, total + 1));
			node->end++;
			total++;
		}
		node->next = get_buffer(fd);
		node = node->next;
	}
	return (flush_list(list, total));
}

char	*get_next_line(int fd)
{
	static t_gnl_node	*list = NULL;

	if (list == NULL)
	{
		list = get_buffer(fd);
		if (list == NULL)
			return (NULL);
	}
	return find_newline(&list, fd);
}
