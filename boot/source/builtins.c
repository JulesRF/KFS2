/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 13:22:14 by rdel-agu          #+#    #+#             */
/*   Updated: 2024/12/12 13:54:37 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/kfs.h"

void    interpretor(char *str)
{
    if (ft_strcmp(str, "hi") == 0)
        print_debug("Hell yea!", RED);
	else if (ft_strcmp(str, "help") == 0)
	{
		print_string("Execute help cmd", WHITE);
	}
}