/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 13:22:14 by rdel-agu          #+#    #+#             */
/*   Updated: 2024/12/20 15:28:33 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/kfs.h"

int		ft_is_only_space(char *str) {
	
	int	i;

	i = 0;
	while (str[i]) {
		
		if (str[i] != ' ')
			return (1);
		i++;
	}
	return (0);
}

int		ft_is_nl(char *str)
{
	if (ft_strlen(str) == 0)
		return (0);
	return (1);
}

void    interpretor(char *str)
{
    if (ft_strcmp(str, "help") == 0)
	{
		print_string("\n--- HELP ---\n", WHITE);
		print_string("help      print a short builtin man\n", WHITE);
		print_string("reboot    not implemented yet\n", WHITE);
		print_string("halt      not implemented yet\n", WHITE);
		print_string("stack     not implemented yet\n", WHITE);
	}
	else if (ft_strcmp(str, "clear") == 0) {
		// TODO
	}
	else if (ft_is_nl(str) == 0)
		print_string("\n", WHITE);
	else if (ft_is_only_space(str) == 0)
		print_string("\n", WHITE);
	else{
		print_string("\ncommand not found: ", WHITE);
		print_string(str, WHITE);
		print_string("\n", WHITE);
	}
}