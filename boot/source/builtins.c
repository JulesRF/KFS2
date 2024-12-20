/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 13:22:14 by rdel-agu          #+#    #+#             */
/*   Updated: 2024/12/18 14:48:46 by rdel-agu         ###   ########.fr       */
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

void    interpretor(char *str)
{
    if (ft_strcmp(str, "hi") == 0)
        print_debug("Hell yea!", RED);
	else if (ft_strcmp(str, "help") == 0)
	{
		print_string("--- HELP ---\n", WHITE);
		print_string("help      print a short builtin man\n", WHITE);
		print_string("reboot    not implemented yet\n", WHITE);
		print_string("halt      not implemented yet\n", WHITE);
	}
	else if (ft_strcmp(str, "clear") == 0) {
		// TODO
	}
	else if (ft_is_only_space(str) == 0) {
		// DO NOTHING
	}
	else{
		print_string("command not found: \n", RED);
	}
}