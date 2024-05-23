/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trischma <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 14:41:51 by trischma          #+#    #+#             */
/*   Updated: 2024/05/23 14:41:52 by trischma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>

static char	*str_join(char *s1, char *s2)
{
	char	*joined_str;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	joined_str = malloc(strlen(s1) + strlen(s2) + 1);
	if (!joined_str)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		joined_str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		joined_str[i + j] = s2[j];
		j++;
	}
	joined_str[i + j] = '\0';
	free(s1);
	return (joined_str);
}

static char	*get_line(char *str)
{
	char	*line;
	size_t	i;

	i = 0;
	if (!str[i])
		return (NULL);
	while (str[i] && str[i] != '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		line[i] = str[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*save_remaining(char *str)
{
	char	*remaining;
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
	{
		free(str);
		return (NULL);
	}
	remaining = malloc(strlen(str) - i + 1);
	if (!remaining)
		return (NULL);
	i++;
	j = 0;
	while (str[i])
		remaining[j++] = str[i++];
	remaining[j] = '\0';
	free(str);
	return (remaining);
}

char	*get_next_line(int fd)
{
	static char	*saved;
	char		*buff;
	char		*line;
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (NULL);
	bytes_read = 1;
	while (!strchr(saved, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, buff, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buff);
			return (NULL);
		}
		buff[bytes_read] = '\0';
		saved = str_join(saved, buff);
	}
	free(buff);
	line = get_line(saved);
	saved = save_remaining(saved);
	return (line);
}
