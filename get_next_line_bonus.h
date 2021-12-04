/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsierra- <nsierra-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 01:43:34 by nsierra-          #+#    #+#             */
/*   Updated: 2021/12/04 03:47:59 by nsierra-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 4096
# endif
# define TRIGGER_CHAR '\n'

char	*get_next_line(int fd);

typedef struct s_gnl_node
{
	char				buffer[BUFFER_SIZE];
	size_t				size;
	ssize_t				nl_position;
	struct s_gnl_node	*next;
}	t_gnl_node;

typedef struct s_gnl
{
	size_t			total;
	int				fd;
	t_gnl_node		*first;
	t_gnl_node		*last;
	struct s_gnl	*next;
}	t_gnl;

ssize_t	find_nl_position(char *buffer, ssize_t size);
char	*flush_buffer_list(t_gnl *gnl);
char	*enqueue_buffer(t_gnl *gnl, char *buffer, ssize_t size, int nl_pos);
t_gnl	*new_init_fd_list(int fd, t_gnl *to_init);
void	printf_buflist(t_gnl *gnl);

#endif
