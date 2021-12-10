/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 15:27:06 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/10 05:02:24 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>

void	free_node(t_buffer *node)
{
	free(node->buffer);
	free(node);
}

char	*free_all(t_buffer **list)
{
	t_buffer	*cursor;
	t_buffer	*prev;

	cursor = *list;
	prev = NULL;
	while (cursor)
	{
		prev = cursor;
		cursor = cursor->next;
		free_node(prev);
	}
	*list = NULL;
	return (NULL);
}

t_buffer	*next_node(t_buffer *list)
{
	t_buffer	*prev;

	prev = list;
	list = list->next;
	free_node(prev);
	return (list);
}
