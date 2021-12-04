/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 01:43:34 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/04 03:45:09 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <stdlib.h>
#include <strings.h>

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

char	*fill_and_destroy(t_gnl *gnl, char *next_line, size_t next_line_size)
{
	t_gnl_node	*cursor;
	t_gnl_node	*prev;
	size_t		i;
	size_t		j;

	i = 0;
	cursor = gnl->first;
	while (i < next_line_size)
	{
		j = 0;
		while (j < cursor->size && i < next_line_size)
			next_line[i++] = cursor->buffer[j++];
		prev = cursor;
		cursor = cursor->next;
		if (prev != gnl->last)
			free(prev);
	}
	next_line[i] = '\0';
	gnl = new_init_fd_list(gnl->fd, gnl);
	if (prev->nl_position == -1 || (size_t)prev->nl_position == prev->size - 1)
		return ((free(prev), next_line));
	enqueue_buffer(gnl, prev->buffer + prev->nl_position + 1,
		prev->size - (prev->nl_position + 1), -1);
	free(prev);
	return (next_line);
}

char	*flush_buffer_list(t_gnl *gnl)
{
	t_gnl_node	*last;
	char		*next_line;
	size_t		next_line_size;

	last = gnl->last;
	if (last == NULL)
		return (NULL);
	else if (last->nl_position >= 0)
		next_line_size = gnl->total - (last->size - last->nl_position) + 1;
	else
		next_line_size = gnl->total;
	next_line = malloc(sizeof(char) * (next_line_size + 1));
	if (next_line == NULL)
		return (NULL);
	next_line = fill_and_destroy(gnl, next_line, next_line_size);
	last = gnl->last;
	if (last != NULL)
		last->nl_position = find_nl_position(last->buffer, (ssize_t)last->size);
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

t_gnl	*new_init_fd_list(int fd, t_gnl *to_init)
{
	t_gnl	*new;

	if (to_init == NULL)
	{
		new = malloc(sizeof(t_gnl));
		if (new == NULL)
			return (NULL);
		new->next = NULL;
	}
	else
		new = to_init;
	new->fd = fd;
	new->total = 0;
	new->first = NULL;
	new->last = NULL;
	return (new);
}

/*void	printf_buflist(t_gnl *gnl)
{
	t_gnl_node	*cursor;

	cursor = gnl->first;
	printf("BUFFER LIST FD %d\n", gnl->fd);
	while (cursor)
	{
		puts("-----");
		printf("Address %p\n", (void *)cursor);
		printf("Size %ld\n", cursor->size);
		printf("NL Pos %d\n", (int)cursor->nl_position);
		printf("Buffer >%.*s<\n", (int)cursor->size, cursor->buffer);
		puts("-----");
		cursor = cursor->next;
	}
	printf("END BUFFER LIST\n");
}
*/
