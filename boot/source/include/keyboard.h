/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 17:04:10 by rdel-agu          #+#    #+#             */
/*   Updated: 2024/12/12 13:27:09 by rdel-agu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYBOARD_H
# define KEYBOARD_H
# define SIZE_COMMAND 2000

# include "kfs.h"

void    keyboard_init();

extern int     isShiftPressed;
extern int     isCapsPressed;
extern int     isCtrlPressed;
extern char	current_commands[2][SIZE_COMMAND];
extern int		commands_index[2];



#endif