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

static char	*save_remaining(char *str)
{
	char	*remaining;
	size_t	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i])
	{
		free(str);
		return (NULL);
	}
	remaining = malloc(ft_strlen(str) - i + 1);
	if (!remaining)
		return (NULL);
	ft_strcpy(remaining, str + i + 1);
	free(str);
	return (remaining);
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
	line = malloc(i + 2);
	if (!line)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		line[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
	{
		line[i] = str[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*read_and_save(int fd, char **saved)
{
	char	*buff;
	ssize_t	bytes_read;

	buff = malloc(BUFFER_SIZE + 1);
	if (!buff)
		return (NULL);
	if (!*saved)
		*saved = ft_strdup("");
	if (!*saved)
		return (free(buff), NULL);
	bytes_read = 1;
	while (!ft_strchr(*saved, '\n') && bytes_read != 0)
	{
		bytes_read = read(fd, buff, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buff);
			return (NULL);
		}
		buff[bytes_read] = '\0';
		*saved = ft_strjoin(*saved, buff);
	}
	free(buff);
	return (*saved);
}

char	*get_next_line(int fd)
{
	static char	*saved;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	if (!read_and_save(fd, &saved))
		return (NULL);
	line = get_line(saved);
	saved = save_remaining(saved);
	return (line);
}
